/* $Id: lcd.c 148 2009-10-25 13:54:05Z mjirik $
 */

/**
 * @file lcd.c
 * @brief
 * Modul obstarává nenižší úroveň pro vypisování dat na LCD.
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "convert.h"
#include "error.h"
#include <avr/pgmspace.h>
#include "watchdog.h"

// global
void lcd_init(void);
void lcd_control(void);
void printxy(char (*screen)[LCD_X_MAX], int x, int y,char * s);
void printc(char c);
void print(char * s);
void clear_screen(char (*screen)[LCD_X_MAX]);
char lcd_new_frame(void);
void printcxyd(int x, int y, char c);
/// Vytiskne n znaků do defaultscreenu na souřadnici x,y.
void printnxyd(int y, int x, char * s, int n);
void printxyd(int y, int x, char * s);
void clear_dscreen(void);
void printxyd_P(int y,int x,void * s);
void printnt_P(char * s);
void lcd_w_inst(char c);
void printnt(char *s);

char (*p_akt_screen)[LCD_X_MAX];
// Je to sice trochu divný, ale funguje to.
// Představuje to ukazatel na pole 20x4 nebo spíš 20xN
// Je to proto, že ten ukazatel ukazuje na pole ukazatelů na dvacetice znaků char. viz Herout str 218.

char default_screen[LCD_Y_MAX][LCD_X_MAX];

//global


#define F_CPU 8000000UL ///< Frekvence oscilátoru  8MHz
//#define LCD_X_MAX 40
//#define LCD_Y_MAX 4


//char konstanta[][6] PROGMEM = {"Ahoj","huua"};//{'A','h','o','j','\n'};

//PGM_P ji = konstanta[0];
//const prog_char *tu = &konstanta[0];
//char * konstanta_p = &konstanta[0]; 

//PSTR("hoj");

//char *str = "Toto";

//PORTB
//0 rs
//1 r/w  // pokud jsou použity dva e, tak tohle je e2
//2 e
//
//PORTA
//d0 .. d7

int lcd_ctrl_count = 0;
int lcd_ctrl_x = 0;
int lcd_ctrl_y = LCD_Y_MAX ;

/// @returns
/// Funkce vrátí 1 pokud se při dalším zavolání lcd_control začne vykreslovat nový frame
/// jinak vrátí nula
char lcd_new_frame(void){
  if (lcd_ctrl_y == LCD_Y_MAX)
    return 1;
  else
    return 0;
}


/// Jen taková zdržovací funkce pro odlazování
void d10(void){
  int i;//250
  for (i=0; i< 250; i++){
    _delay_us(1);
  }
}

/// Vymaže default screen.
void clear_dscreen(void){
  clear_screen(default_screen);
}


// Vymaže obsah lcd_data
void clear_screen(char (*screen)[LCD_X_MAX]){
  int i,j;

  for (i=0; i < LCD_X_MAX; i++){
    for (j = 0; j < LCD_Y_MAX; j++){
      screen[j][i] = ' ';
    }
  }
}

/**
 * Funkce vytiskne znak na místo kurzoru. Funkce není prováděna ve "vlákně".
 */
void printc(char c){
  d10();
  PORTA = c;
  PORTB = ((PORTB & 0xf8) | (5 & 0x07));
  d10();
  PORTB = ((PORTB & 0xf8) | (1 & 0x07));
  d10();
  PORTB = ((PORTB & 0xf8) | (5 & 0x07));
}

/**
 * Funkce vypíše na současnou pozici kurzoru text z programové paměti.
 * Funkce není napsána do "vlákna".
 */
void printnt_P(char * s){
  int i=0;
  char c;
  while ((c = pgm_read_byte_near(s+i)) != '\0'){
    printc(c);
    i++;
  }
}

void printnt(char *s){
  int i=0;
  char c;
  while ((c = s[i]) != '\0'){
    printc(c);
    i++;
  }
}
///vytiskne text do default_screenu.
void print(char * s){
  printxy(default_screen,0,0,s);
}

/// Vytiskne znak do defaultscreenu na souřadnici x,y.
void printcxyd(int y, int x, char c){
  if ((x < LCD_X_MAX)&&(y < LCD_Y_MAX)){
    default_screen[y][x] = c;
  }
}

/// Vytiskne n znaků do defaultscreenu na souřadnici x,y.
void printnxyd(int y, int x, char * s, int n){
  // mínus jedna je tam proto, že 30+10 = 40, ale posledni index je 39
  if ((x < LCD_X_MAX)&&(y < LCD_Y_MAX)&&((x+n-1) < LCD_X_MAX)){
    int i;
    for(i = 0; i < n; i++){
      default_screen[y][x+i] = s[i];
    }
  }
}

/// Tiskne do default_screenu
void printxyd(int y, int x, char * s){
  printxy(default_screen, y, x, s);
}

/// Funkce vloží na danou pozici řetězec @a s
void printxy(char (*screen)[LCD_X_MAX],int y,int x,char * s){
  int n = 0;
  int n_x = 0;
  char c;
  if (s == NULL){
    s = "NULL";
  }

  if ((x < LCD_X_MAX)&&(y < LCD_Y_MAX)){
    //Kopíruj do pole lcd
    c = s[0];
    while ((c != '\0') && ( (x + n_x) < LCD_X_MAX)){
      screen[y][x+n_x]=c;
      //default_screen[y][x+n_x] = c;

      n++;
      n_x++;
      c = s[n];
      if(c == '\n'){
        y++;
        n_x = 0;
        n++;
        c = s[n];
        if (y >= LCD_Y_MAX){
          new_error(80);
          break;
        }
        
      }
    }
    if ((x+n_x) > LCD_X_MAX){
      new_error(81);
    }
  }
  else{
    new_error(82);
  }
}

/// Funkce vloží na danou pozici řetězec @a s
void printxyd_P(int y,int x,void * s){
  int n = 0;
  int n_x = 0;
  char c;

  if ((x < LCD_X_MAX)&&(y < LCD_Y_MAX)){
    //Kopíruj do pole lcd
    c = pgm_read_byte_near(s);
    while ((c != '\0') && ( (x + n_x) < LCD_X_MAX)){
      //screen[y][x+n_x]=c;
      default_screen[y][x+n_x] = c;

      n++;
      n_x++;
      c = pgm_read_byte_near(s+n);
      if(c == '\n'){
        y++;
        n_x = 0;
        n++;
        c = pgm_read_byte_near(s+n);
        if (y >= LCD_Y_MAX){
          new_error(80);
          break;
        }
        
      }
    }
    if ((x+n_x) > LCD_X_MAX){
      new_error(81);
    }
  }
  else{
    new_error(82);
  }
}


/// Zapiš instrukci.
void lcd_w_inst(char c){
#ifdef DVA_E
  PORTA = c;
  PORTB = ((PORTB & 0xf8) | (6 & 0x07));
   _delay_ms(10);
  PORTB = ((PORTB & 0xf8) | (0 & 0x07));
   _delay_ms(10);
  PORTB = ((PORTB & 0xf8) | (6 & 0x07));
   _delay_ms(10);
#elif
  PORTA = c;
  PORTB = ((PORTB & 0xf8) | (4 & 0x07));
   _delay_ms(10);
  PORTB = ((PORTB & 0xf8) | (0 & 0x07));
   _delay_ms(10);
  PORTB = ((PORTB & 0xf8) | (4 & 0x07));
   _delay_ms(10);
#endif
}

void lcd_init(void){
  p_akt_screen = default_screen;

  int i;
  //cekej 15ms od náběhu Vcc nad 4,5V
  DDRB = (DDRB & 0xf8)|(7 & 0x07);
  DDRA = 255;

  for (i = 0; i < 3; i++){
    _delay_ms(10);
  }
  PORTB = ((PORTB & 0xf8) | (0 & 0x07));
  _delay_ms(10);
  PORTB = ((PORTB & 0xf8) | (4 & 0x07));
  _delay_ms(10);
  
  lcd_w_inst(0x38);
  lcd_w_inst(0x0c); //0x0e; //-kurzor zapnutý  //0x0c //vypne kurzor
  lcd_w_inst(0x06);
  lcd_w_inst(0x01); //vymaže display a posune kurzor na začátek.

  PORTB = ((PORTB & 0xf8) | (5 & 0x07)); //připrraví na odesílání dat
  
  /*
  printc('A');
  printc('h');
  printc('o');
  printc('j');
  printc(' ');
  printc('S');
  printc('v');
  printc('e');
  printc('t');
  printc('e');
  printc('!');
*/

  //Vymazani lcddata
  
  clear_screen(default_screen);
  /*
  {
  char *us;

  us = (char) konstanta[0];
  us[0] = konstanta[0];
  printxy(default_screen,0,0,us);
  printxy(default_screen,1,0,us);
  printxy(default_screen,2,0,(char *)PSTR("hujer"));
  printxy(default_screen,3,0,(char *)tu);
  }
  //printxy(default_screen,3,3,konstanta);

*/
 // printxy(default_screen,0,10,int2string("  ",0));
  //lcd_print();

}

/**
 * Funkce řídí činnost displeje. 
 * Jejím úkolem je neustále dokola vypisovat obsah lcd_data na displej.
 * Vždy když je zavolána, zjistí si co má vypsat dál a udělá to.
 */
void lcd_control(void){
  //wtch_dbg_info();
  // Jde to po řádku a vypisuje to znak po znaku, na konci řádku 
  // to skočí na další. na posledním řádku to přidá k počítadlu
  // ještě jedničku a pak zafunguje první if. Ten nastaví displej na začátek.
  if (lcd_ctrl_y == (LCD_Y_MAX -1 + 1 )){
  
    switch (lcd_ctrl_count){
#ifndef DVA_E
      case 0:  // pripravime na vynulovani a nastaveni na zacatek 
        //TODO mozna by stacilo presunuti na zacatek
        PORTA = 2;
        PORTB = ((PORTB & 0xf8) | (4 & 0x07));
        break;
      case 1:  //zapisem
        PORTB = ((PORTB & 0xf8) | (0 & 0x07));
        break;
      case 2:  //zase nahodime ecko a připravíme na zápis dat
        PORTB = ((PORTB & 0xf8) | (5 & 0x07));
        break;
#else
      case 0:  // pripravime na vynulovani a nastaveni na zacatek 
        //TODO mozna by stacilo presunuti na zacatek
        PORTA = 2;
        PORTB = ((PORTB & 0xf8) | (6 & 0x07));
        break;
      case 1:  //zapisem
        PORTB = ((PORTB & 0xf8) | (0 & 0x07));
        break;
      case 2:  //zase nahodime ecko a připravíme na zápis dat
        PORTB = ((PORTB & 0xf8) | (7 & 0x07));
        break;
#endif
    }
    if (lcd_ctrl_count != 2)
      lcd_ctrl_count++;
    else{
      lcd_ctrl_count = 0;
      lcd_ctrl_x = 0;
      lcd_ctrl_y = 0;
    }
  }
  else{
    switch (lcd_ctrl_count){
      case 0:  // pripravime na vynulovani a nastaveni na zacatek //TODO mozna by stacilo presunuti na zacatek
#ifndef DVA_E
        {
        uint8_t lcd_ctrl_y_prohod = lcd_ctrl_y;
        //následující řádky prohazujou dryhý a třetí řádek.
        if (lcd_ctrl_y == 2)
          lcd_ctrl_y_prohod = 1;
        else if (lcd_ctrl_y == 1)
          lcd_ctrl_y_prohod = 2;
        PORTA = p_akt_screen[lcd_ctrl_y_prohod][lcd_ctrl_x];
        PORTB = ((PORTB & 0xf8) | (5 & 0x07));
        }
#else
        PORTA = p_akt_screen[lcd_ctrl_y][lcd_ctrl_x];
        PORTB = ((PORTB & 0xf8) | (7 & 0x07));
#endif
        break;
      case 1:  //zapisem
#ifndef DVA_E
        PORTB = ((PORTB & 0xf8) | (1 & 0x07));
#else
        if (lcd_ctrl_y < 2)
          PORTB = ((PORTB & 0xf8) | (3 & 0x07));
        else
          PORTB = ((PORTB & 0xf8) | (5 & 0x07));

#endif
        break;
      case 2:  //zase nahodime ecko a připravíme na zápis dat
#ifndef DVA_E
        PORTB = ((PORTB & 0xf8) | (5 & 0x07));
#else
        PORTB = ((PORTB & 0xf8) | (7 & 0x07));
#endif
        break;
    }

    if (lcd_ctrl_count != 1)
      lcd_ctrl_count++;
    else{
      // Tady se řeší správné čítání. Při dosažení mezních hodnot pole dojde k nastavení souřadnic na další řádek...
      // sem se to dostane vždy po dokončení zápisu znaku (instrukce).
      lcd_ctrl_count = 0;
      if (lcd_ctrl_x < (LCD_X_MAX - 1)){
        //pokud jsme nedosáhli koncce řádku, zvyšujeme jeho souřadnici
        lcd_ctrl_x++;
      }
      else{
        // když jsme na konci řádku, nastavíme souřadnici na nulu a zvýšíme počítadlo řádků.
        lcd_ctrl_x = 0;

        if(lcd_ctrl_y < (LCD_Y_MAX -1 + 1)){
          lcd_ctrl_y++;
        }
        else{
          //pokud jsme se dostali na poslední řádek displaye + 1 , je něco divně
          // tenhle případ by totiž měla chytit první podmínka. Místo posleního řádku + 1 se zpracovává instrukce pro 
          // nastavení displeje na nulu.
          lcd_ctrl_y = 0;
        }
      }
      //lcd_ctrl_y++;
    }
    
  }


}

/* end of lcd.c */
