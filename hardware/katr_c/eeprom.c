/*
 * File name: eeprom.c
 * Date:      2006/02/08 13:12
 * Author:    Mira
 */

/*
 * nasleduje mapa eeprom
 *
 * 0  1  - irc_counter
 * 2  3  - irc_k
 */ 


#include <avr/eeprom.h> 

#include "eeprom.h"
#include "irc.h"

#define DEFAULT_irc_k 425
#define DEFAULT_irc_set_on_value 450
//int irc_counter = 0;

//deklarace funkci  

//kods
void eeprom_load(void){
  irc_counter = (int32_t)eeprom_r32((void *)ee_irc_counter);
  irc_k = (uint16_t)eeprom_r16((void *)ee_irc_k);
  irc_set_on_value = (int32_t)eeprom_r32((void *)ee_irc_set_on_value);

  //následující kód zajišje poèáteèní nastavení hodnot
  if (irc_k > 10000){
    set_irc_k(DEFAULT_irc_k);
    set_irc_set_on_value(DEFAULT_irc_set_on_value);
  }
}

void eeprom_save(void){
  eeprom_w32((void *)ee_irc_counter, irc_counter);
}

//8 bit
void eeprom_w8(void *uiAddress, uint8_t c){
  //cekame dokud neni volno
  eeprom_busy_wait();
  eeprom_write_byte(uiAddress, c);
  
}

char eeprom_r8(void *uiAddress){
  return eeprom_read_byte(uiAddress);
}


//16bit
void eeprom_w16(void *uiAddress, uint16_t word){
  //cekame dokud neni volno
  eeprom_busy_wait();
  eeprom_write_word(uiAddress, word);
  
}

uint16_t eeprom_r16(void *uiAddress){
  return eeprom_read_word(uiAddress);
}

//32bit
void eeprom_w32(void *uiAddress, uint32_t data){
  //cekame dokud neni volno
  eeprom_busy_wait();
  eeprom_write_block( (void *)&data, uiAddress, 4);
}

uint32_t eeprom_r32(void *uiAddress){
  uint32_t out = 0;
  eeprom_read_block( (void *)&out, uiAddress, 4);
  
  return out;
}



/* end of eeprom.c */
