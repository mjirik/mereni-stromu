/* $Id: stp_scr.c 117 2007-09-18 21:53:25Z Mira $ */
/**
 * @file stp_scr.c
 * @brief
 * Modul zajiöùuje vykreslov·nÌ a obsluhu obrazovky nastavenÌ.
 */

#include <stdio.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <inttypes.h>
#include "hlp_scr.h"
#include "lcd.h"
#include "gui.h"
#include "gui_toolbox.h"
#include "convert.h"
#include "measure.h"
#include "irc.h"
#include "keyboard.h"
#include "data.h"
//#include "lst_scr.h"
#include "main.h"
#include "error.h"
#include "eeprom.h"
#include "usart.h"

/// Maxim·lnÌ poËet poloûek menu.
#define MAX_MENU 5

/// Stav uûivatelskÈho interface.
static char  run = 0 ;

static MENU_ITEM *menu[MAX_MENU];

static char s_stp_scr[] PROGMEM = "k_irc_k             lista\nk_irc_l\nv_irc_k\nv_irc_l";

static char  menu_i =0;


void stp_nastav (char parametr, float hodnota);

void stp_scr_open(void){
  int i;
  for (i = 0; i < (MAX_MENU ); i++) {
    menu[i] = new_menu_item();
    menu[i] -> x =(char) 8 + ((i / 4) * 20);
    menu[i] -> y =(char) (i % 4);
    menu[i] -> polozky = NULL;;
    menu[i] -> len = 0;
  }


}

void stp_scr_draw(void){
  clear_dscreen();
  printxyd_P(0,0,(void *)s_stp_scr);
}


void stp_scr_control(void){
  char c;
  if(run == 1){
    char *s;
    if ((s = read_line()) != NULL){
      if (s[0] != '\0'){
        int cislo;
        char s2[10];
        if (sscanf(s,"%i",&cislo) == 0){
          //printxyd(1,30,"blbe");
          printxyd(2,30,s);
        }
        sprintf(s2,"%i",cislo); 
        printxyd(3,30,s2);
        stp_nastav(menu_i,(float)cislo);
      }
      else { // pokud Esc,  nebo je retezec nulove delky
        
      }
      free((void *) s);
      run = 0; 
    }
    
  }
  if ((c = get_spec()) != 0){
    switch(c){
    case 2: //Tab
    case 8:
    case 10:
      menu_i = menu_next_item(menu, menu_i, MAX_MENU);
      break;
    case 7:
    case 9:
      menu_i = menu_prev_item(menu, menu_i, MAX_MENU);
      break;
    case 5: //Esc
      scr_ch(MSR_SCR);
      break;
    case 4: //Enter
      // p¯epoËet z indexu menu v dlst_scr na index seznamu jmen na msr_scr
      {
        char * s = (char *)mmalloc(9); // TODO ölo by to pak p¯ealokovat na menöÌ
        if (s != NULL){
          mezery(s,8);
          s[8] = '\0';
          m_jmena[MAX_MENU] = s;
          read_line_init (menu[(int)menu_i] -> y, menu[(int)menu_i] -> x, s);
          run = 1;
        }
        
      }
      
    default:
      break;
    }
  }
}

/**
Funkce provede s parametrem p¯ÌsluËnou operaci
*/
void stp_nastav (char parametr, float hodnota){
  switch (parametr){
  case 0:
    set_irc_k((uint16_t)hodnota);
    break;
  case 1:
    set_irc_set_on_value((int32_t) hodnota);
    break;
  case 2:
    {
      char * msg;
      msg = (char*)malloc(LenOfMsg);
      msg[0] = '\x10';
      msg[1] = ((uint16_t) hodnota) / 256;
      msg[2] = ((uint16_t) hodnota) % 256;
      new_msg(msg);
      break;
    }
  case 3:
    {
      char * msg;
      msg = (char*)malloc(LenOfMsg);
      msg[0] = '\x11';
      msg[1] = ((uint16_t) hodnota) / 256;
      msg[2] = ((uint16_t) hodnota) % 256;
      new_msg(msg);
      break;
    }
  case 4: // nastavenÌ zdroje mÏ¯enÌ öÌ¯ky
    if ((hodnota <= 2) && (hodnota >= 0)){
      width_src = (unsigned char) hodnota;
    }
    break;
  default:
    break;
  }
}

void stp_scr_close(void){
  int i;

  for ( i = 0; i < MAX_MENU; i++){
    free((void *)menu[i]);
  }
}
