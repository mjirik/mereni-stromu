// $Id: watchdog.c 93 2007-08-19 01:40:58Z Mira $
/**
 * @file watchdog.c
 * @brief
 * Modul zajišuje správnou funkci watchdogu.
 * Pro správnou funkci zaøízení není monné vyuívat samotnı watchdog
 * naimplementovanı vırobcem. S resetem by toti došlo ke ztrátì
 * dat. Proto byl vytvoøen programovı watchdog.
 * Ten je resetován nastavením èítaèe wtch_counter na nulu.
 * Sám o sobì resetovat jednoèip neumí, na to však lze však vyuít
 * sluby watchdogu od Atmelu. 
 */

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "watchdog.h"
#include "lcd.h"
#include "eeprom.h"

#define MY_WATCHDOG 10000 ///< Kolikrát 

static char err97[] PROGMEM = "Error 97: Vazna chyba behu programu.    Ukladani dat.";
static char err97b[] PROGMEM = "Error 97: Data ulozena. Reset.";

/// Èítaè mého watchdogu. pokud bude vìtší ne 200, dojde k vıpisu chybové hlášky.
uint16_t wtch_counter = 0;


void wtch_init(void){
  wtch_enable2();
//  wdt_enable(WDTO_2S);
}


/**
 * Funkce sleduje èinnost jednoèipu. Pokud dojde k "zaseknutí" v
 * nìjaké vnitøní smyèce, je vypsán chybovı vıpis, data jsou uloena
 * do eeprom a program je uveden do nekoneèné smyèky, ve které èeká na
 * reset prostøednictvím softwarového watchdogu.
 * Watchdog lze deaktivovat nastavením èítaèe na zápornou hodnotu.
 */
void wtch_main(void){
  if (wtch_counter < 0){ // Sluba je vypnuta

  }
  else if (wtch_counter < MY_WATCHDOG){// všechno v poøádku
    wtch_counter++;
  }
  else { // Nastal problém
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
    //èekání na reset watchdogu
    for(;;){
      //tohle je resetování softwarového watchodogu.
      wtch_reset();
    };
  }
}
