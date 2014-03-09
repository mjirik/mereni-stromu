/**
 * @file vizualizace.c
 * Soubor obsahuje modul, který zajišťuje vykreslování LCD a stavové diody na
 * obrazovku PC a po sériovém portu totéž odesílá do zařízení na vozíku. 
 */

#include <stdio.h>
#include <curses.h>
#include <stdlib.h>

#include "vizualizace.h"
#include "lcd.h"
#include "gui_toolbox.h"
#include "keyboard.h"
#include "inout.h"
#include "memory.h"


//global
void vizualizace();
void vis_print(char*s);
//global

#define LCD_NCURSOR_X 5
#define LCD_NCURSOR_Y 4
#define COLOR1      1            /* barvicka 1. */
#define COLOR2      2            /* barvicka 2. */
#define COLOR_IND_ON 3
#define COLOR_IND_OFF 3



void lcd_printf(void);
void lcd_ncursor(void);
void vis_ind_cur(void);
void vis_si_cur(void);

/// proměnná určuje, zda bude pro vykreslování použito prostředí ncursor
static int ncursor = 1;


void vis_print(char*s){
  
}


/**
 * Je-li zvoleno grafické prostředí vizualizace, funkce připraví obrazovku
 * pomocí ncurses a vykreslí základní grafické prvky (rámeček lcd atd.).
 */
void vizualizace_init(void){
  if (ncursor == 1){


//   int      c;
//   char      *s;

   initscr ();
   cbreak ();
   noecho ();                  /* vypnuti echa */
   start_color ();                  /* chceme barvy */
   timeout(50);
   keypad (stdscr, TRUE);            /* chceme klavesy pod makry KEY_neco */
   curs_set(0);                  /* at tam ta mrska neblika */
   

   atexit((void *)erase);                        /* smaz nase vytvory */
   atexit((void *)refresh );
   atexit((void *) endwin); // na konci bude přepnut terminál do normálu

   if (!has_colors ()) {            /* umi terminal barvy ? */
       endwin ();
       fputs ("Hmm.. tady barvy nejdou !", stderr);
       exit (1);
   }
   /*         barva - popredi   -       pozadi            */
   init_pair (COLOR1, COLOR_RED,       COLOR_BLUE);      /* barvicka 1. */
   //init_pair (COLOR2, COLOR_YELLOW, COLOR_BLACK);      /* a druha */
   init_pair (COLOR2, COLOR_BLACK, COLOR_GREEN);      /* a druha */


   // barva indikátoru stavu
   init_pair (COLOR_IND_ON, COLOR_GREEN,COLOR_GREEN);      /* a druha */
   init_pair (COLOR_IND_OFF, COLOR_GREEN,COLOR_BLACK);      /* a druha */

   //attron (COLOR_PAIR( COLOR1 ));                  /* pouzivat barvu 1. */
   //mvaddstr (2, 5, "Cervene na modrem");

   attron (COLOR_PAIR( COLOR2 ));

   //ramecek
   
   mvhline(LCD_NCURSOR_Y-1, LCD_NCURSOR_X,ACS_HLINE,40);
   mvhline(LCD_NCURSOR_Y+4, LCD_NCURSOR_X,ACS_HLINE,40);
   mvvline(LCD_NCURSOR_Y, LCD_NCURSOR_X-1,ACS_VLINE,4);
   mvvline(LCD_NCURSOR_Y, LCD_NCURSOR_X+40,ACS_VLINE,4);
   mvaddch(LCD_NCURSOR_Y-1,LCD_NCURSOR_X-1,ACS_ULCORNER);
   mvaddch(LCD_NCURSOR_Y-1,LCD_NCURSOR_X+40,ACS_URCORNER);
   mvaddch(LCD_NCURSOR_Y+4,LCD_NCURSOR_X-1,ACS_LLCORNER);
   mvaddch(LCD_NCURSOR_Y+4,LCD_NCURSOR_X+40,ACS_LRCORNER);
   
   //border(5, 8, 10, 12, 10, 5, 1, 2);
   //mvaddstr (3, 5, "Zlute na cernem");

   //attron (A_BOLD);                        /* od ted vse BOLD */
   //mvaddstr (4, 5, "Zlute na cernem a tucne");

   attroff (COLOR_PAIR( COLOR2 ));                  /* vypne barvu */
   mvhline(LINES-2, 0, ACS_HLINE, COLS);            /* nakresli caru */
   mvaddstr (LINES-1, COLS-15, "F10 - konec");
   mvaddstr (10, 5, "Jmeno klavesy:");

   mvaddstr (12, 5, "");
   attron (COLOR_PAIR( COLOR2 ));
   //move(12,5);
   //mvaddch(12,4,' ');
   //curs_set(0);
  }
}


/**
 * Funkce zobrazuje LCD v konzoli
 */
void vizualizace() {

  read_key_ncurses();
  // vykreslení
  if (zmena_na_lcd != 0) {
    if (ncursor == 1){
      lcd_ncursor();
      vis_ind_cur();
      vis_si_cur();
      move(12,5);
      attron (COLOR_PAIR( COLOR1 ));
    }
    else{
      lcd_printf();
    }

  }

}


void lcd_ncursor(void){
  int i = 0;
  int j = 0;

  attron (COLOR_PAIR( COLOR2 ));
  move(LCD_NCURSOR_Y,LCD_NCURSOR_X);
    //printf("\n\n");
    for (i = 0; i < LCD_Y_MAX; i++) {
      for (j = 0; j < LCD_X_MAX; j++) {
        // ošetření znaků, které nelze zobrazit
        if (default_screen[i][j] == '\377') {
          //printf("%c", '*');
          mvprintw (i+LCD_NCURSOR_Y,j+LCD_NCURSOR_X, "*");
        } else {
          mvprintw (i+LCD_NCURSOR_Y,j+LCD_NCURSOR_X, "%c", default_screen[i][j]);
          //printf("%c", default_screen[i][j]);
        }

      }
      //printf("\n");
    }
    attroff (COLOR_PAIR( COLOR2 ));                  /* vypne barvu */
    //ACS
    //move(20,20);
    mvaddch(12,4,' ');
    //curs_set(0);
}

/**
 * Funkce se stará o vizualizaci indikátoru stavu pomocí curses.
 */
void vis_ind_cur(void){
  // zajímavé je, že většina těch parametrů se nepoužívá :-)
  attron (COLOR_PAIR( COLOR_IND_ON ));
  if (io_get_ind()==0){
    attron (A_NORMAL);
    mvhline(1,43,ACS_BLOCK,2);
  }else{
    attron (A_BOLD);
    mvhline(1,43,ACS_BLOCK,2);
  }
  attroff (COLOR_PAIR( COLOR_IND_ON ));
  move(12,4);
}

void vis_si_cur(void){
  mvprintw(4,50,"%5i",(int)used_mem());
}

void lcd_printf(void){
  int i = 0;
  int j = 0;

    printf("\n\n");
    for (i = 0; i < LCD_Y_MAX; i++) {
      for (j = 0; j < LCD_X_MAX; j++) {
        // ošetření znaků, které nelze zobrazit
        if (default_screen[i][j] == '\377') {
          printf("%c", '*');
        } else {
          printf("%c", default_screen[i][j]);
        }

      }
      printf("\n");
    }

}

void odesli_lcd(){

  int i = 0;
  int j = 0;

  // vykreslení
  if (zmena_na_lcd == 1) {
    printf("\n\n");
    for (i = 0; i < LCD_Y_MAX; i++) {
      for (j = 0; j < LCD_X_MAX; j++) {
        // ošetření znaků, které nelze zobrazit
        if (default_screen[i][j] == '\377') {
          printf("%c", '*');
        } else {
          printf("%c", default_screen[i][j]);
        }

      }
      printf("\n");
    }



  }

}
