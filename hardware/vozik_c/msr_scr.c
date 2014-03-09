/* $Id: msr_scr.c 117 2007-09-18 21:53:25Z Mira $
 */
/**
 * @file msr_scr.c
 * @brief
 * Hlavní obrazovka mìøení. Menu je alokováno a pøed zobrazením. Hodnoty jsou
 * mezitím uloeny v poli values.
 *
 * Poslední polokou menu je klikátko pro odchod na datovou obrazovku
 * 
 */


#include <stdio.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "msr_scr.h"
#include "hlp_scr.h"
#include "dlst_scr.h"
#include "lcd.h"
#include "gui.h"
#include "gui_toolbox.h"
#include "convert.h"
#include "measure.h"
#include "irc.h"
#include "keyboard.h"
#include "data.h"
//#include "lst_scr.h"
#include "dat_scr.h"
#include "main.h"
#include "error.h"
#include "eeprom.h"
#include "usart.h"
#include "data_serial.h"
#include "keyfcn.h"

static void keyb(void);
void save_menu_values(void);
void load_menu_values(void);


//static MENU_ITEM *p_a= NULL;
//static MENU_ITEM *p_b =NULL;
//

/// Definice poètu poloek menu
#define VELIKOST_MENU 6

/// Definice poètu poloek menu, které mohou nabıvat nìjakıch hodnot.
/// Ostatní poloky jsou "nehodnotové". Jsou to napøíklad klikátka pro odchod
/// na jinou obrazovku.
#define VELIKOST_MENU_HODNOT 5


static MENU_ITEM *menu[VELIKOST_MENU];

/// @name PROGMEM
/// Texty jsou uloeny v programové pamìti. Dojde tak k úspoøe místa
// @{
static char s_zakz[] PROGMEM = "Zakz";
static char s_drev[] PROGMEM = "Drev";
static char s_oper[] PROGMEM = "Oper";
static char s_delka[] PROGMEM = "Delka";
static char s_prumer_v[]PROGMEM = "prum_v";
static char s_prumer_h[]PROGMEM = "prum_h";
static char s_dat_scr[]PROGMEM = "Data";
static char s_smazat[] PROGMEM = " \n  Opravdu odstranit jmeno ze seznamu?";
static char s_msr_zahajeno[] PROGMEM = "Mereni zahajeno   ";
static char s_msr_delka[] PROGMEM =    "Zmerena delka     ";
static char s_msr_domereno[] PROGMEM = "Mereni dokonceno  ";
static char s_msr_pause[] PROGMEM =    "Mereni pozastaveno";
// @}

//static char s_nelze_smazat[] PROGMEM = "Jmeno nelze odstranit pokud je\n na nej odkaz mezi namerenymi kmeny!";


//TODO pøed spuštìním je tøeba nastavit všechno na null,
//aby se to pøi kadém vypnutí a zapnutí nezaplòovalo z EEpromky
//char * m_jmena[MAX_ZAKAZNIK];
//static char jmena_len = 3;



/// @name Hodnoty poloek menu.
/// Pole øeìzcù, ketré jsou vypisovány pro pøíslušné volby.
/// @todo Pøesunout tyto texty do PROGMEM
//@{
static char * m_drevo[] = {"Smrk", "Bor-kura","Bor-borka","Buk","Dub"}; // menit i v dat_scr.c
static char * m_katrem[] = {"1", "2"};
static char * m_dalsi[] = {"", "samo", "paso"};
static char * m_manipulace[] = {"", "man"};
//@}
static char menu_i = 0;

static uint8_t menu_values[VELIKOST_MENU_HODNOT]={0,0,0,0,0};



void save_menu_values(void){
  int i = 0;
  
  for (i=0; i < VELIKOST_MENU_HODNOT; i++){
    menu_values[i] = menu[i] -> val;
  }
}

void load_menu_values(void){
  int i = 0;
  
  for (i=0; i < VELIKOST_MENU_HODNOT; i++){
    menu[i] -> val = menu_values[i];
  }
}

void measure_scr_init(void){
//  int i;

  //free_m_jmena();

//for (i = 0; i < MAX_ZAKAZNIK; i++){
//  m_jmena[i] = NULL;
//}

  //m_jmena[0] = "Fanda";
  //m_jmena[2] = "Jarda";
  //m_jmena[3] = "Tonda";

  // poloka pøidat je na posledním místì v seznamu poloky pøed ní jsou
  // NULL a jsou tudí pøeskakovány. V pøípadì naplnìní je na poslední
  // místo místo nápisu "pridat" uveden NULL. Tím dojde k jeho nevypsání
//  m_jmena[MAX_ZAKAZNIK - 1] = menu_pridat;
//  m_jmena[MAX_ZAKAZNIK] = menu_pridat;


  //menu_jmena_i = &(menu[0] -> val);

}

void measure_scr_open(void){
  int i = 0;

  for (i = 0; i < VELIKOST_MENU; i++){
    menu[i] = new_menu_item();
  
    menu[i] -> val = 0;
  }

  menu[0] -> x = 7;
  menu[0] -> y = 0;
  menu[0] -> polozky = m_jmena;
  menu[0] -> len = MAX_ZAKAZNIK;

  menu[1] -> x = 7;
  menu[1] -> y = 1;
  menu[1] -> polozky = m_drevo;
  menu[1] -> len = 5;

  menu[2] -> x = 7;
  menu[2] -> y = 2; 
  menu[2] -> polozky = m_katrem;
  menu[2] -> len = 2;

  menu[3] -> x = 9;
  menu[3] -> y = 2; 
  menu[3] -> polozky = m_dalsi;
  menu[3] -> len = 3;

  menu[4] -> x = 14;
  menu[4] -> y = 2; 
  menu[4] -> polozky = m_manipulace;
  menu[4] -> len = 2;

  menu[5] -> x = 7;
  menu[5] -> y = 3;
  menu[5] -> polozky = NULL;
  menu[5] -> len = 0;
  menu[5] -> val = 255;

  load_menu_values();
}

void measure_scr_close(void){
  int i = 0;

  save_menu_values();

  for (i = 0; i < VELIKOST_MENU; i++){
    free((void *)menu[i]);
    menu[i] = NULL;
  }
  
}



void measure_scr_draw(void){
  clear_dscreen();
  akt_scr = MSR_SCR;
  printxyd(0,0, strcpy_P(str_buf,s_zakz));
  printxyd(1,0, strcpy_P(str_buf,s_drev));
  printxyd(2,0, strcpy_P(str_buf,s_oper));
  printxyd_P(3,0, (void*) s_dat_scr);

  printxyd(1, 25, strcpy_P(str_buf,s_delka));
  printxyd(2, 25, strcpy_P(str_buf,s_prumer_v));
  if (width_src > 0){
    printxyd(3, 25, strcpy_P(str_buf,s_prumer_h));
    printcxyd(3,37, '.');
  }

  printcxyd(1,37, '.');
  printcxyd(2,37, '.');

  menu_draw(menu,VELIKOST_MENU,menu_i);

}



/// Funkce udruje LCD aktuální. Vykresluje nepøetritì správné údaje.
/// Na konci je volána funkce pro kontrolu klávesnice.
/// @todo Bylo by moné zde získat rychlost. Øada údajù se vykresluje
/// nepøetritì, pøitom by staèilo je aktualizovat poze pøi zmìnì
/// promìnné msr_status.
void measure_scr_control(void){
  //uint16_t l,d_v,d_h;

// vykreslování stavu
  switch (msr_status){
  case L_NEZMERENO:
    printxyd_P(0,20,(void *)s_msr_zahajeno);
    break;
  case L_ZMERENO:
    printxyd_P(0,20,(void *)s_msr_delka);
    break;
  case ZMERENY_PRUMERY:
    printxyd_P(0,20,(void *)s_msr_domereno);
    break;
  case PAUSE_L_NEZMERENO:
  case PAUSE_L_ZMERENO:
    printxyd_P(0,20,(void *)s_msr_pause);
    break;
  default:
    printxyd(0,20,mezery(str_buf,20));
    break;
  }

  if (msr_status >= L_ZMERENO){
    //l = msr_delka;
    printxyd(1,19, int2strn(str_buf,2,(msr_delka / 100)));
    str_buf[0] = '0';
    printxyd(1,22, int2strn(str_buf,2,(msr_delka % 100)));
    printcxyd(1,21,'.');
    str_buf[0] = ' ';

  }
  else{
    printxyd(1,18,mezery(str_buf,7));
//    l = irc_get_l();
  }
  
  if (msr_status == ZMERENY_PRUMERY){

    printxyd( 2,18, int2strn(str_buf, 3,(uint16_t)(msr_prumer_v / 10)));
    
    printcxyd( 2,22,(msr_prumer_v % 10) + '0');
    printcxyd( 2,21,'.');

    if (width_src > 0){
      printxyd( 3,18, int2strn(str_buf,3,(msr_prumer_h / 10)));
      printcxyd(3,22, (msr_prumer_h % 10) + 48);
      printcxyd(3,21,'.');
    }


//    printxyd( 0,35, int2strn(str_buf,1,(msr_objem / 1000)));
//    printxyd( 0,37, int2strn(str_buf,3,(msr_objem % 1000)));
//    printcxyd(0,36,'.');
  }
  else{
    printxyd( 2,18, mezery(str_buf,7));
    printxyd( 3,18, mezery(str_buf,7));
    //printxyd( 0,35, mezery(str_buf,7));
  }
  
//  printcxyd( 3,10, msr_status + '0');
//  printxyd( 3,13, int2strn(str_buf,2,msr_pocet_mereni));    

  printxyd( 1,34, int2strn(str_buf,3,(irc_get_l() / 100)));
  str_buf[0] = '0';
  printxyd( 1,38, int2strn(str_buf,2,(irc_get_l() % 100)));
  str_buf[0] = ' ';

  printxyd( 2,34, int2strn(str_buf,3,(prumer_v / 10)));


  printcxyd( 2,38,(prumer_v % 10) + 48);
  
//  printxyd(,34,"00");
  if (width_src >  0){
    printxyd( 3,34, int2strn(str_buf,3,(prumer_h / 10)));
    printcxyd( 3,38, (prumer_h % 10) + 48);
  }
  
  
//  printxyd( 3,35, int2strn(str_buf,4,msr_prumer) );
 
  keyb();
}

static void keyb(void){
  char c;
  static char run = 2;
  if (run == 1){
    char * s;
    if ((s = read_line()) != NULL){
      if (s[0] != '\0'){
        //tady by to šlo pøealokovat
        // TODO pamì
        m_jmena[MAX_ZAKAZNIK - 1] = menu_pridat;
        dat_add_zakaznik(s);
      }
      else { // pokud Esc,  nebo je retezec nulove delky
        free((void *) m_jmena[menu[0] -> len - 1]);
        m_jmena[menu[0] -> len - 1] = menu_pridat;
      }
      run = 2;
    }
  }
  else if (run == 3){  // Potvrzení pøed smazáním jména
    clear_dscreen();
    ok_scr_init((char *) s_smazat);
    
    switch (ok_scr()){
    case -1:
      measure_scr_draw();
      run = 2;
      break;
    case 1:
      run = 2;
      dat_odstran_zakaznika(menu[0] -> val);
      menu_next_value(menu, menu_i);
      scr_draw(MSR_SCR);
      //measure_scr_draw();
      break;
    default:
      break;
    }
    //  }else if(run == 4){
    //    clear_dscreen();
    //    ok_scr_init((char *) s_nelze_smazat);
    //    if (ok_scr() != 0){
    //      scr_draw(MSR_SCR);
    //      //measure_scr_draw();
    //      run = 2;
    //    }
    
  }else if ((c = get_char()) != 0){
    switch(c){
    case ' ':
      menu_next_value(menu, menu_i);
      break;
    case '1':
      menu_next_value(menu, 0);
      break;
    case '2':
      menu_next_value(menu, 1);
      break;
    case '3':
      menu_next_value(menu, 2);
      break;
    case '4':
      menu_next_value(menu, 3);
      break;
    case '5':
      menu_next_value(menu, 4);
      break;
      
    default:
      break;
    }
  }
  else if ((c = get_spec()) != 0){
    switch (c){
    case 1:
      //menu_next_value(menu, menu_i);
      break;
    case 2:  //Tab
    case 8:  //DownArrow
    case 10: //RightArrow
      menu_i = menu_next_item(menu, menu_i, VELIKOST_MENU);
      break;
    case 7:
    case 9:
      menu_i = menu_prev_item(menu, menu_i, VELIKOST_MENU);
      break;
    case 4: //Enter
      if (menu_i == 0){
        
        if ((menu[0] -> val == (MAX_ZAKAZNIK - 1))&&
          (m_jmena[menu[0] -> val] == menu_pridat)){ // spouštìní zaávání nového èlovìka
        
        char * s = (char *)mmalloc(9); // šlo by to pak pøealokovat na menší
        if (s == NULL)
          new_error(88);
        else{
          mezery(s,8);
          m_jmena[MAX_ZAKAZNIK - 1] = s;
          read_line_init (menu[0] -> y, menu[0] -> x, s);
          run = 1;
        }
          }
          else{
            // zde je øešeno pøedávání parametrù jména zákazníka do
            // obrazovky zákazníkù
            scr_ch(DLST_SCR);
          }
      }
      else if(menu_i == 5){
        scr_ch(DAT_SCR);
      }
      else{
        menu_next_value(menu, menu_i);
        //	scr_ch(LST_SCR);
        //list_scr_draw();
      }
      break;
    case 6: //Del
      if (menu[0] -> val != 255){ // nelze mazat "prázdné"
        // nutno testovat, zda záznam není pøiøazen k nìkterému stromu
        if (find_last_record_of_zakaznik(menu[0] -> val) != -1){
          new_error(95);
          //	  ok_scr_init((char *) s_nelze_smazat);
          //	  run = 4;
        }
        else{
          
          ok_scr_init((char *)s_smazat);
          run = 3;
        }
      }
      break;
    case 11:  /// - F1 : Help screen
      //lcd_init(); /// @todo odstanil jsem to, nevim co se stande
      scr_ch(HLP_SCR);
      //help_scr_draw();
      break;
    case 12:  /// - F2 : Data screen
      scr_ch(DAT_SCR);
      //data_scr_draw();
      break;
    case 13:  /// - F3 : Stálí zákazníci
      //assert(1==5);
      //scr_ch(LST_SCR);
      //data_scr_draw();
      break;
    case 14:  /// - F4 : Error screen
      scr_ch(ERR_SCR);
      //error_scr_draw();
      //p_akt_screen = error_screen;
      break;
    case 15:  /// - F5 : Transmitt buffer screen
      /// @warning zobrazení odesílacího bufferu bude pro provoz
      /// odstaveno. Pro odlazování by však tento prostøedek mohl bıt
      /// uitešènı.
      //      scr_ch(SER_SCR);
      break;
    case 16: 
      scr_ch(STP_SCR);
      break;
      /*{ 
      int i=0;
      char * s;
      s = (char *)mmalloc (10);
      strcpy(s,"Nazdar !");
      new_msg(s);
      //new_error(11);
      for (i=0; i < 2000; i++){
      _delay_ms(1);
      }
      break;
      }*/
    case 19:  //F9
      kf_start();
      break;
    case 20:  //F10
      if (msr_status == ZMERENY_PRUMERY){
        if (menu[0] -> val == 255){
          new_error(94);
        }
        else{
          uloz_posledni_data();
          msr_status = VOLNO;
        }
      }
      break;
    case 21: // F11 - Strorno mìøení
      kf_zrusit();
      break;
    case 22: //F12
      // Odeslání dat.
      send_data();
      break;
    default:
      break;
    }
    
  }
}

/**
 * Funkce má za úkol pøipravit z promìnnıch, které jsou k dispozici data pro
 * uloení a pak je i uloit.
 * Je volána po stisknutí tlaèítek pro zahájení mìøení, nebo pro konec mìøení,
 * nebo pak z vnìjšku v pøípadì vıpadku napájení, aby nezùstal poslední
 * záznam neuloenı,
 * */
  void uloz_posledni_data(void){
    if ((msr_status == ZMERENY_PRUMERY)&&((menu[0] -> val != 255))){ ///@todo zauvaovat jestli by se nemìl
								     ///ukládat i bezejmenı
    char scr_zakaznik = menu[0] -> val;
    char scr_drevo = menu[1] -> val;
    char scr_operace = 0 | (((menu[2] -> val) & 1) << 3);
    scr_operace |= ((menu[3] -> val) & 3) << 1;
    scr_operace |= (menu[4] -> val) & 1;
    
    new_record(scr_zakaznik, scr_drevo, scr_operace, msr_delka, msr_prumer);
  }
}


/**
 * Funkce slouí pro získání aktuálního zákaz z vnìjšku
 * Napøíklad ji vyuívá modul pro vykreslování seznamu
 * dnešních zákazníkù. Implementace s testováním promìnné
 * akt_scr se ukázala jako nevhodná, nebo modul dlst_scr
 * volá funkci poté co je ji msr_scr ve stavu closed, ale
 * akt scr na nìj stále ukazuje, nebo ještì stále není
 * otevøen dlst_scr. Proto byla implementace zmìnìna a funkce
 * vdy vrací hodnotu ze záloního pole menu_values,
 * která v pøípadì, e je právì vykreslován msr_scr není
 * aktuální.
 * @return Hodnotu val z poloky menu, která zobrazuje jména
 * zákazníkù. Jednodušeji - Kolikátı zákazník byl naposledy
 * vybrán v msr_scr.
 */
unsigned char get_stored_menu_jmena_i(void){
/*  if (akt_scr == MSR_SCR){
    return menu[0] -> val;
  }
  else {
    return menu_values[0];
  }
*/
  return menu_values[0];
}


/** 
 * Funkce vrací hodnotu menu_i.
 * @warning 
 * V nìkterıch situacích je vhodnìjší volat funkce get_stored_menu_jmena_i().
 * Jedná se zejména o situace bìhem pøepínání obrazovek.
 */ 
unsigned char get_menu_jmena_i(void){
  if (akt_scr == MSR_SCR){
    return menu[0] -> val;
  }
  else {
    return menu_values[0];
  }
}

/**
 * Funkce slouí k nastavení aktuálního zákazníka z vnìjšku.
 * Napøíklad ji vyuívá modul pro vykreslování seznamu
 * dnešních zákazníkù.
 */
void set_menu_jmena_i(unsigned char i){
  if (akt_scr == MSR_SCR){
    menu[0] -> val = i;
  }
  else{
    menu_values[0] = i;
  }
}

/* end of msr_scr.c */
