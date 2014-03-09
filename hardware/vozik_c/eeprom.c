/* $Id: eeprom.c 120 2007-09-21 05:43:42Z Mira $
 *
 */

/**
 * @file eeprom.c
 * @brief
 * Modul spravuje pamÏù EEPROM. Jsou zde funkce pro ukl·d·nÌ a naËÌt·nÌ.
 * 
 */

/*
 * nasleduje mapa eeprom
 *
 * 0  1  - irc_counter
 * 2  3  - irc_k
 */ 


#include <avr/eeprom.h>
#include <avr/wdt.h>

#include "eeprom.h"
#include "irc.h"
#include "data.h"
#include "msr_scr.h"
#include "watchdog.h"
#include "measure.h"
//#include "lst_scr.h"

#define DEFAULT_irc_k 336
#define DEFAULT_irc_set_on_value 4300
//int irc_counter = 0;

//deklarace funkci  

//kods
void eeprom_load(void){
  char flag; // ukazuje, zda doölo k uloûenÌ dat
  void * p_eeprom;
  irc_counter = (int32_t)eeprom_r32((void *)ee_irc_counter);
  irc_k = (uint16_t)eeprom_r16((void *)ee_irc_k);
  irc_set_on_value = (int32_t)eeprom_r32((void *)ee_irc_set_on_value);
  width_src = (unsigned char) eeprom_r8((void *)ee_width_src);
  width_src = width_src < 3?width_src:1;
  
  flag = eeprom_r8((void *)ee_flag);
  if (flag == 63){
    width_src = eeprom_r8((void *)ee_width_src);
    //lst_read_eeprom((void *)ee_lst);
    p_eeprom = dat_zak_read_eeprom((void *)ee_lst);
    p_eeprom = dat_read_eeprom((void *)ee_data);

  }
  else{
    set_irc_k(DEFAULT_irc_k);
    set_irc_set_on_value(DEFAULT_irc_set_on_value);
    dat_free_m_jmena();
  }
}

void eeprom_save(void){
  void * p_eeprom;
  
  wtch_reset2();
  wtch_disable2();
  eeprom_w32((void *)ee_irc_counter, irc_counter);
  eeprom_w8((void *)ee_width_src,width_src);
  eeprom_w8((void *)ee_flag,63);
  uloz_posledni_data();
  //lst_write_eeprom((void *) ee_lst);
  p_eeprom = dat_zak_write_eeprom((void *)ee_lst);
  p_eeprom = dat_write_eeprom((void *)ee_data);

  wtch_enable2();
  wtch_reset2();
}

//8 bit
void eeprom_w8(void *uiAddress, uint8_t c){
  //cekame dokud neni volno
  wtch_reset2();
  eeprom_busy_wait();
  eeprom_write_byte(uiAddress, c);
  
}

char eeprom_r8(void *uiAddress){
  return eeprom_read_byte(uiAddress);
}

//16bit
void eeprom_w16(void *uiAddress, uint16_t word){
  //cekame dokud neni volno
  wtch_reset2();
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
  wtch_reset2();
  uint32_t out = 0;
  eeprom_read_block( (void *)&out, uiAddress, 4);
  
  return out;
}



/* end of eeprom.c */
