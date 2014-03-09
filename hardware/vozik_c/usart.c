/* $Id: usart.c 120 2007-09-21 05:43:42Z Mira $
 */
/**
 * @file usart.c
 * @brief
 * Modul zajišuje odesílání zpráv po sériové lince.
 * Funkce new_ack_msg() vytvoøí zprávu, na ní je oèekávána
 * odpovìï. Pøi pøijentí zprávy je zavolána funkce, na ní u
 * ukazuje poínter v parametru funkce. V pøípadì, e je
 * pøekroèena urèitá doba (daná MAX_MSG_ACK_DELAY) je odeslání
 * zprávy zopakováno. Kolikrát toto opakování probìhne ukazuje
 * promìnná MAX_MSG_ACK_REPEATS (celkem je zpráva odeslána
 * MAX_MSG_ACK_REPEATS + 1 krát). Pokud ani na opakované odesílání
 * není obdrena odpovìï, je vyhlášen error.
 *
 * Odesilani zprav pracuje tak, ze funkce new_msg() vytvari a udrzuje seznam
 * zprav a funkce send_msg() je uívána pøi odesílání pro vybírání z tohoto seznamu
 * Pouívají se zde poèítadla.
 * msg_buffer_counter ukazuje na zpravu, kterou je treba odeslat. Je-li -1, pak
 * není ádná zpráva k odeslání.
 *
 * 
 */

/*
 * Soupis zprav
 */


#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <avr/pgmspace.h> 
//#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


#include "usart_recv.h"
#include "error.h"
#include "main.h"
#include "usart.h"
// /// @todo odstranit
// #include "lcd.h"

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
#define LenOfMsgBuffer 5
#define Delay 500

#define MAX_MSG_ACK_DELAY 5000
//#define MAX_MSG_ACK_REPEATS 2

//int pocitadlo = 0; //pocitadlo pro testy. Odstranit!
int usart_recv_count = -1;
char usart_recv_buff[LenOfRecvBuff];
char usart_trans_buff[LenOfTransBuff];
char prepinani = 0;

//* Pokud je usart_trans_buff = -1 znamena to, ze je buffer prazdny
int usart_trans_count = -1;

//* Pole udruje seznam zpráv k odeslaní.
char * msg_buffer[LenOfMsgBuffer];
int msg_buffer_counter = -1;

/// Buffer pro pøijatou zprávu.
char recived_msg[LenOfMsg];

/// Odmlka po odeslání.
int after_trans_delay_counter = Delay ;

/// Èasovaè. Pokud pøeteèe, znamená to, e nedorazila vèas odpovìï
/// na potvrzovanou zprávu.
static unsigned int msg_ack_timer = 0;

/// zde je poslední odeslaná zpráva pro úèel opakování v pøípadì nedoruèení
/// @todo pole by šlo zmenšit na LenOfMsg
static char last_msg_ack[LenOfTransBuff];

///poèet opakování odeslání zprávy
static unsigned char msg_ack_repeats = 0;

/** Ukazatel na funkci, která bude volána po pèijetí potvrzení zprávy */
static void (*p_ack_fcn)(char*s) = NULL;

/// Ukazatel na funkci, kter bude volána v pøípadì, e èekání na
/// odpovìï je neúspìšné.
static void (*p_nack_fcn)(void) = NULL;

// Deklarace funkci
void zarad_tri_zpravy(void);
void send_msg (void);
int test_message(char *msg);
void process_recived_msg(void);

static void reset_buffer(void);
static void into_buffer(unsigned char c);
static void read_port(void);


/**
 * Inicializace usartu
 */
void USART_Init( unsigned int baud ){
  int i = 0;
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
  DDRD = DDRD | (1 << 4); // pøepínání sériovıch linek


}


/// Funkce vynuluje pøijímací buffer
void reset_buffer(void){
  int i=0;
  for (i=0; i < LenOfRecvBuff; i++){
    usart_recv_buff[i]=0;
  }
}


/**
 * Funkce vklÃ¡dÃ¡ data do bufferu.
 * Funkce je zaloená na stejné implementaci jako na serveru.
 */
void into_buffer(unsigned char c){
  int i=0;
  for (i=0;i < LenOfRecvBuff - 1;i++){
    usart_recv_buff[i]=usart_recv_buff[i+1];
  }
  usart_recv_buff[i]=c;
}


/// Nová implementace pøijímní zpráv
/// Není naimplementována chyba z chybnì pøijatéo prvního znaku.
/// @todo Znovuzapojit resetovaè bufferu.
void read_port(){
  static unsigned char byte_counter = 0;
//  unsigned char c;
//  unsigned char typ=0;

  into_buffer(UDR);
  byte_counter++;

  if (test_message(usart_recv_buff) == 1){
    process_recived_msg();
    reset_buffer();
    byte_counter = 0;
//    zpracuj_zpravu();
//dorazilo
  }
  if ((usart_recv_buff[0] == 255) && (byte_counter > LenOfRecvBuff)){
    // chyba prenosu - neshoda v kontrolnim souctu
    new_error(86);
  }
    

}

/// Tahle èást se spouští vdy kdy pøijmeme nìjakı byte po sériovém kanále.
SIGNAL(SIG_UART_RECV){
  read_port();

  
  //zarad_tri_zpravy();
}



/// Tohle se spouští kdy je prázdnı odesílací buffer.
/// Kdy je zapotøebí vysílat, staèí, kdy nastaví usart_trans_count = 0 .
/// Odesílá to po bytech vìci z usart_trans_bufferu az do konce
/// na konci je malé odmlèení, kvùli lepší synchronizaci
SIGNAL(SIG_UART_DATA){
  // Pokud neni co odesilat, tak se mrnkni jestli neco nema send_msg
  if (usart_trans_count == -1){
    send_msg();
    
  }
  // pokud je neco v bufferu, tak makej
  if (usart_trans_count != -1){
    
    if (usart_trans_count != LenOfTransBuff){ // zde je odesílán byte po bytu
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
 

    // teï se po kadım odeslání udìlá pauza
    // dela se to tak, e se vdycky sníí counter a kdy je nula, tak se vše
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
    //*/
    
  }
  else{
    UCSRB = UCSRB & (~(1 << UDRIE));
  }
}




/// Tato funkce obaluje zpravu kodem pouzivanym pro prenos po seriovem kanale.
/// Zde je provedeno uvolnìní pamìti zprávy. Je tak uèinìno po nakopírování
/// zprávy do odesílacího bufferu.
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

  free((void *)msg);
  msg = NULL;
}

/// Funkce tesuje spravnost zpravy
/// @return 
/// Pokud je zprava v poradku vrati 1,
/// pokud je ve zprave chyba vrati funkce 0.
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



/// Vstupem do teto funkce je ukazatel na zpravu
/// pokud se ukazatel uz v seznamu nenachazi, tak je zarazen na konec seznamu.
/// Prostor musí bıt správnì naalokovanı. Po odeslání zprávy je pamì uvolnìna.
void new_msg (char *msg){
//pokud nedochazi k prepinani
  //if (prepinani == 0){
  if (1){

  // pokud zadna zprava v seznamu neni, tak ji strc na nulty misto
  // a nastartuj citac a zapni preruseni od datovaho bufferu.
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
        new_error(96);
        //psat error preplneni zasobniku zprav
      }
    
    }
  }
  
}

/// Funkce po svem zavolani projde seznam zprav. Jestli je nejaka zprava k
/// odeslani, zkopiruje ji do odesilaciho bufferu a nastavi usart_trans_count=0;
/// Tim se zahaji odesilani.
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

/// V tìle této funkce je øešeno zpracování pøijaté zprávy.
void process_recived_msg(){
  int i = 0;
  
  //zkopirovani zpravy a tim uvoneni prijimaciho bufferu
  for (i = 0; i < LenOfMsg; i++){
    recived_msg[i] = usart_recv_buff[i + 1];
  }
  recv_msg(recived_msg);
}


/**
 * Funkce slouí k odesílání zpráv, které jsou uloeny v pamìti programu.
 * Funkce naalokuje pamì pro uloení øetìzce do SRAM, øetìzec tam zkopíruje
 * a zprávu odešle
 * @param s_p Ukazatel do programové pamìti.
 * @return Ukazatel v pamìti SRAM, kam je zkopírován øetìzec z \a s_p
 **/
void new_msg_P( PGM_P s_p){
  char * p_pom;

  p_pom = (char *) malloc(LenOfMsg);
  if(p_pom == NULL){
    new_error(88); //Došla pamì
    return ;
  }
  else{
    memcpy_P(p_pom, s_p,LenOfMsg);
    new_msg(p_pom);
  }

  return;
}



/**
 * Funkce odešlì zprávu, na kterou je oèekávána odpovìï.
 * V okamiku, kdy odpovìï pøijde, bude zavolána funkce
 * v argumentu (fcn).
 * 
 * @param msg Parametr je ukazatelem na eezec, kterı bude tvoøit
 * zprávu. Na tuto zprávu bude oèekávána odpovìï.
 *
 * @param repeats Parametr urèuje poèet pokusù o odeslání zprávy.
 * 
 * @param p_fcn Ukazatel na funkci, která je spouštìna pøi odpovìdi na
 * na zprávu. Nesmí bıt NULL, pak by nefungoval èasovaè, kterı toto testuje.
 *
 * @param p_fcn_nresponse Ukazatel na funkci, která bude volána v
 * pøípadì neúpìšného èekání na odpovìï.
 *
 * @bug
 * Podezøení na chyby v uvolòování pamìti. Pøi opakovaném odeslání asi
 * dochází k uvolnìní pamìti po øetìzci msg, pøi dalším odesílání se
 * však odesílají data opìt z ukazatele msg, která jsou však ji
 * uvolnìná. 
 * 
 * */
void new_ack_msg(char * msg, unsigned char repeats, void (*p_fcn)(char*s),void (*p_fcn_nresponse)(void)){
  int i = 0;

  assert(msg != NULL);
  assert(repeats > 0);
  for(i =0; i < LenOfMsg; i++){
    last_msg_ack[i] = msg[i];
  }
  msg_ack_timer = 0;
  msg_ack_repeats = repeats;
  p_ack_fcn = p_fcn;
  p_nack_fcn = p_fcn_nresponse;
  assert(p_ack_fcn != NULL);
  assert(p_nack_fcn != NULL);
  new_msg(msg);
/*  */
}

/**
 * Funkce je volána pøi pøíjmu potvrzovací zprávy. Zavolá funkci,
 * která má bıt volána po potvrzení.
*/
void msg_ack_fcn(char * msg){
  //static int i = 0;
  if (p_ack_fcn != NULL){
    void (*p_fcn_tmp)(char*s);
    p_fcn_tmp = p_ack_fcn;
    p_ack_fcn = NULL;
    p_nack_fcn = NULL; // tohle musí bıt pøed voláním té funkce, protoe v ní je
                //znovu zahájeno vysílání zprávy a tady by se to zase zastavilo.
    p_fcn_tmp(msg);
    msg_ack_timer = 0;
    p_fcn_tmp = NULL;
    
  }
/*  msg_ack_timer = 0;
  p_ack_fcn = NULL;
  p_nack_fcn = NULL;
  printcxyd(3,0, 'a' + i);
  i++; */
}

/**
 * Funkce uzavírá komunikaci pøi neúspìšném pokusu o odeslání zprávy s
 * odpovìdí.
 */
void msg_nack_fcn(void){
  if (p_nack_fcn != NULL){
    p_nack_fcn();
  }
  //new_error(91);
  msg_ack_timer = 0;
  p_ack_fcn = NULL;
  p_nack_fcn = NULL;
}

/// Zde je išetøeno èekání na potvrzovanou zprávu.
/// Funkce zobsluhuje èítaè doby èekání na odezvu zprávy a
/// èítaè poètu pokusù o odeslání. Na odpovìï na kadou potvrzovanou zprávu se èeká
/// MAX_MSG_ACK_DELAY cyklù. Po uplynutí této doby je zpráva zopakována. Po
/// MAX_MSG_ACK_REPEATS - tém opakování je vytvoøen error.
void serial_msg_ack_timer(void){
  if (p_ack_fcn != NULL){
    msg_ack_timer++;
    if (msg_ack_timer > MAX_MSG_ACK_DELAY){
      if (msg_ack_repeats > 0){
        char * opakovana_zprava;
        int i = 0;
        
        opakovana_zprava = (char *) mmalloc(LenOfMsg);
        for (i = 0; i < LenOfMsg; i++){
          opakovana_zprava[i] = last_msg_ack[i];
        }
        
        new_msg(opakovana_zprava);
        msg_ack_repeats--;
        msg_ack_timer = 0;
      }
      else{
        msg_nack_fcn();
      }
    }
  }
}
    
/// Funkce vrací pøíslušnı znak z pøijímacího bufferu sériového kanálu.
char usart_get_recv_buff(int i){
  return usart_recv_buff[i];
}
  
/* end of usart.c */
