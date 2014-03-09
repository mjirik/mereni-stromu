/*
 * File name: width.c
 * Date:      2006/01/21 12:12
 * Author:    Mira
 */
// Inkludy
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <stdio.h>

#include "usart.h"
#include "main.h"
#include "irc.h"

// definice
#define LenOfWidIntegrArr 80
#define widthIterLimit 100
#define widthIterComparator 50
#define SizeOfMsg 5
//kazdy tolikatý pruchod dojde k naètení osmice bitù
#define widthSlow 5

// parametry katru v mm
#define vzdalSvetlaOdSnimacu 2000
#define vzdalSnimacuOdStromu 50

// deklarace promenych
//pole v nemz se scitaji hodnoty z mereni sirky
int width_integrate_array[LenOfWidIntegrArr];
// vystup multiplexu
char width_multiplex_adress = 0;
// pocitdlo sescitani hodnot v integracnim poli
int width_counter = 0;
// zmerena hodnota a jeji platnost
int width_valid = 0;
char width_value = 0;
extern char data_msg[SizeOfMsg];
//kazdy tolikatý pruchod dojde k naètení osmice bitù
int width_slow_counter = widthSlow;
// prepocet - urèuje jestli bude pøed odesíláním docházet k pøepoètu 
//  namìøené šíøky na skuteènou
char width_prepocet = 1;

//char * w_slij (char soucasny, char pocet, char nula2jedna);

// deklarace funkci
void width_watch(void);
void width_watch_init(void);
void width_computation(void);
void width_send_msg(void);

/* Sledovani sirky. Funkce zajistuje multiplexovani snimacu a zpracovani
 * signalu. Je volána cyklicky z funkce main()
 * Sebere vzdy 8 bitù najednou.
 * Nedelá to však s každým prùchodem, ale každý widthSlow-tý prùchod
 * */
void width_watch(void){
  unsigned char vstup0;
  unsigned char vstup1;
  int i = 0;

  //tenhle if zajistí, že se ètení z multiplexù nekoná každé zavolání, každé widthSlow-té
  if (width_slow_counter == 0){
  
    vstup0 = PINB;
    vstup1 = (PIND >> 6); // to jsou dva piny z D

    // pri každém prùchodu se zaznamená osmice bitù
    for (i = 0 ; i < 8 ; i++){
      width_integrate_array[(i*8) + width_multiplex_adress] += (vstup0 >> i) & 0x01;
    }
    width_integrate_array[8*8 + width_multiplex_adress] += (vstup1 >> 0) & 0x01;
    width_integrate_array[9*8 + width_multiplex_adress] += (vstup1 >> 1) & 0x01;
    
      
    if (width_multiplex_adress < 7){
      width_multiplex_adress++;
    }
    else{
      width_multiplex_adress = 0;
      if (width_counter < widthIterLimit){
        width_counter++;
      }
      else{
        width_counter = 0;
        width_computation();
      }
    }
    // vypis adresu multiplexu na prvni tri bity portu C
    PORTC = ((PORTC & 0xf8) | (width_multiplex_adress & 0x07));


    width_slow_counter = widthSlow; 
  }
  else{
    width_slow_counter--;
      
  }
  
}


// Inicializace mereni sirky
// Vyuluje se zde pole pro nascitavani zmerenych hodnot.
void width_watch_init(void) {
  int i = 0;
  for (i = 0; i < LenOfWidIntegrArr ; i++){
    width_integrate_array[i] = 0;
  }
  // vypis adresu multiplexu na prvni tri bity portu C
  DDRC |= 0x07; 
  PORTC = ((PORTC & 0xf8) | (width_multiplex_adress & 0x07));
  PORTB = 0xff;
  PORTD |= (1 << 6)|(1 << 7);
}


/* Funkce je volána v tìle funkce width_watch
 * Je zde implementováno erodování osamìlých jednièek a nastavení nul obklopených jednièkami na jedna
 */
void width_computation(void){
  int i = 0;
  char pocet = 0;
  char ppppredchozi = 0;
  char pppredchozi = 0;
  char ppredchozi = 0; //predpredchozi
  char predchozi = 0;
  char nula2jedna = 0;
  
  for (i = 0; i < LenOfWidIntegrArr; i++){
    //if (width_integrate_array[i] > widthIterLimit){ // zda se mi že je tam chyba 
    if (width_integrate_array[i] > widthIterComparator){
      pocet++;
      
      // tady se pocita pocet prechodu z nuly na jednicku
      if (predchozi == 0){
        nula2jedna++;
      }
      // vyraz na slivani
      // pokud je nula mezi jednièkama, hodí to nulu na jednièku
      if ((predchozi == 0)&(ppredchozi == 1)){
        nula2jedna--;
        pocet++;
        predchozi = 1;
      }
      ppppredchozi = pppredchozi;
      pppredchozi = ppredchozi;
      ppredchozi = predchozi;
      //konec slivani

      predchozi = 1;
      
    }
    else{
      //slivani
      // jesli je za jednou jednickou a pred jendickou 
      // (to plyne z toho, že to neopravil jednièkovej slívací výraz) dve nuly,
      // z jednièky udìlej nulu
      /*
      if ((predchozi == 0)&(ppredchozi == 1)&(pppredchozi == 0)){
        ppredchozi = 0;
        pocet--;
      }*/
      ppppredchozi = pppredchozi;
      pppredchozi = ppredchozi;
      ppredchozi = predchozi;
      //konec slivani
      predchozi = 0;
    }

    width_integrate_array[i] = 0;

    
  }
  
  if (nula2jedna < 2){
    width_value = pocet;
    width_valid = 1;
    width_send_msg();

  }
  else {
    width_value = 0;
    width_valid = 0;
    // chyba mereni sirky - vice ploch
    //new_msg("\003width");
    //
    //lazeni
    width_value = pocet;
    width_send_msg();

  }
}


uint16_t get_d_h(void){
  float r1;
  uint16_t w;

  w = width_value * 10; // v mm

  if (width_prepocet != 0){
    r1 = (irc_get_d()) / 2.0; //dva jsou aby to bylp polomìèr a né prùmìr

    w = (uint16_t)((w / ((float) vzdalSvetlaOdSnimacu)) * ((float)vzdalSvetlaOdSnimacu - r1 - vzdalSnimacuOdStromu));
  } 
  return w;
}

void width_send_msg(void){
  uint16_t w;
  char ch1,ch2;


  //if (width_value >=0){
    //w = width_value * 10; // v mm
  /*}
  else{
    w = 0;
  }*/

  w = get_d_h();
    
  ch1 = w % 256;
  ch2 = w / 256;
    // odeslima pouze pokud doslo ke zmene
  if ((data_msg[3] != ch1) || (data_msg[4] != ch2)){ 
    data_msg[3] = ch1;
    data_msg[4] = ch2;
    new_msg(data_msg);

  }
}
/* end of width.c */
