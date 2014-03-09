/* $Id: buttons.c 117 2007-09-18 21:53:25Z Mira $ */
/**
 * @file buttons.c
 * @brief
 * Modul zabezpeèuje funkci tlaèítek pro ovládání základních funkcí.
 * 
 */
#include <avr/io.h>
#include "buttons.h"
#include "keyboard.h"
static char minuly_stav = 0;


static void button_on(char button);

void buttons_init(void){
  //Nastavení pull-up
  // Ve skuteènosti bude PORTC == 0xff, protože na prvním pinu je
  // klávesnice, která si jej taky nastaví.
  PORTC |= 0xFE;
  minuly_stav = 254; //PINC
}

void buttons(void){
  char zmena;
  char vzorek_pinc = PINC;
  int i = 0;



  zmena = minuly_stav^vzorek_pinc;


  for (i = 1; i < 8; i++)
  {
    if (((zmena >> i) & 1 ) == 1)
    { //došlo-li ke zmìnì na pøslušném pinu
      if (((minuly_stav >> i) & 1) == 1)
      {  //sepnutí
        button_on((char)i);
      }
      else
      {
        // button_off((char)i);
      }
    }
  }
  minuly_stav = vzorek_pinc;
}

/**
 * Funkce je spouštìna pøi stisku tlaèítka (nábìžná hrana).
 * Zde je místo pro implementaci pøípadného filtru zákmitù.
 * Pro každé konkrétní tlaèítko je zde nabindována klávesa klávesnice.
 * Je to provedeno voláním funkce keyb_in_buff(), která pøidá do bufferu
 * klávesnice kód libovolné klávesy.
 * 
 * @param button
 * Udává které tlaèítko bylo stisknuto.
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
