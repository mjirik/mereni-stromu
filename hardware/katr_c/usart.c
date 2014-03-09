/*
 * $Id: usart.c 122 2007-09-21 06:18:10Z Mira $
 * File name: usart.c
 * Date:      2006/01/21 17:07
 * Author:    Mira
 */

/*
 * Soupis zprav
 */


#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <stdio.h>
#include <stdint.h>

#include "usart_recv.h"



/* Velikost dat */
#define DATASIZE 10
/* Délka zprávy */
#define LenOfMsg DATASIZE + 1
/* Délka pøijímacího bufferu */
#define LenOfRecvBuff 2 + DATASIZE + 1 + (DATASIZE / 2)  
/* Délka bufferu pro odesílání */
#define LenOfTransBuff 2 + DATASIZE + 1 + (DATASIZE / 2)
// uvodni byte
// popis dat
// data[0] 
// data[1]
// data[2]
// ..
// data[n]
// xor 0 1 
// ..
// xor n/2 (n/2)+1
//
// pocet dat + popis dat udava LenOfMsg
// Pozor! kdyz to zmenim, je nutno prepsat funkci to_transmitt_buffer()

/* Délka seznamu ještì neodeslaných zpráv */
#define LenOfMsgBuffer 5
#define Delay 500

#define set_bit(c , bit) (c | (1 << bit))
#define clear_bit(c , bit) (c & (254 << bit))

//int pocitadlo = 0; //pocitadlo pro testy. Odstranit!
int usart_recv_count = -1;
char usart_recv_buff[LenOfRecvBuff];
char usart_trans_buff[LenOfTransBuff];

// pokud je usart_trans_buff = -1 znamena to, ze je buffer prazdny
int usart_trans_count = -1;

// pole udrzuje seznam zprav k odeslani
char * msg_buffer[LenOfMsgBuffer];
int msg_buffer_counter = -1;

// buffer pro prijatou zpravu
char recived_msg[LenOfMsg];

// odmlka po odeslání
int after_trans_delay_counter = Delay ;

/* pøíznak toho, zda mùžeme odesílat */
char vysilame = 1;

// Deklarace funkci
void USART_Init( unsigned int baud );
void zarad_tri_zpravy(void);
void send_msg (void);
void new_msg (char *);
int test_message(char *msg);
void process_recived_msg(void);
void odesilame(char c);


/**
 * Inicializace usartu
 */
void USART_Init( unsigned int baud ){
  int i = 0;

  DDRD = set_bit(DDRD,3); // nastaveni vystupniho pinu pro rizeni  

  /* Set baud rate */
  UBRRH = (unsigned char)(baud>>8);
  UBRRL = (unsigned char)baud;
  /* Enable Receiver and Transmitter */
  /* Data,RX interrupt enable*/
  UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE); // |(1<<UDRIE)
  /* Set frame format: 8data, 2stop bit */
  UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);

  // Clear usart recive buffer
  for (i = 0; i < LenOfRecvBuff; i++){
    usart_recv_buff[i] = 0;
  }

  // Clear usart transmitt buffer
  for (i = 0; i < LenOfTransBuff; i++){
    usart_trans_buff[i] = 0;
  }
  
  // Clear usart transmitt buffer level 2
  /*
  for (i = 0; i < LenOfTransBuff; i++){
    usart_trans_buff_2[i] = 0;
  }
  */

//  UDR = 0;
}

// Tahle cast se spousti vzdy kdyz prijmeme nejaky byte po seriovym kanale
SIGNAL(SIG_UART_RECV){
  char prijmuty;

  prijmuty = UDR;
  
  if (usart_recv_count == -1){
    if (prijmuty == 255){
      usart_recv_buff[0] = 255;
      usart_recv_count = 1;
    }
    else{
      // error 
      // chyba prenosu - zahajeno nespravnym znakem
      new_msg("\002rse2");
    }
  }
  else{
    usart_recv_buff[usart_recv_count] = prijmuty;
    usart_recv_count++;

    //kdyz jsou prijaty vsechny byty...
    if (usart_recv_count == LenOfRecvBuff){
      // test validity zpravy
      if (test_message(usart_recv_buff) == 1){
        // zpracovani prijate zpravy
        
        process_recived_msg();
      }
      else {
        // chyba prenosu - neshoda v kontrolnim souctu
        new_msg("\001rse1");
      }
      usart_recv_count = -1;
    }
    
  }
  
  
  //zarad_tri_zpravy();
}


// Tohle se spousti kdyz je prazdnej odesilaci buffer
// Kdyz chce nekdo neco vysilat, staci, kdyz nastavi usart_trans_count = 0;
// odeslíá to po bytech vìci z usart_trans_bufferu az do konce
// na konci je malé odmlèení, kvùli lepší synchronizaci
SIGNAL(SIG_UART_DATA){
  // Pokud neni co odesilat, tak se mrnkni jestli neco nema send_msg
  if (usart_trans_count == -1){
    send_msg();
    
  }
  // pokud je neco v bufferu, tak makej
  if (usart_trans_count != -1){
    PORTD = set_bit(PORTD, 3); // pøipojení odesílací linky

    if (usart_trans_count != LenOfTransBuff){
      UDR = usart_trans_buff[usart_trans_count];
    
      usart_trans_count++;
    }
    
    // tak takhle to fungovalo
    /*
    if (usart_trans_count == LenOfTransBuff){
      usart_trans_count = -1;
    }
    */
    //az sem
    
    // teï se po každým odeslání udìlá pauza
    // dela se to tak, že se vždycky sníží counter a když je nula, tak se vše
    // nastaví a ukonèí
    //*
    if (usart_trans_count == LenOfTransBuff){
      if (after_trans_delay_counter != 0){
        after_trans_delay_counter--;
      

      }
      else{
        after_trans_delay_counter = Delay;
        usart_trans_count = -1;
      }
    }
  }
  else{
      PORTD = clear_bit(PORTD,3); // odpojení odesílacího kanálu od linky
    UCSRB = UCSRB & (~(1 << UDRIE));
  }

} 
                     


// Odesilani zprav pracuje tak, ze funkce new_msg vytvari a udrzuje seznam
// zprav a funkce send_msg tyto zpravy rozesila. Pouzivji se zde pocitadla.
// msg_buffer_counter ukazuje na zpravu, kterou je treba odeslat. Je-li -1, pak
// neni zadne zprava k odeslani.

// Tato funkce obaluje zpravu kodem pouzivanym pro prenos po seriovem kanale.
void to_transmitt_buffer(char *msg){
  int i = 0;
  
  usart_trans_buff[0] = 255;
  
  for (i = 0; i < LenOfMsg; i++){
    usart_trans_buff[i + 1] = msg[i];
  }

  usart_trans_buff[LenOfMsg + 1] = usart_trans_buff[0] ^ msg[0];  

  for(i=0; i < DATASIZE; i = i + 2){
      usart_trans_buff[DATASIZE + 3 + (i/2)] = msg[i+1]^msg[i+2];
  }


//  usart_trans_buff[LenOfMsg + 1 + 1] = msg[1] ^ msg[2];
//  usart_trans_buff[LenOfMsg + 1 + 2] = msg[3] ^ msg[4];
}

// Funkce tesuje spravnost zpravy
// Pokud je zprava v poradku, vrati 1
// pokud je ve zprave chyba, vrati funkce 0
int test_message(char *msg){
  int i=0;

  if (msg[0] != 255){
    return 0;
  }
//  printf("zacatek ok!\n");
  for(i=0;i < DATASIZE;i=i+2){
    unsigned char a,b,c,d;
    a = msg[i+2];
    b = msg[i+3];
    c = 2 + DATASIZE + 1 + (i/2);
    d = msg[c];
    //printf("%i ^ %i = %i, compare with buffer[%i] = %i\n",a,b,a^b,c,d);
    if ((a^b) != d){
	//printf("fuckoff");
      return 0;
    }
  }
  return 1;
}



// vstupem do teto funkce je ukazatel na zpravu
// pokud se ukazatel uz v seznamu nenachazi, tak je zarazen na konec seznamu.
void new_msg (char *msg){
  if (vysilame > 0){

    // pokud zadna zprava v seznamu neni, tak ji strc na nulty misto a nastartuj
    // citac a zapni preruseni od datovaho bufferu.
    if (msg_buffer_counter == -1){
      msg_buffer_counter = 0;
      msg_buffer[msg_buffer_counter] = msg;
      UCSRB = UCSRB | (1 << UDRIE);
    
    }
    // pokud jsou v seznamu zpravy, strc ji na konec.
    else{
      int i = 0;

      // testujeme, zda uz zprava neni v seznamu
      for (i = 0; i < LenOfMsgBuffer; i++){
        if (msg_buffer[i][0] == msg[0]){
          return;
        }
        
      }
    
      // nastavime pocitadlo na prvek za aktualnim prvkem
      if (msg_buffer_counter != (LenOfMsgBuffer - 1))
        i = msg_buffer_counter + 1;
      else
        i = 0;
    
      while(i != msg_buffer_counter){
        // kdyz najdeme ukazatel NULL, tak vyskocime z cyklu
        if (msg_buffer[i] == NULL)
          break;

        // jesli nejsme na konci, inkrementujem, jinak na nulu.
        if (i != (LenOfMsgBuffer - 1)) 
          i++;
        else 
          i = 0;
      }
    
      if (i != msg_buffer_counter){
        msg_buffer[i] = msg;
      }
      else{
        msg_buffer[i] = "\007msgf";
        //psat error preplneni zasobniku zprav
      }
    
    }
  }
  
}

// Funkce po svem zavolani projde seznam zprav. Jestli je nejaka zprava k
// odeslani, zkopiruje ji do odesilaciho bufferu a nastavi usart_trans_count=0;
// Tim se zahaji odesilani.
void send_msg (void){
  if (msg_buffer_counter != -1){
    char *poiter_to_msg;

    // zkopirujeme obsah do bufferu
    poiter_to_msg = msg_buffer[msg_buffer_counter];

    to_transmitt_buffer(poiter_to_msg);

    /*
    for (i = 0; i < LenOfTransBuff; i++){
      usart_trans_buff[i] = poiter_to_msg[i];
    }
    */
    // 
    msg_buffer[msg_buffer_counter] = NULL;

    // zahajime odesilani
    usart_trans_count = 0;

    // Pokusime se nalezt dalsi zpravu
    if (msg_buffer_counter == (LenOfMsgBuffer - 1)){
      msg_buffer_counter = 0;
    }
    else{
      msg_buffer_counter++;
    }

    // Pokud zprava neexistuje, nastavime pocitadlo na -1;
    if (msg_buffer[msg_buffer_counter] == NULL){
      msg_buffer_counter = -1; 
      // jestli uz je vsechno odeslany zakazeme preruseni od prazdneho datoveho
      // bufferu. Stejne tam ted nic neprijde.  Az budeme preruseni potrebovat,
      // funkce new_msg ho zase zapne.
      // nejak to nefunguje
      /*
      if (usart_trans_count == -1){
        UCSRB = UCSRB & (~(1 << UDRIE));
      }
      */
    }
  }
  
}

// V tele teto funkce je reseno zpracovani prijate zpravy.
void process_recived_msg(){
  int i = 0;
  
  //zkopirovani zpravy a tim uvoneni prijimaciho bufferu
  for (i = 0; i < LenOfMsg; i++){
    recived_msg[i] = usart_recv_buff[i + 1];
  }
  recv_msg(recived_msg);
}

void odesilame(char c){
  vysilame = c;
}
/* end of usart.c */

