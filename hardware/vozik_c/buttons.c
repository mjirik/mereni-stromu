/* $Id: buttons.c 117 2007-09-18 21:53:25Z Mira $ */
/**
 * @file buttons.c
 * @brief
 * Modul zabezpe�uje funkci tla��tek pro ovl�d�n� z�kladn�ch funkc�.
 * 
 */
#include <avr/io.h>
#include "buttons.h"
#include "keyboard.h"
static char minuly_stav = 0;


static void button_on(char button);

void buttons_init(void){
  //Nastaven� pull-up
  // Ve skute�nosti bude PORTC == 0xff, proto�e na prvn�m pinu je
  // kl�vesnice, kter� si jej taky nastav�.
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
    { //do�lo-li ke zm�n� na p�slu�n�m pinu
      if (((minuly_stav >> i) & 1) == 1)
      {  //sepnut�
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
 * Funkce je spou�t�na p�i stisku tla��tka (n�b�n� hrana).
 * Zde je m�sto pro implementaci p��padn�ho filtru z�kmit�.
 * Pro ka�d� konkr�tn� tla��tko je zde nabindov�na kl�vesa kl�vesnice.
 * Je to provedeno vol�n�m funkce keyb_in_buff(), kter� p�id� do bufferu
 * kl�vesnice k�d libovoln� kl�vesy.
 * 
 * @param button
 * Ud�v� kter� tla��tko bylo stisknuto.
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
