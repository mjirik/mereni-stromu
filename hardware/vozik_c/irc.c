/* $Id: irc.c 71 2007-08-11 22:08:59Z Mira $
 * 
 * File name: irc.c
 * Date:      2006/01/21 17:27
 * Author:    Mira
 */

/**
 * @file irc.c
 * @brief
 * Modul irc.c øeøí obsluhu inkrementálího èidla. Veøejná funkce 
 * irc_watch() sleduje vsupy irc a obsluhuje èítaè irc. Výstupem je
 * seriový kanál. Po nìm se odesílá hodnota irc pøenásobená konstantou.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdint.h>
//#include "main.h"
#include "usart.h"
#include "irc.h"
#include "eeprom.h"
#include "error.h"

//#define IrcSetOnValue 10
#define SizeOfMsg 5


//deklarace promennych
unsigned char prev_state; ///< Hodnota kanálù A a B pøi minulém volání funkce irc_watch().
/// poèítadlo krokù
int32_t irc_counter = 0;
/// Hodnota pro nastavení pøi pøejezdu pøes nastavovací snímaè
int32_t irc_set_on_value = 100;

/// Poslední operace provedena s irc poèítadlem
/// 0 - bylo naposled odeèítáno, 1 - pøièítáno
/// vlastnì to vyjadøuje smìr pohybu
unsigned char last_oper_with_irc_counter = 0; 

// Toto je pole pro odesílanou zprávu. Používá se pøi odesílání pøes sériový
// kanál. Je definované zvenku.
// extern char data_msg[SizeOfMsg];

// Zde je zpráva o rozdílu mezi hodnotou a nastavením.
//char irc_dif_msg[SizeOfMsg];

/// irc_k konstanta, kterou se násobí irc_counter pøed odesláním. Tim se pøevádí
/// hodnota na použitelné jednotky.
uint16_t irc_k = 100;

uint16_t irc_get_k(void);

/// Výstup je v setinách milimetru. Je tøeba jej vydìlit 1000 aby byl v cm.
uint16_t irc_get_l(void);


// deklarace funkci
void irc_watch(void);
void irc_send_msg(void);
void set_irc_k(uint16_t i);
void set_irc_set_on_value(int32_t q);


/// Funkce vypoète a nastaví pøenásobovací konstantu irc
/// Vstupem je èíslo, které odpodvídá kolik milimetrù je na jednu otáèku.
void set_irc_k (uint16_t i) {

  //10000/480 = 125/6, 120 pulzù * 4 hrany  = 480
  // irc_k = (i * 125) / (60); // 125/60 = 25/12 
  irc_k = (i * 25) / (12);
  eeprom_w16((void *)ee_irc_k,irc_k);
}

uint16_t irc_get_k(void){
  return irc_k;
}

/// Nastavení hodnoty.
/// Vstupem je vzdálenost nastavovacíhoo bodu od poèátku v centimetrech vynásobená deseti.
/// tj 65 cm je 650
void set_irc_set_on_value(int32_t q){
  // je tu jen 1000 místo 10000. Je to protože ta vstupní hodnota je pøenásobená deseti, tak je
  // tøeba jí vydìlit
  irc_set_on_value = (int32_t)(q * (1000.0 /(irc_k)));
  eeprom_w32((void *)ee_irc_k,irc_set_on_value);
}

/**
 * Inicializace IRC.
 */
void irc_init(void){
  PORTD = PORTD | (1 << 5) | (1 << 6) | (1 << 7);
  // aby nedocházelo k vyhlášení erroru pøi prvním zavolání funkce irc_watch
  prev_state = ((PIND) >> 5) & 0x07; 
}

/// V teto funkci je reseno sledovani vstupu irc a jeho nastaveni. Vzdy kdyz je
/// funkce zavolana, je zkontrolovano, zda nedoslo okd minuleho zavolani ke
/// zmene na vstupech irc. Pokud ano, jsou provedeny prislusne korky - zvyseni
/// nebo snizeni citace irc a pripadne odeslani namerene hodnoty. Namerena
/// hodnota je dana jako hodnota irc cidla krat
/// konstanta (obvykle mensi nez jedna). 
void irc_watch(void){
  unsigned char stav;
  unsigned char zmena_stavu;
  unsigned char ruzne;

  // vstupy jsou invertovany
  //stav = ((~PINC) >> 3) & 0x07;
  stav = ((PIND) >> 5) & 0x07;
  zmena_stavu = stav ^ prev_state;
  ruzne = (((stav >> 0) ^ (stav >> 1)) & 0x01);

  // zde se resi pricitani a odecitani pri zmenach vstupu A a B Jestlize dojde
  // ke zmene nektereho vstupu, pak se porovna zda jsou oba bity stejne, ci ne
  // a podle toho se citac bud inkrementuje nebo dekrementuje.
  // Rovnez je zde resen erroru pri soucasne zmene obou vstupu.
  switch ((zmena_stavu >> 0) & 0x03){
    case 1 : 
      // zmenaA
      if (ruzne == 0){
        irc_counter++;
        last_oper_with_irc_counter = 1;
        //irc_send_msg();
      }
      else{
        irc_counter--;
        last_oper_with_irc_counter = 0;
        //irc_send_msg();
      }
      break;
    case 2 :
      // zmenaB
      if (ruzne == 0){
        irc_counter--;
        last_oper_with_irc_counter = 0;
        //irc_send_msg();
      }
      else{
        irc_counter++;
        last_oper_with_irc_counter = 1;
        //irc_send_msg();
      }
      break;
    case 3 :
      // oba
      // error
      new_error(6);
      break;
    default : 
      break;

  }

  // zde je reseno nastavovani irc citace
  // pokud se objevi jednicka na pinc.6 a pohybujeme se nahoru (nula je tam
  // pri obraceni vstupu)
  // 
  
  if ((((zmena_stavu >> 2) & 0x01 ) == 1) && (last_oper_with_irc_counter == 1) && (((stav >> 2 ) & 1) == 0)){
    int dif = 0;

    dif = irc_set_on_value - irc_counter;
    if (dif < 0)
      dif = -1 * dif;
    
//    dif = dif * 0.0001 * irc_k;
 
//    irc_dif_msg[0] = 5;
//    irc_dif_msg[1] = dif;
//    irc_dif_msg[2] = 0;
//    irc_dif_msg[3] = 0;
//    irc_dif_msg[4] = 0;

    irc_counter = irc_set_on_value;


    //irc_send_msg();
    // tady by šlo tršku nahnat
    if ((dif * 0.0001 * irc_k) > 10){
      new_error(83);
    }
    
  }
  

    
  prev_state = stav;
  
}




uint16_t irc_get_l(void){
  uint16_t out;
  int32_t count;
  
  if (irc_counter >= 0){
    count = (int32_t) irc_counter;
  }
  else {
    count = 0;
  }

  //out = (uint16_t) (count * (0.0001 * irc_k));

  out = count * 0.0001 * irc_k;
  return out;

}

/* end of irc.c */
