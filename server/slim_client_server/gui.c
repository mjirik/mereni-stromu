/*
 * $Id: gui.c 117 2007-09-18 21:53:25Z Mira $
 */
/**
 * @file gui.c
 * @brief
 * Modul řeší uživatelské rozhraní. Každá obrazovka je tvořena jedním modulem.
 * Ten vždy implementuje  funkce *_init(), *_open(), *_control(), *_close(),
 * *_draw(). V tomto modulu jsou funkce, které v závislosti na tom, co chceme
 * provádět tyto funkce volají. V podstatě jedinou smysluplnou akcí je změna
 * obraovky. Ta se provádí prostřednictvím funkce scr_ch().  Každý modul
 * obrazovky rovněž spravuje data, které se na něm objevují. Většina řetězců je
 * neměnná a tak jsou uloženy v programové části paměti (ve flash). Proto je
 * třeba před změnou obrazovky tyto proměnné nainicializovat. Uživatel se o to
 * starat nemusí, to řeší již zmíněná funcke scr_ch().
 */


#include <stdio.h>
//#include <avr/pgmspace.h>

#include "gui.h"
#include "gui_toolbox.h"
#include "lcd.h"
#include "error.h"
//#include "irc.h"
#include "convert.h"
#include "measure.h"
#include "keyboard.h"
#include "data.h"
#include "msr_scr.h"
//#include "lst_scr.h"
#include "hlp_scr.h"
#include "dat_scr.h"
#include "dlst_scr.h"
//#include "ser_scr.h"
#include "data_serial.h"
#include "stp_scr.h"
#include "keyfcn.h"

// global
AKT_SCR akt_scr;
char * menu_pridat;

void gui_control(void);
void gui_init(void);
void gui_prev_scr(char last_scr);
void ok_scr_init(char *str);
int ok_scr(void);
void scr_draw(char scr);
void scr_ch(char scr);
// global


static char s_ok[]  = "Ok (Enter)";
static char s_storno[]  = "Storno (Esc)";

/// Popisuje jaká je vykreslována obrazovka
AKT_SCR akt_scr = MSR_SCR;


/// Textov� popisek, pro přidání (zákazníka, atd.).
char * menu_pridat = "Pridat";



void scr_open(char scr);
void scr_close(char scr);


void ok_scr_init(char *str){
  clear_dscreen();
  printxyd_P(0,0,(void *)str);

  printxyd_P(3,4,(void *)s_ok);
  printxyd_P(3,24,(void *)s_storno);
}

int ok_scr(void){
  char c;

  if ((c = get_spec()) != 0){
    switch(c){
    case 4: //Enter
      return 1;
      break;
    case 5: //Esc
      return -1;
      break;
    case 21: // F11 - Strorno měření
      kf_zrusit();
      break;
    case 22: //F12
      // Odeslání dat.
      send_data();
      break;
      //case 11:
      //break;
    default:
      break;
    }
    
  }
  return 0;
}


void gui_init(void){

//  list_scr_init();
  data_scr_init();
  measure_scr_init();
 // je treba nastartovat uvodni obrazovku
  measure_scr_open();
  measure_scr_draw();


}
/*  */
void gui_control(void){
  if (lcd_new_frame()){
  
    switch (akt_scr){
    case MSR_SCR:
      measure_scr_control();
      break;
//    case LST_SCR:
//      list_scr_control();
//      break;
    case HLP_SCR:
      help_scr_control();
      break;
    case ERR_SCR:
      error_scr_control();
      break;
    case DAT_SCR:
      data_scr_control();
      break;
    case DLST_SCR:
      dlst_scr_control();
      break;
    case SER_SCR:
      //ser_scr_control();
      break;
    case STP_SCR:
      stp_scr_control();
      break;
    default:
        break;
    }
  }
  
}


///Funkce vykreslí obrazovku. Počítá se s předchozí alokací.
void scr_draw(char scr){
  switch(scr){
    case MSR_SCR:
      measure_scr_draw();
      break;
//    case LST_SCR:
//      list_scr_draw();
//      break;
    case DAT_SCR:
      data_scr_draw();
      break;
    case HLP_SCR:
      help_scr_draw();
      break;
    case ERR_SCR:
      error_scr_draw();
      break;
    case DLST_SCR:
      dlst_scr_draw();
      break;
    case STP_SCR:
      stp_scr_draw();
      break;
    default:
      break;
  }
}

/// Funkce přepne na jinou obrazovku. Zajistí přitom všechny potřebné 
/// zavedení proměnnných
void scr_ch(char scr){
  if (akt_scr == scr){ // nem� smysl alokovat
    scr_draw(scr); // dojde jen k p�ekreslen�
  }
  else{
    scr_close(akt_scr);
    scr_open(scr);
    akt_scr = scr;
    scr_draw(scr);
  }
}


void scr_open(char scr){
  switch(scr){
//  case LST_SCR:
//    list_scr_open();
//    break;
  case MSR_SCR:
    measure_scr_open();
    break;
  case DLST_SCR:
    dlst_scr_open();
    break;
  case DAT_SCR:
    data_scr_open();
    break;
  case SER_SCR:
    //ser_scr_open();
    break;
  case HLP_SCR:
    hlp_scr_open();
    break;
  case ERR_SCR:
    err_scr_open();
    break;
  case STP_SCR:
    stp_scr_open();
    break;
  default:
    break;
  }
}

void scr_close(char scr){
  switch (scr){
//  case LST_SCR:
//    list_scr_open();
//    break;
  case MSR_SCR:
    measure_scr_close();
    break;
  case DLST_SCR:
    dlst_scr_close();
    break;
  case DAT_SCR:
    data_scr_close();
    break;
  case STP_SCR:
    stp_scr_close();
    break;
  default:
    break;
  }
}
/* end of gui.c */
