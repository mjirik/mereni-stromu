/*
 * $Id: gui.c 117 2007-09-18 21:53:25Z Mira $
 */
/**
 * @file gui.c
 * @brief
 * Modul �e�� u�ivatelsk� rozhran�. Ka�d� obrazovka je tvo�ena jedn�m modulem.
 * Ten v�dy implementuje  funkce *_init(), *_open(), *_control(), *_close(),
 * *_draw(). V tomto modulu jsou funkce, kter� v z�vislosti na tom, co chceme
 * prov�d�t tyto funkce volaj�. V podstat� jedinou smysluplnou akc� je zm�na
 * obraovky. Ta se prov�d� prost�ednictv�m funkce scr_ch().  Ka�d� modul
 * obrazovky rovn� spravuje data, kter� se na n�m objevuj�. V�t�ina �et�zc� je
 * nem�nn� a tak jsou ulo�eny v programov� ��sti pam�ti (ve flash). Proto je
 * t�eba p�ed zm�nou obrazovky tyto prom�nn� nainicializovat. U�ivatel se o to
 * starat nemus�, to �e�� ji� zm�n�n� funcke scr_ch().
 */

#include <stdio.h>
#include <avr/pgmspace.h>

#include "gui.h"
#include "gui_toolbox.h"
#include "lcd.h"
#include "error.h"
#include "irc.h"
#include "convert.h"
#include "measure.h"
#include "keyboard.h"
#include "data.h"
#include "msr_scr.h"
//#include "lst_scr.h"
#include "hlp_scr.h"
#include "dat_scr.h"
#include "dlst_scr.h"
#include "ser_scr.h"
#include "data_serial.h"
#include "stp_scr.h"
#include "keyfcn.h"


static char s_ok[] PROGMEM = "Ok (Enter)";
static char s_storno[] PROGMEM = "Storno (Esc)";

/// Popisuje jak� je vykreslov�na obrazovka
AKT_SCR akt_scr = MSR_SCR;


/// Textov� popisek, pro p�id�n� (z�kazn�ka, atd.).
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
    case 21: // F11 - Strorno m��en�
      kf_zrusit();
      break;
    case 22: //F12
      // Odesl�n� dat.
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
      ser_scr_control();
      break;
    case STP_SCR:
      stp_scr_control();
      break;
    default:
        break;
    }
  }
  
}


///Funkce vykresl� obrazovku. Po��t� se s p�edchoz� alokac�.
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

/// Funkce p�epne na jinou obrazovku. Zajist� p�itom v�echny pot�ebn� 
/// zaveden� prom�nnn�ch
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
    ser_scr_open();
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
