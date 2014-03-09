/*
 * $Id: keyboard.c 64 2007-08-03 23:04:08Z Mira $
 * 
 * File name: keyboard.c
 * Date:      2006/07/31 22:21
 * Author:    Mira
 */
/**
 * @file keyboard.c
 * @brief
 * Modul obsluhuje klávesnici. Časování je připojeno k přerušení INT1, 
 * data jsou přivedena na libovolný pin. Hodnoty kláves jsou čteny dvěma funkcemi.
 * get_char() a get_spec(). Modul je vybaven výstupním bufferem, takže v případě
 * stisku několika kláves bezprostředně po sobě, nedojde ke ztrátě v důsledku
 * pomalého zpracovávání dalším programem.
 */


#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  // kvůli isprint()

//#include <conio.h>
#include <curses.h>

//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "keyboard.h"
#include "lcd.h"
#include "convert.h"


// global
void keyboard_init(void);
char get_char(void);
void read_key_ncurses(void);
char get_spec(void);
void keyboard_recv_watchdog(void);
void keyb_in_buff(char flag, char kod);
void read_key();
//global

/// Konstanta uvádí, po kolika zavoláních při nedoručeném bitu je resetován příjem.
/// Tímto je ošetřen problém, který vznikal při připojení klaávesnice k běžícímu
/// zařízení. Dorazila pak vždy jen část bitů a při stisku další klávesy byly
/// chybjející data doplněna začátkam následujících. Zavedení resetu problém řeší.
#define KEYB_RECV_WATCH_N 1000

/// Velikost výstupního bufferu klávesnice.
#define KEYB_BUFF_SIZE 3



/// Struktura pro popis stisknuté klávesy.

typedef struct key {
  /// Vlajka ukazující typ klávesy, která byla stisknuta.
  /// - 0 : nic
  /// - 1 : přijat char
  /// - 2 : efko, šipka
  char flag;
 /// Vlastní kód klávesy. V případě že je flag == 0, je to ASCII.
  char kod;
} KEY;



//static int8_t data_counter = 0; ///< Počítadl bitů, které již přišly od klávesnice.
//static char recivedc = 0; ///< Střadač bitů.

static int minuly_kod = 0;
//static uint16_t keyb_recv_watchdog = 0;

/// Buffer pro výstupní kódy. Pracuje se s ním prostřednictvím
/// funkcí keyb_in_buff() a get_key_from_buff().
/// Buffer je pole v němž jsou prvky typu key
/// Buffer je implementován jako nekruhový. Je to z důodu ušetření
/// několika málo bytů. Zvýření výpočetní náročnosti je nevýznamné, nebť
/// velikost bufferu je celkem malá ( KEYB_BUFF_SIZE ). Posouvání
/// každé položky o jedno místo při každém příchodu nové klávesy a
/// každém čtení nezpomaluje běh programu.
static KEY keyb_buff[KEYB_BUFF_SIZE];



// FUNKCE
void zpracuj_kod(char);
static KEY get_key_from_buff(void);

/// Funkce vloží do bufferu další klávesu.
/// Funkce je definována v keyboard.h . Tím je umožněno Načítání
/// kláves i z jiného zdroje než z klávesnice.
/// @param flag Příznak typu klávesy.
/// - 0 : nic
/// - 1 : přijat char
/// - 2 : efko, šipka
///
/// @param kod Vlastní kód klávesy.
void keyb_in_buff(char flag, char kod) {
  int i = 0;

  if (flag == 0)
    return;

  for (i = 0; i < (KEYB_BUFF_SIZE - 1); i++) {
    if ((keyb_buff[i + 1].flag) != 0) {
      keyb_buff[i] = keyb_buff[i + 1];
    } else {
      keyb_buff[i].flag = flag;
      keyb_buff[i].kod = kod;
      return;
    }
  }
  keyb_buff[KEYB_BUFF_SIZE - 1].flag = flag;
  keyb_buff[KEYB_BUFF_SIZE - 1].kod = kod;
}

/// Funkce pro výběr klávesy z bufferu. Při výběru je nutné posunout
/// všechny prvky o jeden vpřed.
static KEY get_key_from_buff(void) {
  int i = 0;
  KEY out;

  out.flag = keyb_buff[0].flag;
  out.kod = keyb_buff[0].kod;
  if (keyb_buff[0].flag == 0)
    return out;

  for (i = 0; i < (KEYB_BUFF_SIZE - 1); i++) {
    if (keyb_buff[i + 1].flag != 0) {
      keyb_buff[i] = keyb_buff[i + 1];
    } else {
      keyb_buff[i] = keyb_buff[i + 1];
      return out;
    }
  }
  keyb_buff[KEYB_BUFF_SIZE - 1].flag = 0;
  return out;
}

void pokus (void ){
#define COLOR2      1            /* barvicka 1. */
#define COLOR1      2            /* barvicka 2. */


   int      c;
   char      *s;

   initscr ();
   cbreak ();
   noecho ();                  /* vypnuti echa */
   start_color ();                  /* chceme barvy */
   keypad (stdscr, TRUE);            /* chceme klavesy pod makry KEY_neco */
   curs_set(0);                  /* at tam ta mrska neblika */

   if (!has_colors ()) {            /* umi terminal barvy ? */
       endwin ();
       fputs ("Hmm.. tady barvy nejdou !", stderr);
       exit (1);
   }
   /*         barva - popredi   -       pozadi            */
   init_pair (COLOR1, COLOR_RED,       COLOR_BLUE);      /* barvicka 1. */
   init_pair (COLOR2, COLOR_YELLOW, COLOR_BLACK);      /* a druha */

   attron (COLOR_PAIR( COLOR1 ));                  /* pouzivat barvu 1. */
   mvaddstr (2, 5, "Cervene na modrem");

   attron (COLOR_PAIR( COLOR2 ));
   mvaddstr (3, 5, "Zlute na cernem");

   attron (A_BOLD);                        /* od ted vse BOLD */
   mvaddstr (4, 5, "Zlute na cernem a tucne");

   attroff (COLOR_PAIR( COLOR2 ));                  /* vypne barvu */
   mvhline(LINES-2, 0, ACS_HLINE, COLS);            /* nakresli caru */
   mvaddstr (LINES-1, COLS-15, "F10 - konec");
   mvaddstr (10, 5, "Jmeno klavesy:");

   while ( (c=getch()) != KEY_F(10)) {
           s = (char *) keyname(c);            /* jmeno klavesy ? */
           mvhline(10, 20, ' ', COLS);            /* smaz */
           if (s)
                 mvprintw (10, 20, "'%s'", s);
           else
                 mvprintw (10, 20, "'%c'", (isprint(c) ? c : '.'));
   }
   erase ();                        /* smaz nase vytvory */
   refresh ();
   endwin ();                        /* konec curses */
   //exit (0);                        /* ....bye */

}


/*
SIGNAL(SIG_INTERRUPT1){

  keyb_recv_watchdog = 0;  // 
  
  if ((data_counter > 0)&&(data_counter < 9)){
    recivedc |= ((PINC & 1) << (data_counter - 1));
  }
  else if(data_counter == 10) {
    data_counter = -1;
    //printxy(default_screen, 0,0,s);
    //printxy(default_screen, 0,2, int2string("   ",recivedc));
    zpracuj_kod(recivedc);
    recivedc = 0;
  }
  //printxy(default_screen, 0,2, int2string("    ",data_counter));
  data_counter++;
}
 */
void read_key_ncurses(void) {
  int ch = 0;
  
  // ERR prijde, pokud člověk nestiskne žádnou klávesu
  //  (během 10 ms viz timeout())
  if ((ch = getch()) != ERR) {

    char * s;

    s = (char *) keyname(ch);

    mvhline(10, 20, ' ', COLS); // smaz
    if (s)
      mvprintw(10, 20, "'%s'", s);
    else
      mvprintw(10, 20, "'%c'", (isprint(ch) ? ch : '.'));
        //printf(s);

    switch (ch) {
      case KEY_BACKSPACE:
        // Backspace
        keyb_in_buff(2, 1);
        break;
      case 10:
      case KEY_ENTER:
        //Enter
        keyb_in_buff(2, 4);
        break;
      case 27: //Esc
        keyb_in_buff(2, 5);
        break;
      case KEY_DC:
        //Del
        keyb_in_buff(2, 6);
        break;
      case KEY_UP:// Up
        keyb_in_buff(2, 7);
        break;
      case KEY_DOWN:// Down
        keyb_in_buff(2, 8);
        break;
      case KEY_LEFT: // Left
        keyb_in_buff(2, 9);
        break;
      case KEY_RIGHT: // Right
        keyb_in_buff(2, 10);
        break;
      case KEY_F(1) : //F1
                keyb_in_buff(2, 11);
        break;
      case KEY_F(2) : //F2
                keyb_in_buff(2, 12);
        break;
      case KEY_F(3) : //F3
                keyb_in_buff(2, 13);
        break;
      case KEY_F(4) : //F4
                keyb_in_buff(2, 14);
        break;
      case KEY_F(5) : //F5
                keyb_in_buff(2, 15);
        break;
      case KEY_F(6) : //F6
                keyb_in_buff(2, 16);
        break;
      case KEY_F(7) : //F7
                keyb_in_buff(2, 17);
        break;
      case KEY_F(8) : //F8
                keyb_in_buff(2, 18);
        break;
      case KEY_F(9) : //F9
                keyb_in_buff(2, 19);
        break;
      case KEY_F(10) : //F10
                keyb_in_buff(2, 20);
        break;
      case KEY_F(11) : //F11
                keyb_in_buff(2, 21);
        break;
      case KEY_F(12) : //F12
                keyb_in_buff(2, 22);

      default:
        //@todo  toupper();
        keyb_in_buff(1, ch);
        break;
    }
  }
}



void read_key() {
  char c  = 0;
  
    c = getch();
  switch (c) {
    case 8: 
      // Backspace
      keyb_in_buff(2, 1);
      break;
    case 13:
      //Enter
      keyb_in_buff(2, 4);
      break;
    case 27: //Esc
      keyb_in_buff(2, 5);
      break;
    case -32 :
      c = getch();
      switch (c) {
        case 83:
          //Del
          keyb_in_buff(2, 6);
          break;
        case 72:// Up
          keyb_in_buff(2, 7);
          break;
        case 80:// Down
          keyb_in_buff(2, 8);
          break;
        case 75: // Left
          keyb_in_buff(2, 9);
          break;
        case 77: // Right
          keyb_in_buff(2, 10);
          break;
        case -123 : //F11
          keyb_in_buff(2, 21);
          break;
        case -122 : //F12
          keyb_in_buff(2, 22);
          break;
        default:
          break;
      }

      break;
    case 0:
      c = getch();
      switch (c) {

        case 59: //F1
          keyb_in_buff(2, 11);
          break;
        case 60: //F2
          keyb_in_buff(2, 12);
          break;
        case 61: //F3
          keyb_in_buff(2, 13);
          break;
        case 62: //F4
          keyb_in_buff(2, 14);
          break;
        case 63: //F5
          keyb_in_buff(2, 15);
          break;
        case 64: //F6
          keyb_in_buff(2, 16);
          break;
        case 65: //F7
          keyb_in_buff(2, 17);
          break;
        case 66: //F8
          keyb_in_buff(2, 18);
          break;
        case 67: //F9
          keyb_in_buff(2, 19);
          break;
        case 68: //F10
          keyb_in_buff(2, 20);
          break;
      }
      break;
    default:
      //@todo  toupper();
      keyb_in_buff(1, c);
      break;
  }
}

/// Funkce na základě kódu přijatého z klávesnice vytváří kód pro funkce se kterými
/// pracuje zbytek programu. V případě, že je stisknuta klávesa tisknutelného znaku,
/// je čtecím funkcím poskytnuta jeho jeho ASCII hodnota. V případě stiskutí klávesy
/// jiného typu je nastaven recive_flag a funkce pro čtení běžných znaků ( get_char() )
/// nic nevrátí. Pro načtení kódu speciální klávesy je nutno použít funkci get_spec().
void zpracuj_kod_PC(char c, char minuly_kod) {
  if (minuly_kod == 0) {
    keyb_in_buff(1, c);
  }// špiky, entery, capslocky, F1-F12
  else {
    switch (c) {
      case (0x5a) :
        break;

    }

  }
}

/// Funkce na základě kódu přijatého z klávesnice vytváří kód pro funkce se kterými
/// pracuje zbytek programu. V případě, že je stisknuta klávesa tisknutelného znaku,
/// je čtecím funkcím poskytnuta jeho jeho ASCII hodnota. V případě stiskutí klávesy
/// jiného typu je nastaven recive_flag a funkce pro čtení běžných znaků ( get_char() )
/// nic nevrátí. Pro načtení kódu speciální klávesy je nutno použít funkci get_spec().
void zpracuj_kod(char c) {
  if (minuly_kod == 0) {
    switch (c) {
      case (0x16) :
                keyb_in_buff(1, '1');
        break;
      case (0x1e) :
                keyb_in_buff(1, '2');
        break;
      case (0x26) :
                keyb_in_buff(1, '3');
        break;
      case (0x25) :
                keyb_in_buff(1, '4');
        break;
      case (0x2e) :
                keyb_in_buff(1, '5');
        break;
      case (0x36) :
                keyb_in_buff(1, '6');
        break;
      case (0x3d) :
                keyb_in_buff(1, '7');
        break;
      case (0x3e) :
                keyb_in_buff(1, '8');
        break;
      case (0x46) :
                keyb_in_buff(1, '9');
        break;
      case (0x45) :
                keyb_in_buff(1, '0');
        break;
      case (0x4e) :
                keyb_in_buff(1, '-');
        break;
      case (0x55) :
                keyb_in_buff(1, '=');
        break;
      case (0x66) : //Backspace 1
                keyb_in_buff(2, 1);
        break;

      case (0x0d) : //Ta 2
                keyb_in_buff(2, 2);
        break;
      case (0x15) :
                keyb_in_buff(1, 'Q');
        break;
      case (0x1d) :
                keyb_in_buff(1, 'W');
        break;
      case (0x24) :
                keyb_in_buff(1, 'E');
        break;
      case (0x2d) :
                keyb_in_buff(1, 'R');
        break;
      case (0x2c) :
                keyb_in_buff(1, 'T');
        break;
      case (0x35) :
                keyb_in_buff(1, 'Y');
        break;
      case (0x3c) :
                keyb_in_buff(1, 'U');
        break;
      case (0x43) :
                keyb_in_buff(1, 'I');
        break;
      case (0x44) :
                keyb_in_buff(1, 'O');
        break;
      case (0x4d) :
                keyb_in_buff(1, 'P');
        break;
      case (0x54) :
                keyb_in_buff(1, '[');
        break;
      case (0x5b) :
                keyb_in_buff(1, ']');
        break;
      case (0x5d) :
                keyb_in_buff(1, '\\');
        break;

      case (0x58) : //Caps Lock
                keyb_in_buff(2, 3);
        break;
      case (0x1c) :
                keyb_in_buff(1, 'A');
        break;
      case (0x1b) :
                keyb_in_buff(1, 'S');
        break;
      case (0x23) :
                keyb_in_buff(1, 'D');
        break;
      case (0x2b) :
                keyb_in_buff(1, 'F');
        break;
      case (0x34) :
                keyb_in_buff(1, 'G');
        break;
      case (0x33) :
                keyb_in_buff(1,
                'H');
        break;
      case (0x3b) :
                keyb_in_buff(1, 'J');
        break;
      case (0x42) :
                keyb_in_buff(1, 'K');
        break;
      case (0x4b) :
                keyb_in_buff(1, 'L');
        break;
      case (0x4c) :
                keyb_in_buff(1, ';');
        break;
      case (0x52) :
                keyb_in_buff(1, '\'');
        break;
      case (0x5a) : //Enter
                keyb_in_buff(2, 4);
        break;

      case (0x1a) :
                keyb_in_buff(1, 'Z');
        break;
      case (0x22) :
                keyb_in_buff(1, 'X');
        break;
      case (0x21) :
                keyb_in_buff(1, 'C');
        break;
      case (0x2a) :
                keyb_in_buff(1, 'V');
        break;
      case (0x32) :
                keyb_in_buff(1, 'B');
        break;
      case (0x31) :
                keyb_in_buff(1, 'N');
        break;
      case (0x3a) :
                keyb_in_buff(1, 'M');
        break;
      case (0x41) :
                keyb_in_buff(1, ',');
        break;
      case (0x49) :
                keyb_in_buff(1, '.');
        break;
      case (0x4a) :
                keyb_in_buff(1, '/');
        break;

      case (0x29) :
                keyb_in_buff(1, ' ');
        break;

      case (0x76) : //Esc
                keyb_in_buff(2, 5);
        break;

      case 0x70: // 0_num
        keyb_in_buff(1, '0');
        break;
      case 0x69: // 1_num
        keyb_in_buff(1, '1');
        break;
      case 0x72: // 2_num
        keyb_in_buff(1, '2');
        break;
      case 0x7A: // 3_num
        keyb_in_buff(1, '3');
        break;
      case 0x6B: // 4_num
        keyb_in_buff(1, '4');
        break;
      case 0x73: // 5_num
        keyb_in_buff(1, '5');
        break;
      case 0x74: // 6_num
        keyb_in_buff(1, '6');
        break;
      case 0x6C: // 7_num
        keyb_in_buff(1, '7');
        break;
      case 0x75: // 8_num
        keyb_in_buff(1, '8');
        break;
      case 0x7D: // 9_num
        keyb_in_buff(1, '9');
        break;
      case 0x7B: // -_num
        keyb_in_buff(1, '-');
        break;
      case 0x71: // ._num
        keyb_in_buff(1, '.');
        break;
      case 0x7C: // *_num
        keyb_in_buff(1, '*');
        break;
      case 0x79: // +_num
        keyb_in_buff(1, '+');
        break;



      case (0x05) : //F1
                keyb_in_buff(2, 11);
        break;
      case (0x06) : //F2
                keyb_in_buff(2, 12);
        break;
      case (0x04) : //F3
                keyb_in_buff(2, 13);
        break;
      case (0x0c) : //F4
                keyb_in_buff(2, 14);
        break;
      case (0x03) : //F5
                keyb_in_buff(2, 15);
        break;
      case (0x0b) : //F6
                keyb_in_buff(2, 16);
        break;
      case (0x83) : //F7
                keyb_in_buff(2, 17);
        break;
      case (0x0a) : //F8
                keyb_in_buff(2, 18);
        break;
      case (0x01) : //F9
                keyb_in_buff(2, 19);
        break;
      case (0x09) : //F10
                keyb_in_buff(2, 20);
        break;
      case (0x78) : //F11
                keyb_in_buff(2, 21);
        break;
      case (0x07) : //F12
                keyb_in_buff(2, 22);
        break;
      case (0xe0) :
                minuly_kod = 0xe0;
        break;

      case (0xf0) :
                minuly_kod = 0xf0;
        break;
      default:
        //keyb_in_buff(0;
        //rec_buf = 0;
        break;

    }
  } else if (minuly_kod == 0xe0) {
    switch (c) {
      case 0x71: //Del
        keyb_in_buff(2, 6);
        break;
      case 0x75: // Up
        keyb_in_buff(2, 7);
        break;
      case 0x72: // Down
        keyb_in_buff(2, 8);
        break;
      case 0x6B: // Left
        keyb_in_buff(2, 9);
        break;
      case 0x74: // Right
        keyb_in_buff(2, 10);
        break;
      case 0x5A: // Enter_num
        keyb_in_buff(2, 4);
        break;
      case 0x4A: // /_num
        keyb_in_buff(1, '/');
        break;
      case 0xF0:
        minuly_kod = 0xF0;
        break;
      case 0xE0:
        minuly_kod = 0xE0;
        break;
      default:
        break;
    }
  } else if (minuly_kod == 0xF0) {
    // napriklad pri pusteni sipky
    minuly_kod = 0;
  } else {
    minuly_kod = 0;
  }


}
//TODO
/// Funkce vrátí poslední přijatý znak .
char get_char(void) {
  if (keyb_buff[0].flag == 1) {
    return get_key_from_buff().kod;
  } else
    return 0;
}

/// Funkce vrátí poslední přijatý stisk speciální klávesy.
char get_spec(void) {
  if (keyb_buff[0].flag == 2) {
    return get_key_from_buff().kod;
  } else
    return 0;
}


void keyboard_init(void){
  //pokus();
  /*
  initscr(); //nastartování zvláštního režimu terminálu
  cbreak();
  noecho();
  keypad (stdscr, TRUE);            // chceme klavesy pod makry KEY_neco

  reset_shell_mode();
  
   * */
}

/// V případě že během posledních pěti volání této funkce nedošlo k příjmu
/// řádného bitu od klávesnice, (tedy pokud už dlouho nic nepřišlo)
/// přeruší příjem posledního signálu a nastaví data_counter na nulu používá se k
/// řešení problému s s nesprávným přijetím zprávy a následným posunutím přijmu
/// funkce přijme třeba jen polovinu bitů, ale neví že už je konec vysílání a když 
/// dorazí první polovina dalšího kódu, zapíše je jako druhou část předchozího.
/*
void keyboard_recv_watchdog(void){
  if (keyb_recv_watchdog > KEYB_RECV_WATCH_N ){
    data_counter = 0;
    recivedc = 0;
  }
  else
    keyb_recv_watchdog++;
}
 */

/* end of keyboard.c */
