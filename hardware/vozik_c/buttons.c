/* $Id: buttons.c 148 2009-10-25 13:54:05Z mjirik $ */
/**
 * @file buttons.c
 * @brief
 * Modul zabezpečuje funkci tlačítek pro ovládání základních funkcí.
 * 
 */
#include <avr/io.h>
#include "buttons.h"
#include "keyboard.h"
#include "watchdog.h" // kvuli wtch_dbg_info()
static char minuly_stav = 0;


static void button_on(char button);

void buttons_init(void){
  //Nastavení pull-up
  // Ve skutečnosti bude PORTC == 0xff, protože na prvním pinu je
  // klávesnice, která si jej taky nastaví.
  PORTC |= 0xFE;
  minuly_stav = 254; //PINC
}

void buttons(void) {
  char zmena;
  char vzorek_pinc = PINC;
  int i = 0;
  wtch_dbg_info();


  zmena = minuly_stav^vzorek_pinc;


  for (i = 1; i < 8; i++) {
    if (((zmena >> i) & 1) == 1) { //došlo-li ke změně na přslušném pinu
      if (((minuly_stav >> i) & 1) == 1) { //sepnutí
        button_on((char) i);
      } else {
        // button_off((char)i);
      }
    }
  }
  minuly_stav = vzorek_pinc;
}

/**
 * Funkce je spouštěna při stisku tlačítka (náběžná hrana).
 * Zde je místo pro implementaci případného filtru zákmitů.
 * Pro každé konkrétní tlačítko je zde nabindována klávesa klávesnice.
 * Je to provedeno voláním funkce keyb_in_buff(), která přidá do bufferu
 * klávesnice kód libovolné klávesy.
 * 
 * @param button
 * Udává které tlačítko bylo stisknuto.
 */
void button_on(char button){
  switch(button)
  {
  case 1:
    keyb_in_buff(2,22);
    break;
  case 2:
    keyb_in_buff(2,19);
    break;
  case 3:
    keyb_in_buff(2,21);
    break;
  case 4:
    keyb_in_buff(2,7);
    break;
  case 5:
    keyb_in_buff(2,8);
    break;
  case 6:
    keyb_in_buff(2,4);
    break;
  case 7:
    keyb_in_buff(2,5);
    break;
  default:
    break;
  }
}
