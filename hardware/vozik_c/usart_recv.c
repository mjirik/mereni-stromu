/*
 *
 * $Id: usart_recv.c 117 2007-09-18 21:53:25Z Mira $
 */
/**
 * @file usart_recv.c
 * @brief
 * Tento soubor se pøipojuje k souboru usart.c
 * Je zde naimplementována jen jedna funkce process_msg
 */

#include <string.h> 
#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "usart_recv.h"
#include "usart.h"
#include "eeprom.h"
#include "irc.h"
//#include "lcd.h"
//#include "convert.h"
#include "error.h"
#include "measure.h"

static char msg0A[LenOfMsg + 1] PROGMEM = "\x0A    ";
static char msg14[LenOfMsg + 1] PROGMEM = "\x14ok  ";


/**
 * Funkce je spouštìna po pøijetí zprávy. Rozhoduje o tom, co bude po pøijetí uèinìno.
 */
void recv_msg(char * recived_msg){
  
  switch (recived_msg[0]){
   
  case '\x04':
    new_error(4);
    break;
  case '\x07':
    new_error(7);
    break;

  case '\x0A':{
    eeprom_save();
    
    new_msg_P(msg0A); 	/* pøepošle zprávu na katr */
    new_msg_P(msg14);
  }
    break;
    
  case '\x0E':
    //nastavení pøenásobovací konstanty
    set_irc_k((uint16_t)((uint16_t)(recived_msg[1] * 256) + recived_msg[2]));
    
    //uložení do eeprom
    eeprom_w16((void *)ee_irc_k, (uint16_t)irc_get_k());
    new_msg_P(msg14);
    break;
    
  case '\x0F':
    set_irc_set_on_value((uint32_t)((uint32_t)(recived_msg[1] * 256) + recived_msg[2]));
    eeprom_w32((void *)ee_irc_set_on_value,(uint32_t) irc_set_on_value);
    new_msg_P(msg14);
    break;
  case '\x10':{
    char * msg;
    msg = (char*)malloc(LenOfMsg);
    memcpy(msg,recived_msg,LenOfMsg);
    new_msg(msg);
    new_msg_P(msg14);
    break;
  }
  case '\x11':{
    char * msg;
    msg = (char*)malloc(LenOfMsg);
    memcpy(msg,recived_msg,LenOfMsg);
    new_msg(msg);
    new_msg_P(msg14);
    break;
  }
  case '\x14':
    msg_ack_fcn(recived_msg);
    break;
    

    /*
    case 8:  //vrací honoru irc_counteru
      {
        char *s = "\x07    ";
        char *c = (char *)&irc_counter;
        
        s[1] = c[0];
        s[2] = c[1];
        s[3] = c[2];
        s[4] = c[3];
        
      new_msg(s);
      }
      break;
    */
  case '\x1A':
    width_src = recived_msg[1];
    break;

  case '\x40':
    
    prumer_v = (recived_msg[2]*256)+recived_msg[1];
    if (width_src == 1){
      prumer_h = (recived_msg[4]*256)+recived_msg[3];
    }
    break;
  case '\x48': // data ze serveru
    if (width_src == 2){
      prumer_h = (recived_msg[4]*256)+recived_msg[3];
    }
    break;
  default:
    break;
    
  }
}

/** 
 * Funkce odešle zprávu s pøíslušným kódem.
 */
void msg_0A (void){
  new_msg_P(msg0A);
}

/** 
 * Funkce odešle zprávu s pøíslušným kódem.
 */
void msg_14 (void){
  new_msg_P(msg14);
}

/* end of usart_recv.c */
