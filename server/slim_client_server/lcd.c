/* $Id: lcd.c 78 2007-08-14 08:49:52Z Mira $
 */

/**
 * @file lcd.c
 * @brief
 * Modul obstarává nenižší úroveň pro vypisování dat na LCD.
 */

#include <stdio.h>
//#include <avr/io.h>
//#include <util/delay.h>
#include "lcd.h"
#include "convert.h"
#include "error.h"
//#include <avr/pgmspace.h>


// global
void lcd_init(void);
void lcd_control(void);
void printxy(char (*screen)[LCD_X_MAX], int x, int y,char * s);
void printc(char c);
void print(char * s);
void clear_screen(char (*screen)[LCD_X_MAX]);
char lcd_new_frame(void);
void printcxyd(int x, int y, char c);
void printxyd(int y, int x, char * s);
void clear_dscreen(void);
void printxyd_P(int y,int x,void * s);
//void printnt_P(char * s);
void lcd_w_inst(char c);
char default_screen[LCD_Y_MAX][LCD_X_MAX];
char (*p_akt_screen)[LCD_X_MAX];
unsigned int zmena_na_lcd = 1;
//global

//#define F_CPU 8000000UL ///< Frekvence oscil�toru  8MHz
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
int lcd_ctrl_y = LCD_Y_MAX;



char default_screen_tmp[LCD_Y_MAX][LCD_X_MAX];


/// @returns
/// Funkce vrátí 1 pokud se při dalším zavolání lcd_control začne vykreslovat nový frame
/// jinak vrátí nula

char lcd_new_frame(void) {
  if (lcd_ctrl_y == LCD_Y_MAX)
    return 1;
  else
    return 0;
}



/// Vymaže default screen.

void clear_dscreen(void) {
  clear_screen(default_screen);
}


// Vymaže obsah lcd_data

void clear_screen(char (*screen)[LCD_X_MAX]) {
  int i, j;

  for (i = 0; i < LCD_X_MAX; i++) {
    for (j = 0; j < LCD_Y_MAX; j++) {
      screen[j][i] = ' ';
    }
  }
}


///vytiskne text do default_screenu.
void print(char * s) {
  printxy(default_screen, 0, 0, s);
}

/// Vytiskne znak do defaultscreenu na souřadnici x,y.
void printcxyd(int y, int x, char c) {
  if ((x < LCD_X_MAX) && (y < LCD_Y_MAX)) {
    default_screen[y][x] = c;
  }
}



/// Tiskne do default_screenu
void printxyd(int y, int x, char * s) {
  printxy(default_screen, y, x, s);
}

/// Funkce vloží na danou pozici řetězec @a s
void printxy(char (*screen)[LCD_X_MAX], int y, int x, char * s) {
  int n = 0;
  int n_x = 0;
  char c;
  if (s == NULL) {
    s = "NULL";
  }

  if ((x < LCD_X_MAX) && (y < LCD_Y_MAX)) {
    //Kop�ruj do pole lcd
    c = s[0];
    while ((c != '\0') && ((x + n_x) < LCD_X_MAX)) {
      screen[y][x + n_x] = c;
      //default_screen[y][x+n_x] = c;

      n++;
      n_x++;
      c = s[n];
      if (c == '\n') {
        y++;
        n_x = 0;
        n++;
        c = s[n];
        if (y >= LCD_Y_MAX) {
          new_error(80);
          break;
        }

      }
    }
    if ((x + n_x) > LCD_X_MAX) {
      new_error(81);
    }
  } else {
    new_error(82);
  }
}

/// Funkce vloží na danou pozici řetězec @a s
void printxyd_P(int y, int x, void * s) {
  printxyd(y, x, s);

}




/**
 * Funkce ��d� �innost displeje. 
 * Jej�m �kolem je neust�le dokola vypisovat obsah lcd_data na displej.
 * V�dy kdy� je zavol�na, zjist� si co m� vypsat d�l a ud�l� to.
 */
void lcd_control(void) {
  // Funkce zjistuje zda doslo od minuleho volaani ke zmene na lcd
  // Obraz je rozdělen na 16 částí po deseti bytech. Každé části je přiřazen
  // jeden bit z proměnné zmena_na_lcd. Je-li v této části změna, je tento bit
  // mastaven na jedna, jinak je ponechán na nule.
  int i = 0;
  int j = 0;
  zmena_na_lcd = 0;
  // zjištění zda se něco od minule na displejo změnilo
  for (i = 0; i < LCD_Y_MAX; i++) {
    for (j = 0; j < LCD_X_MAX; j++) {
      if (default_screen[i][j] != default_screen_tmp[i][j]) {
        int dec;
        //výpočet indexu (i*LCD_X_MAX+j)
        // kolikátá desatice index / 10
        dec = (i*LCD_X_MAX+j)/10;


        // nastavení příslušného bitu
        zmena_na_lcd = zmena_na_lcd |(1 << dec);
      }
    }
  }

   // nakopírování obrazovky aby bylo příště možné zjistit změny
  if (zmena_na_lcd != 0){
    for (i = 0; i < LCD_Y_MAX; i++) {
      for (j = 0; j < LCD_X_MAX; j++) {
        default_screen_tmp[i][j] = default_screen[i][j];
      }
    }
  }
}

/* end of lcd.c */
