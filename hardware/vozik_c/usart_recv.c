/*
 *
 * $Id: usart_recv.c 148 2009-10-25 13:54:05Z mjirik $
 */
/**
 * @file usart_recv.c
 * @brief
 * Tento soubor se připojuje k souboru usart.c
 * Je zde naimplementována jen jedna funkce process_msg
 */

#include <string.h> 
#include <inttypes.h>
#include <stdlib.h>

#include "sysinfo.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "usart_recv.h"
#include "usart.h"
#include "eeprom.h"
#include "irc.h"
//#include "lcd.h"
//#include "convert.h"
#include "error.h"
//#include "measure.h"
#include "common/msgn.h"
#include "lcd.h"
#include "inout.h"
#include "memory.h"


static char msg0A[LenOfMsg + 1] PROGMEM = "\x0A    ";
static char msg14[LenOfMsg + 1] PROGMEM = "\x14ok  ";



/**
 * Funkce je spouštěna po přijetí zprávy. Rozhoduje o tom, co bude po přijetí učiněno.
 */
void recv_msg(char * recived_msg){
  msg_time_0();
  switch (recived_msg[0]){
   
  case '\x04':
    new_error(4);
    break;
  case '\x07':
    new_error(7);
    break;

  case '\x0A':{
    eeprom_save();
    
    new_msg_P(msg0A); 	/* přepošle zprávu na katr */
    new_msg_P(msg14);
  }
    break;
    
  case '\x0E':
    //nastavení přenásobovací konstanty
    set_irc1_k_mm((uint16_t)((uint16_t)(recived_msg[1] * 256) + recived_msg[2]));
    
    //uložení do eeprom
    eeprom_w16((void *)ee_irc_k, (uint16_t)irc1_get_k());
    new_msg_P(msg14);
    break;
    
  case '\x0F':
    set_irc1_set_on_value_mm((uint32_t)((uint32_t)(recived_msg[1] * 256) + recived_msg[2]));
    eeprom_w32((void *)ee_irc_set_on_value,(uint32_t) get_irc1_set_on_value());
    new_msg_P(msg14);
    break;
  case '\x10':{
    char * msg;
    msg = (char*)mmalloc(LenOfMsg);
    memcpy(msg,recived_msg,LenOfMsg);
    new_msg(msg);
    new_msg_P(msg14);
    break;
  }
  case '\x11':{
    char * msg;
    msg = (char*)mmalloc(LenOfMsg);
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
    //width_src = recived_msg[1];
    break;

  case '\x40':
/*
    prumer_v = (recived_msg[2]*256)+recived_msg[1];
    if (width_src == 1){
      prumer_h = (recived_msg[4]*256)+recived_msg[3];
    }*/
    break;
  case '\x48': // data ze serveru
/*
    if (width_src == 2){
      prumer_h = (recived_msg[4]*256)+recived_msg[3];
    }
*/
    break;

    // příjem toho co se má zobrazit na LCD,
    // ta +1 u recived_msd je proto, že první byte je identifikátor zprávy
    case MSGN_LCD00:
      printnxyd(0, 0 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD01:
      printnxyd(0, 1 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD02:
      printnxyd(0, 2 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD03:
      printnxyd(0, 3 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD04:
      printnxyd(1, 0 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD05:
      printnxyd(1, 1 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD06:
      printnxyd(1, 2 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD07:
      printnxyd(1, 3 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD08:
      printnxyd(2, 0 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD09:
      printnxyd(2, 1 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD10:
      printnxyd(2, 2 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD11:
      printnxyd(2, 3 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD12:
      printnxyd(3, 0 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD13:
      printnxyd(3, 1 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD14:
      printnxyd(3, 2 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;
    case MSGN_LCD15:
      printnxyd(3, 3 * LCD_NBYTES, recived_msg+1, LCD_NBYTES);
      break;


    case MSGN_MSR_LED_ON:
      io_msr_led_on();
      break;
    case MSGN_MSR_LED_OFF:
      io_msr_led_off();
      break;

    case MSGN_IRC1_OPTIONS:
    {
      uint32_t k1;
      uint32_t q1;
      recive_uint32(recived_msg+1, &k1, &q1);
      io_irc1_options((uint16_t) k1, q1);
    }
    break;
    case MSGN_IRC2_OPTIONS:
    {
      uint32_t k2;
      uint32_t q2;
      recive_uint32(recived_msg+1, &k2, &q2);
      io_irc2_options((uint16_t) k2, q2);
    }
    break;
    case MSGN_SYSINFO:
      si_set_flag(recived_msg[1]);
      break;
    default:
    break;
    
  }
}

/** 
 * Funkce odešle zprávu s příslušným kódem.
 */
void msg_0A (void){
  new_msg_P(msg0A);
}

/** 
 * Funkce odešle zprávu s příslušným kódem.
 */
void msg_14 (void){
  new_msg_P(msg14);
}

/* end of usart_recv.c */
