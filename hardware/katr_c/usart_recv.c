/*
 * $Id: usart_recv.c 71 2007-08-11 22:08:59Z Mira $
 */
/**
 * @file usart_recv.c
 * @brief
 * Tento soubor se p�ipojuje k souboru usart.c
 * Je zde naimplementov�na jen jedna funkce process_msg
 */


#include <inttypes.h>
#include "usart_recv.h"
#include "usart.h"
#include "eeprom.h"
#include "irc.h"
#include "width.h"

void recv_msg(char * recived_msg){

  switch (recived_msg[0]){
   
  case 0:
    break;
  case '\x0A':
    eeprom_save();
    new_msg("\x14ok  ");
    break;
    
  case '\x09':
    if (recived_msg[1] > 0){
      odesilame (recived_msg[1]);
      new_msg("\x14ok  ");
      irc_send_msg(1);
      
    }
    else{
      odesilame (1); // musi to byt, aby i v pripade ze to mlci umelo odpovedet
      new_msg("\x14ok  ");
      odesilame (recived_msg[1]);
   
      
    }
    break;
  case '\x10':
    //nastaven� p�en�sobovac� konstanty
    set_irc_k((uint16_t)((uint16_t)(recived_msg[1] * 256) + recived_msg[2]));
    
    //ulo�en� do eeprom
    eeprom_w16((void *)ee_irc_k, (uint16_t)irc_get_k());
    new_msg("\x14ok  ");
    break;
    
  case '\x11':
    set_irc_set_on_value((uint32_t)((uint32_t)(recived_msg[1] * 256) + recived_msg[2]));
    new_msg("\x14ok  ");
    break;
    
  case '\x13': // vyp�n� p�epo��t�v�n� pr�m�ru na skute�n�
    width_prepocet = recived_msg[1];
    new_msg("\x14ok  ");
    break;
/*    case 8:  //vrac� honoru irc_counteru 
      {
      char *s = "\x07    ";
      char *c = (char *)&irc_counter;
      
      s[1] = c[0];
      s[2] = c[1];
      s[3] = c[2];
      s[4] = c[3];
      
      new_msg(s);
      }
      break; */
  case '\x16':
    // TODO odesl�n� datov� zpr�vy
    irc_send_msg(1);
    break;
  default:
    break;
      
  }
}

/* end of usart_recv.c */
