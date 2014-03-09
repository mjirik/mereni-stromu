/* $Id: ser_scr.c 33 2007-04-20 18:30:12Z Mira $ 
 */

/**
 * @file ser_scr.c
 * @brief
 *
 * Modul zajišuje chod obrazovky na které jsou vypisovány
 * znaky z pøijímacího bufferu.
 * Obrazovka je vyuívána pro odlazení.
 */


#include <inttypes.h>
#include <avr/pgmspace.h>
#include "usart.h"
#include "lcd.h"
#include "convert.h"
#include "gui.h"
#include "main.h"
#include "keyboard.h"
#include "ser_scr.h"


void ser_scr_open(void){
  int i;
  akt_scr = SER_SCR;
  clear_dscreen();
  
  for (i = 0;i < LenOfRecvBuff; i++){
    char c;
    c = usart_get_recv_buff(i);
    printcxyd(i%4,(i/4) * 7, c);
    printxyd(i%4,(i/4) * 7 + 2, int2strn(str_buf,3,(unsigned int)c));    
  }
}

void ser_scr_control(void){
  char c;

  if ((c = get_spec()) != 0){
    switch (c){
    case 5: //Esc
      scr_ch(MSR_SCR);
      //measure_scr_draw();
      break;
    case 13: //refresh
      ser_scr_open();
      break;
    default:
      break;
    }
  }
  //printxy(error_screen,3,0,int2string("     ",pocitadlo_1));
}



