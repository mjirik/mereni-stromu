// $Id: keyfcn.c 117 2007-09-18 21:53:25Z Mira $
/**
@file keyfcn.c
@brief
Modul obsahuje funkce kter� odpov�daj� jednotliv�m kl�ves�m.
Je pou��v�n zejm�na pro kl�vesy, jejich� funkce se v r�zn�ch menu nem�n�.
*/
#include <stdio.h>

#include "keyfcn.h"
#include "measure.h"
#include "error.h"
#include "msr_scr.h"


//global
void kf_zrusit(void);
void kf_start(void);
//global

void kf_zrusit(void){
  switch(msr_status){
  case L_ZMERENO:
  case L_NEZMERENO:
    msr_pause();
    break;
  case PAUSE_L_ZMERENO:
  case PAUSE_L_NEZMERENO:
    msr_storno();
    break;
  default:
    break;
  }
}

void kf_start(void){
  switch(msr_status){
  case ZMERENY_PRUMERY:
    if (get_menu_jmena_i() == 255){
      new_error(94);
    }
    else{
      uloz_posledni_data();
      msr_start();
    }
    break;
  case VOLNO:
    msr_start();
    break;
  case PAUSE_L_NEZMERENO:
  case PAUSE_L_ZMERENO:
    msr_unpause();
    break;
  default:
    break;
  }
}

int kf_global_key(void){
  return 0;
}
