/*
 * File name: main.c
 * Date:      2006/01/19 13:17
 * Author:    Mira
 */

//#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <stdio.h>
#include <util/delay.h>

#include "width.h"
#include "usart.h"
#include "irc.h"
#include "eeprom.h"

#define SizeOfMsg 11
#define F_CPU 8000000UL //8MHz

char data_msg[SizeOfMsg];




// Obsluha inkrement�ln�ho �idla je vol�na v p�eru�en� ��ta�e
SIGNAL (SIG_OUTPUT_COMPARE1A) /* Note [2] */
{
  //cli();
  irc_watch();
  //new_msg("alive");

  //printf("ssss");
  //sei();
}



/* FUnkce p�i sv�m zavol�n� ov���, zda je na p��slu�n�m pinu nap�t� (nap�jen�).
 * Pokud ne, zah�j� se ukl�d�n� dat do eeprom.
 */
void test_shutdown(void){
  if (((PIND >> 2) & (0x01)) == 0){
    
    //
    eeprom_save();
    _delay_ms(10);
    while((((PIND >> 2) & (0x01)) == 0)){
      _delay_ms(10);
    }
  }
}



// Inicializace ��ta�e pro vol�n� inkrement�ln�ho �idla
void ioinit (void) {
  int pocet_cyklu = 400;

  // citac bude citat v ka�d�m cyklu, re�im - Clear Timer on Compare (CTC)
  TCCR1B = (1 << CS10) | (1 << WGM12);
  // nastaveni p�eru�en� p�i porovn�n�
  TIMSK = (1 << OCIE1A);

  // Nastaven� jak �asto se bude volat 
  OCR1AH = (unsigned char)(pocet_cyklu >> 8);
  OCR1AL = (unsigned char) pocet_cyklu;
}



int main(void){
  int i=0;

  //eeprom_w32((void *)0,10);
  //eeprom_w16((void *)4,20);
  //eeprom_w32((void *)6,30);
  
  //po�k�me 100 ms
  for(i=0; i < 100;i++){
  
  _delay_ms(1);
  
  }
  ioinit ();
  data_msg[0] = '\x40';
  irc_init();
  width_watch_init();
  USART_Init(51);
  eeprom_load();
  sei();
  irc_send_msg(1); //odel�e informaci o poloze v�lce

  while(1){
    width_watch();
        
    test_shutdown();
  }
}





// 0.125 us na jeden cyklus (instrukci)
/* end of main.c */
