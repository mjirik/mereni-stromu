/* $Id: hlp_scr.c 117 2007-09-18 21:53:25Z Mira $
 */
/**
 * @file hlp_scr.c
 * @brief
 * Modul spravuje obrazovku nápovìdy. Nápovìdné texty jsou uloženy v
 * programové pamìti, jsou-li vypisovány na obrazovku, jsou doèasnì našteny
 * do SRAM.
 */


#include <avr/io.h>
#include <avr/pgmspace.h>

#include "hlp_scr.h"
#include "msr_scr.h"
//#include "lst_scr.h"
#include "gui.h"
#include "lcd.h"
#include "main.h"
#include "keyboard.h"
#include "measure.h"
#include "data_serial.h"
#include "keyfcn.h"

static char prev_scr;

static char msr1[] PROGMEM = "Obrazovka mereni: F10 - Zahajeni mereni\nF11 - Ulozit mereni\nTab - Polozka, Mezer - Hodnota polozky\nEnter - Seznam";
static char lst1[] PROGMEM = "Seznam: Enter(OK) - Vyber/vlozeni jmena\nEsc(ZRUSIT) - Zpet";
static char dat1[] PROGMEM = "Seznam mereni: \nEnter(OK) - Upravit radek";
static char err1[] PROGMEM = "Chybova obrazovka: \nEnter(OK), Esc(ZRUSIT) - odchod";


void hlp_scr_open(void){
  if (akt_scr != HLP_SCR)
    prev_scr = akt_scr;
  else
    prev_scr = MSR_SCR;
  //TODO ještì by se nechala dopsat nápovìda k help screenu
}

void help_scr_draw(void){
  clear_dscreen();
  akt_scr = HLP_SCR;

  switch(prev_scr){
  case MSR_SCR:
    printxyd_P(0,0,(void *)msr1);
    break;
  case DLST_SCR:
    printxyd_P(0,0,(void *)lst1);
    break;
  case DAT_SCR:
    printxyd(0,0,(void *)dat1);
    break;
  case ERR_SCR:
    printxyd(0,0,(void *)err1);
  default:
    break;
  }
}

void help_scr_control(void){
  char c;
  if ((c = get_spec()) != 0){
    switch(c){
    case 5:
      scr_ch(prev_scr);
      break;
      /// @todo Rozhodnout o implementaci tlaèítka START
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


/* end of hlp_scr.c */
