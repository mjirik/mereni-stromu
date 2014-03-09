// $Id: watchdog.c 148 2009-10-25 13:54:05Z mjirik $
/**
 * @file watchdog.c
 * @brief
 * Modul zajišťuje správnou funkci watchdogu.
 * Pro správnou funkci zařízení není monžné využívat samotný watchdog
 * naimplementovaný výrobcem. S resetem by totiž došlo ke ztrátě
 * dat. Proto byl vytvořen programový watchdog.
 * Ten je resetován nastavením čítače wtch_counter na nulu.
 * Sám o sobě resetovat jednočip neumí, na to však lze však využít
 * služby watchdogu od Atmelu. 
 */

#include <stdio.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "watchdog.h"
#include "lcd.h"
#include "eeprom.h"


//global
 char * wtch_sfile = __FILE__;
 int wtch_sline = __LINE__;
 //char * wtch_sfunction;
//global



#define MY_WATCHDOG 5000 ///< Kolikrát

static char err97[] PROGMEM = "Error 97: Vazna chyba behu programu.    Ukladani dat.";
static char err97b[] PROGMEM = "Error 97: Data ulozena. Reset.";

/// Čítač mého watchdogu. pokud bude větší než 200, dojde k výpisu chybové hlášky.
int16_t wtch_counter = 0;


void wtch_init(void){
  wtch_enable2();
//  wdt_enable(WDTO_2S);
}


/**
 * Funkce sleduje činnost jednočipu. Pokud dojde k "zaseknutí" v
 * nějaké vnitřní smyčce, je vypsán chybový výpis, data jsou uložena
 * do eeprom a program je uveden do nekonečné smyčky, ve které čeká na
 * reset prostřednictvím softwarového watchdogu.
 * Watchdog lze deaktivovat nastavením čítače na zápornou hodnotu.
 */
void wtch_main(void){
  if (wtch_counter < 0){ // Služba je vypnuta

  }
  else if (wtch_counter < MY_WATCHDOG){// všechno v pořádku
    wtch_counter++;
  }
  else { // Nastal problém
    int i;
    char str[10];
    lcd_w_inst(0x01);
    printnt_P((void *)err97);

    eeprom_save();

    lcd_w_inst(0x01);
    printnt_P((void *)err97b);
    printnt(" ");
    printnt(wtch_sfile);
    printnt(" ");
    sprintf(str,"%i",wtch_sline);
    printnt(str);
    //printnt(" ");
    //printnt(wtch_sfunction);
    wtch_reset2();

    for(i=0 ; i < 3000;i++){
      _delay_ms(1);
      wtch_reset2();
    }
    //čekání na reset watchdogu
    for(;;){
      //tohle je resetování softwarového watchodogu.
      wtch_reset();
    };
  }
}
