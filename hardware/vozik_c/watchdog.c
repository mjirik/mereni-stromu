// $Id: watchdog.c 93 2007-08-19 01:40:58Z Mira $
/**
 * @file watchdog.c
 * @brief
 * Modul zaji��uje spr�vnou funkci watchdogu.
 * Pro spr�vnou funkci za��zen� nen� mon�n� vyu��vat samotn� watchdog
 * naimplementovan� v�robcem. S resetem by toti� do�lo ke ztr�t�
 * dat. Proto byl vytvo�en programov� watchdog.
 * Ten je resetov�n nastaven�m ��ta�e wtch_counter na nulu.
 * S�m o sob� resetovat jedno�ip neum�, na to v�ak lze v�ak vyu��t
 * slu�by watchdogu od Atmelu. 
 */

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "watchdog.h"
#include "lcd.h"
#include "eeprom.h"

#define MY_WATCHDOG 10000 ///< Kolikr�t 

static char err97[] PROGMEM = "Error 97: Vazna chyba behu programu.    Ukladani dat.";
static char err97b[] PROGMEM = "Error 97: Data ulozena. Reset.";

/// ��ta� m�ho watchdogu. pokud bude v�t�� ne� 200, dojde k v�pisu chybov� hl�ky.
uint16_t wtch_counter = 0;


void wtch_init(void){
  wtch_enable2();
//  wdt_enable(WDTO_2S);
}


/**
 * Funkce sleduje �innost jedno�ipu. Pokud dojde k "zaseknut�" v
 * n�jak� vnit�n� smy�ce, je vyps�n chybov� v�pis, data jsou ulo�ena
 * do eeprom a program je uveden do nekone�n� smy�ky, ve kter� �ek� na
 * reset prost�ednictv�m softwarov�ho watchdogu.
 * Watchdog lze deaktivovat nastaven�m ��ta�e na z�pornou hodnotu.
 */
void wtch_main(void){
  if (wtch_counter < 0){ // Slu�ba je vypnuta

  }
  else if (wtch_counter < MY_WATCHDOG){// v�echno v po��dku
    wtch_counter++;
  }
  else { // Nastal probl�m
    int i;
    lcd_w_inst(0x01);
    printnt_P((void *)err97);

    eeprom_save();

    lcd_w_inst(0x01);
    printnt_P((void *)err97b);
    wtch_reset2();

    for(i=0 ; i < 2000;i++){
      _delay_ms(1);
      wtch_reset2();
    }
    //�ek�n� na reset watchdogu
    for(;;){
      //tohle je resetov�n� softwarov�ho watchodogu.
      wtch_reset();
    };
  }
}
