/* $Id: irc.c 69 2007-08-04 14:24:09Z Mira $ */
/**
 * @file irc.c
 * @brief
 * Modul irc.c resi obsluhu inkrementalnaho cidla. Verejna funkce je jen jedna
 * - irc_watch(). Ta sleduje vsupy irc a obsluhuje citac irc. Vystupem je
 *   seriovy kanal. Po nem se odesila hodnota irc prenasobena konstantou.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "usart.h"
#include "eeprom.h"
#include "width.h"
#include "irc.h"

//#define IrcSetOnValue 10
#define SizeOfMsg 10

//deklarace promennych
unsigned char prev_state = 0;
// po��tadlo krok�
int32_t irc_counter = 0;
// Hodnota pro nastaven� p�i p�ejezdu p�es nastavovac� sn�ma�
int32_t irc_set_on_value = 100;

// posledni operace provedena s irc pocitadlem
// 0 - bylo naposled odecitano, 1 - pricitano
// vlastne to vyjadruje smer pohybu
unsigned char last_oper_with_irc_counter = 0; 

//toto je pole pro odesilanou zpravu. Pouziva se pri odesilani pres seriovy
//kanal. Je definovane zvenku.
extern char data_msg[SizeOfMsg];

// zde je zprava o rozdilu mezi hodnotou a nastavenim
char irc_dif_msg[SizeOfMsg];

//irc_k konstanta kterou se nasobi irc_counter pred odeslanim. Tim se pravdi
//hodnota na pouzitelne jednotky.
uint16_t irc_k;

uint16_t irc_get_k(void);

//v�stup je v setin�ch milimetru. Je t�eba jej vyd�lit 1000 aby byl v cm
uint16_t irc_get_d(void);


// deklarace funkci
/* void irc_watch(void); */
/* void irc_send_msg(void); */
/* void set_irc_k(uint16_t i); */
/* void set_irc_set_on_value(int32_t q); */

void irc_init(void){
  PORTC |= (1 << 3);
  // aby nedoch�zelo k vytv��en� erroru p�i prvn�m vol�n� irc_watch()
  prev_state = ((PINC) >> 3) & 0x07; 
}

/**
 * Funkce nastav� irc_k tak, aby p�i oto�en� o jednu ot��ku byla nam��en� vzd�lenost 
 * pr�v� 'i'. Jin�mi slovy: vstupem funkce je vzd�lenost v mm, kterou popojede v�lec 
 * p�i oto�en� IRC o jednu ot��ku.
 * 
 */
void set_irc_k (uint16_t i) {
  irc_k = (i * 125) / (6); //10000/480 = 125/6, 120 pulz� * 4 hrany  = 480
  
}

uint16_t irc_get_k(void){
  return irc_k;
}

/// Nastaven� hodnoty offsetu irc, p�i projet� kolem referen�n�ho bodu.
///
/// param q
/// Vstupem je vzd�lenost nastavovac�hoo bodu od po��tku v centimetrech vyn�soben� deseti,
/// tj 65 cm je 650.
void set_irc_set_on_value(int32_t q){
  // je tu jen 1000 m�sto 10000. Je to proto�e ta vstupn� hodnota je p�en�soben� deseti, tak je
  // t�eba j� vyd�lit
  irc_set_on_value = (int32_t)(q * (10000.0 /(irc_k)));
  eeprom_w32((void *)ee_irc_set_on_value, irc_set_on_value);
}

/// V teto funkci je reseno sledovani vstupu irc a jeho nastaveni. Vzdy kdyz je
/// funkce zavolana, je zkontrolovano, zda nedoslo okd minuleho zavolani ke
/// zmene na vstupech irc. Pokud ano, jsou provedeny prislusne korky - zvyseni
/// nebo snizeni citace irc a pripadne odeslani namerene hodnoty. Namerena
/// hodnota je dana jako hodnota irc cidla krat konstanta (obvykle mensi nez
/// jedna). 
void irc_watch(void){
  unsigned char stav;
  unsigned char zmena_stavu;
  unsigned char ruzne;

  // vstupy jsou invertovany
  //stav = ((~PINC) >> 3) & 0x07;
  stav = ((PINC) >> 3) & 0x07;
  zmena_stavu = stav ^ prev_state;
  ruzne = (((stav >> 2) ^ (stav >> 1)) & 0x01);

  // zde se resi pricitani a odecitani pri zmenach vstupu A a B Jestlize dojde
  // ke zmene nektereho vstupu, pak se porovna zda jsou oba bity stejne, ci ne
  // a podle toho se citac bud inkrementuje nebo dekrementuje.
  // Rovnez je zde resen erroru pri soucasne zmene obou vstupu.
  switch ((zmena_stavu >> 1) & 0x03){
    case 1 : 
      // zmenaA
      if (ruzne == 0){
        irc_counter++;
        last_oper_with_irc_counter = 1;
        irc_send_msg(0);
      }
      else{
        irc_counter--;
        last_oper_with_irc_counter = 0;
        irc_send_msg(0);
      }
      break;
    case 2 :
      // zmenaB
      if (ruzne == 0){
        irc_counter--;
        last_oper_with_irc_counter = 0;
        irc_send_msg(0);
      }
      else{
        irc_counter++;
        last_oper_with_irc_counter = 1;
        irc_send_msg(0);
      }
      break;
    case 3 :
      // oba
      // error
      new_msg("\004ircb     ");
      break;
    default : 
      break;

  }

  // zde je reseno nastavovani irc citace
  // pokud se objevi jednicka na pinc.6 a pohybujeme se nahoru (nula je tam
  // pri obraceni vstupu)
  //
  if ((((zmena_stavu ) & 0x01 ) == 1) && (last_oper_with_irc_counter == 1) && (((stav & 0x01) == 1))){
    int dif = 0;

    dif = irc_set_on_value - irc_counter;
    if (dif < 0)
      dif = -1 * dif;

    dif = dif * 0.0001 * irc_k;

    irc_dif_msg[0] = 5;
    irc_dif_msg[1] = dif / 256;
    irc_dif_msg[2] = dif % 256;
    irc_dif_msg[3] = 0;
    irc_dif_msg[4] = 0;

    irc_counter = irc_set_on_value;


    irc_send_msg(1);

    if ((dif * 0.0001 * irc_k) > 10){
      new_msg(irc_dif_msg);
    }
    
  }
  

    
  prev_state = stav;
  
}

/**
 * Tato funkce je vol�na p�i ka�d� zm�n� ��ta�e irc. Zde je realizov�n p�ed
 * odesl�n�m je zde zrealizov�n p�epo�et mezi nas��tan�mi kroky z irc a
 * skute�ou hodnotou. Pokud ji� byla odesl�na zpr�va se stejn�mi daty (a
 * paramer sure je <= nule) nic se neodes�l�.
 * @param sure Je-li hodnota v�t�� ne� nula, je zpr�va odesl�na bez ohledu na to,
 * zda ji� nebyla odesl�na stejn� zpr�va se stejn�mi daty.
 */
void irc_send_msg(char sure){
  unsigned int count;
  char ch1 = 0;
  char ch2 = 0;
  char ch3, ch4;
  uint16_t w;

  //ch = irc_counter * (0.1) * irc_k;
  count = irc_get_d();
  w = get_d_h();

  //rozlo��m si integer na dva chary a ty pak ode�lu
  // v�stup je v setin�ch milimetru. Je t�eba jej vyd�lit 1000 aby byl v cm
  ch1 = (char) count;
  ch2 = (char) (count / 256);
 
  ch3 = (char) (w % 256);
  ch4 = (char) (w / 256);

  // odesilame jen pokud doslo ke zmene v vertikalnim nebo horizontalnim
  if ((ch1 != data_msg[1])|(ch2 != data_msg[2])|(ch3 != data_msg[3])|(ch4 != data_msg[4])|(sure > 0)){
    //data_msg[1] = ch;
    //

    data_msg[1] = ch1;
    data_msg[2] = ch2;
    
    data_msg[3] = ch3;
    data_msg[4] = ch4;
    data_msg[5] = 0;
    data_msg[6] = 0;

    //new_msg("jojoj");
    new_msg(data_msg);
  }
  
}

uint16_t irc_get_d(void){
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
