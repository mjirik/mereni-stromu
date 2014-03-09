/* $Id: lcd.c 78 2007-08-14 08:49:52Z Mira $
 */

/**
 * @file lcd.c
 * @brief
 * Modul obstar�v� neni��� �rove� pro vypisov�n� dat na LCD.
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "convert.h"
#include "error.h"
#include <avr/pgmspace.h>


#define F_CPU 8000000UL ///< Frekvence oscil�toru  8MHz
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
//1 r/w  // pokud jsou pou�ity dva e, tak tohle je e2
//2 e
//
//PORTA
//d0 .. d7

int lcd_ctrl_count = 0;
int lcd_ctrl_x = 0;
int lcd_ctrl_y = LCD_Y_MAX ;

/// @returns
/// Funkce vr�t� 1 pokud se p�i dal��m zavol�n� lcd_control za�ne vykreslovat nov� frame
/// jinak vr�t� nula
char lcd_new_frame(void){
  if (lcd_ctrl_y == LCD_Y_MAX)
    return 1;
  else
    return 0;
}


/// Jen takov� zdr�ovac� funkce pro odlazov�n�
void d10(void){
  int i;
  for (i=0; i< 250; i++){
    _delay_us(1);
  }
}

/// Vyma�e default screen.
void clear_dscreen(void){
  clear_screen(default_screen);
}


// Vyma�e obsah lcd_data
void clear_screen(char (*screen)[LCD_X_MAX]){
  int i,j;

  for (i=0; i < LCD_X_MAX; i++){
    for (j = 0; j < LCD_Y_MAX; j++){
      screen[j][i] = ' ';
    }
  }
}

/**
 * Funkce vytiskne znak na m�sto kurzoru. Funkce nen� prov�d�na ve "vl�kn�".
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
 * Funkce vyp�e na sou�asnou pozici kurzoru text z programov� pam�ti.
 * Funkce nen� naps�na do "vl�kna".
 */
void printnt_P(char * s){
  int i=0;
  char c;
  while ((c = pgm_read_byte_near(s+i)) != '\0'){
    printc(c);
    i++;
  }
}

///vytiskne text do default_screenu.
void print(char * s){
  printxy(default_screen,0,0,s);
}

/// Vytiskne znak do defaultscreenu na sou�adnici x,y.
void printcxyd(int y, int x, char c){
  if ((x < LCD_X_MAX)&&(y < LCD_Y_MAX)){
    default_screen[y][x] = c;
  }
}



/// Tiskne do default_screenu
void printxyd(int y, int x, char * s){
  printxy(default_screen, y, x, s);
}

/// Funkce vlo�� na danou pozici �et�zec @a s
void printxy(char (*screen)[LCD_X_MAX],int y,int x,char * s){
  int n = 0;
  int n_x = 0;
  char c;
  if (s == NULL){
    s = "NULL";
  }

  if ((x < LCD_X_MAX)&&(y < LCD_Y_MAX)){
    //Kop�ruj do pole lcd
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

/// Funkce vlo�� na danou pozici �et�zec @a s
void printxyd_P(int y,int x,void * s){
  int n = 0;
  int n_x = 0;
  char c;

  if ((x < LCD_X_MAX)&&(y < LCD_Y_MAX)){
    //Kop�ruj do pole lcd
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


/// Zapi� instrukci.
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
  //cekej 15ms od n�b�hu Vcc nad 4,5V
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
  lcd_w_inst(0x0c); //0x0e; //-kurzor zapnut�  //0x0c //vypne kurzor
  lcd_w_inst(0x06);
  lcd_w_inst(0x01); //vyma�e display a posune kurzor na za��tek.

  PORTB = ((PORTB & 0xf8) | (5 & 0x07)); //p�iprrav� na odes�l�n� dat
  
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
 * Funkce ��d� �innost displeje. 
 * Jej�m �kolem je neust�le dokola vypisovat obsah lcd_data na displej.
 * V�dy kdy� je zavol�na, zjist� si co m� vypsat d�l a ud�l� to.
 */
void lcd_control(void){
  // Jde to po ��dku a vypisuje to znak po znaku, na konci ��dku 
  // to sko�� na dal��. na posledn�m ��dku to p�id� k po��tadlu
  // je�t� jedni�ku a pak zafunguje prvn� if. Ten nastav� displej na za��tek.
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
      case 2:  //zase nahodime ecko a p�iprav�me na z�pis dat
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
      case 2:  //zase nahodime ecko a p�iprav�me na z�pis dat
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
        //n�sleduj�c� ��dky prohazujou dryh� a t�et� ��dek.
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
      case 2:  //zase nahodime ecko a p�iprav�me na z�pis dat
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
      // Tady se �e�� spr�vn� ��t�n�. P�i dosa�en� mezn�ch hodnot pole dojde k nastaven� sou�adnic na dal�� ��dek...
      // sem se to dostane v�dy po dokon�en� z�pisu znaku (instrukce).
      lcd_ctrl_count = 0;
      if (lcd_ctrl_x < (LCD_X_MAX - 1)){
        //pokud jsme nedos�hli koncce ��dku, zvy�ujeme jeho sou�adnici
        lcd_ctrl_x++;
      }
      else{
        // kdy� jsme na konci ��dku, nastav�me sou�adnici na nulu a zv���me po��tadlo ��dk�.
        lcd_ctrl_x = 0;

        if(lcd_ctrl_y < (LCD_Y_MAX -1 + 1)){
          lcd_ctrl_y++;
        }
        else{
          //pokud jsme se dostali na posledn� ��dek displaye + 1 , je n�co divn�
          // tenhle p��pad by toti� m�la chytit prvn� podm�nka. M�sto poslen�ho ��dku + 1 se zpracov�v� instrukce pro 
          // nastaven� displeje na nulu.
          lcd_ctrl_y = 0;
        }
      }
      //lcd_ctrl_y++;
    }
    
  }


}

/* end of lcd.c */
