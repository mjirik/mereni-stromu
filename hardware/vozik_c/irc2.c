/* $Id: irc.c 125 2009-09-13 09:16:22Z mjirik $
 *
 * File name: irc.c
 * Date:      2006/01/21 17:27
 * Author:    Mira
 */

/**
 * @file irc.c
 * @brief
 * Modul irc.c řeří obsluhu inkrementálího čidla. Veřejná funkce
 * irc_watch() sleduje vsupy irc a obsluhuje čítač irc. Výstupem je
 * seriový kanál. Po něm se odesílá hodnota irc přenásobená konstantou.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdint.h>
//#include "main.h"
#include "usart.h"
#include "irc2.h"
#include "eeprom.h"
#include "error.h"

#include "lcd.h"
#include "sysinfo.h"
#include "memory.h"
//debug
#include "lcd.h"
#include "watchdog.h"

//global
void irc2_watch(void);
void set_irc2_k_mm(uint16_t);
uint16_t irc2_get_l(void);
void set_irc2_set_on_value_mm(int32_t q);
uint16_t irc2_get_k(void);
void irc2_init(void);

int32_t get_irc2_set_on_value(void);

void set_irc2_counter (int32_t a);
int32_t get_irc2_counter(void);
void set_irc2_k(uint16_t a);
void set_irc2_set_on_value(int32_t a );
//global


//#define IrcSetOnValue 10
//#define SizeOfMsg 5

/// Definice pinů pro vstup A, B a setpoint
///{

#define A_PORT PORTB
#define A_PIN PINB
#define A_BIT 3

#define B_PORT PORTB
#define B_PIN PINB
#define B_BIT 4

#define SP_PORT PORTD
#define SP_PIN PINB
#define SP_BIT 5

///}

//deklarace promennych
static unsigned char prev_state; ///< Hodnota kanálů A a B při minulém volání funkce irc_watch().
/// počítadlo kroků
static int32_t irc_counter = 0;
/// Hodnota pro nastavení při přejezdu přes nastavovací snímač
static int32_t irc_set_on_value = 100;

/// Poslední operace provedena s irc počítadlem
/// 0 - bylo naposled odečítáno, 1 - přičítáno
/// vlastně to vyjadřuje směr pohybu
static unsigned char last_oper_with_irc_counter = 0;

// Toto je pole pro odesílanou zprávu. Používá se při odesílání přes sériový
// kanál. Je definované zvenku.
// extern char data_msg[SizeOfMsg];

// Zde je zpráva o rozdílu mezi hodnotou a nastavením.
//char irc_dif_msg[SizeOfMsg];

/// irc_k konstanta, kterou se násobí irc_counter před odesláním. Tim se převádí
/// hodnota na použitelné jednotky.
static uint16_t irc_k = 100;

/// Po resetu je proměnná jedna, při prvním nasetování na kontrolním bodě se
/// nastaví na nulu. Používá se pro kontrolu nastavení.
static uint8_t irc2_wait4set = 1;


//uint16_t irc_get_k(void);

/// Výstup je v setinách milimetru. Je třeba jej vydělit 1000 aby byl v cm.
//uint16_t irc_get_l(void);


// deklarace funkci
//void irc_watch(void);
//void irc_send_msg(void);
//void set_irc_k_mm(uint16_t i);
//void set_irc_set_on_value_mm(int32_t q);

int32_t get_irc2_set_on_value(){
  return irc_set_on_value;
}

//void set_irc_set_on_value(int32_t a){
//  irc_set_on_value = a;
//}

void set_irc2_counter (int32_t a){
  irc_counter = a;
}

int32_t get_irc2_counter(void){
  return irc_counter;
}

void set_irc2_k(uint16_t a){
  irc_k = a;
}

/// Funkce vypočte a nastaví přenásobovací konstantu irc
/// Vstupem je číslo, které odpodvídá kolik milimetrů je na jednu otáčku.
void set_irc2_k_mm (uint16_t i) {

  //10000/480 = 125/6, 120 pulzů * 4 hrany  = 480
  // irc_k = (i * 125) / (60); // 125/60 = 25/12
  irc_k = (i * 25) / (12);
  eeprom_w16((void *)ee_irc_k,irc_k);
}

uint16_t irc2_get_k(void){
  return irc_k;
}

/// Nastavení hodnoty.
/// Vstupem je vzdálenost nastavovacíhoo bodu od počátku v centimetrech vynásobená deseti.
/// tj 65 cm je 650
void set_irc2_set_on_value_mm(int32_t q){
  // je tu jen 1000 místo 10000. Je to protože ta vstupní hodnota je přenásobená deseti, tak je
  // třeba jí vydělit
  irc_set_on_value = (int32_t)(q * (1000.0 /(irc_k)));
  eeprom_w32((void *)ee_irc_k,irc_set_on_value);
}

void set_irc2_set_on_value(int32_t a ){
  irc_set_on_value = a;
}

/**
 * Inicializace IRC.
 */
void irc2_init(void){
  //unsigned char tmp;

  // nastavení výstupu na jedničky

  // předchozí implementace - funkční
  //PORTD = PORTD | (1 << 5) | (1 << 6) | (1 << 7);

  A_PORT = A_PORT | (1 << A_BIT);
  B_PORT = B_PORT | (1 << B_BIT);
  SP_PORT = SP_PORT | (1 << SP_BIT);


  // aby nedocházelo k vyhlášení erroru při prvním zavolání funkce irc_watch
  // předchozí implementace - funkční
  //prev_state = ((PIND) >> 5) & 0x07;


  prev_state = 0x00;
  prev_state = prev_state | (((A_PIN >> A_BIT) & 0x01) << 0);
  prev_state = prev_state | (((B_PIN >> B_BIT) & 0x01) << 1);
  prev_state = prev_state | (((SP_PIN >> SP_BIT) & 0x01) << 2);

}

/// V teto funkci je reseno sledovani vstupu irc a jeho nastaveni. Vzdy kdyz je
/// funkce zavolana, je zkontrolovano, zda nedoslo okd minuleho zavolani ke
/// zmene na vstupech irc. Pokud ano, jsou provedeny prislusne korky - zvyseni
/// nebo snizeni citace irc a pripadne odeslani namerene hodnoty. Namerena
/// hodnota je dana jako hodnota irc cidla krat
/// konstanta (obvykle mensi nez jedna).
void irc2_watch(void){
  unsigned char stav;
  unsigned char zmena_stavu;
  unsigned char ruzne;

  //wtch_dbg_info();
  //char stavt;

  // vstupy jsou invertovany
  //stav = ((~PINC) >> 3) & 0x07;

  //předchozí implementace - funkční
  //stavt = ((PIND) >> 5) & 0x07;
  

  stav = 0x00;
  stav = stav | (((A_PIN >> A_BIT) & 0x01) << 0);
  stav = stav | (((B_PIN >> B_BIT) & 0x01) << 1);
  stav = stav | (((SP_PIN >> SP_BIT) & 0x01) << 2);


  si_print_dbgirc2(stav);
  //printcxyd(0,0,'0'+ stav);
  //printcxyd(0,1,'0'+ stavt);

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

    // odeslání informace o prvním nasetování
    if (irc2_wait4set > 0){
      char * msg ;
      irc2_wait4set =0;
      msg =(void*) mmalloc (DATASIZE);

      msg[0] = MSGN_IRC2_SETCOMPLETED;
      new_msg(msg);
    }

    irc_counter = irc_set_on_value;


    //irc_send_msg();
    // tady by šlo tršku nahnat
    if ((dif * 0.0001 * irc_k) > 10){
      new_error(83);
    }

  }



  prev_state = stav;

}




uint16_t irc2_get_l(void){
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

