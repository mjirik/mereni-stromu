/* $Id: usart.c 120 2007-09-21 05:43:42Z Mira $
 */
/**
 * @file usart.c
 * @brief
 * Modul zaji��uje odes�l�n� zpr�v po s�riov� lince.
 * Funkce new_ack_msg() vytvo�� zpr�vu, na n� je o�ek�v�na
 * odpov��. P�i p�ijent� zpr�vy je zavol�na funkce, na n� u
 * ukazuje po�nter v parametru funkce. V p��pad�, �e je
 * p�ekro�ena ur�it� doba (dan� MAX_MSG_ACK_DELAY) je odesl�n�
 * zpr�vy zopakov�no. Kolikr�t toto opakov�n� prob�hne ukazuje
 * prom�nn� MAX_MSG_ACK_REPEATS (celkem je zpr�va odesl�na
 * MAX_MSG_ACK_REPEATS + 1 kr�t). Pokud ani na opakovan� odes�l�n�
 * nen� obdr�ena odpov��, je vyhl�en error.
 *
 * Odesilani zprav pracuje tak, ze funkce new_msg() vytvari a udrzuje seznam
 * zprav a funkce send_msg() je u��v�na p�i odes�l�n� pro vyb�r�n� z tohoto seznamu
 * Pou��vaj� se zde po��tadla.
 * msg_buffer_counter ukazuje na zpravu, kterou je treba odeslat. Je-li -1, pak
 * nen� ��dn� zpr�va k odesl�n�.
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

//* Pole udr�uje seznam zpr�v k odeslan�.
char * msg_buffer[LenOfMsgBuffer];
int msg_buffer_counter = -1;

/// Buffer pro p�ijatou zpr�vu.
char recived_msg[LenOfMsg];

/// Odmlka po odesl�n�.
int after_trans_delay_counter = Delay ;

/// �asova�. Pokud p�ete�e, znamen� to, �e nedorazila v�as odpov��
/// na potvrzovanou zpr�vu.
static unsigned int msg_ack_timer = 0;

/// zde je posledn� odeslan� zpr�va pro ��el opakov�n� v p��pad� nedoru�en�
/// @todo pole by �lo zmen�it na LenOfMsg
static char last_msg_ack[LenOfTransBuff];

///po�et opakov�n� odesl�n� zpr�vy
static unsigned char msg_ack_repeats = 0;

/** Ukazatel na funkci, kter� bude vol�na po p�ijet� potvrzen� zpr�vy */
static void (*p_ack_fcn)(char*s) = NULL;

/// Ukazatel na funkci, kter bude vol�na v p��pad�, �e �ek�n� na
/// odpov�� je ne�sp�n�.
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
  DDRD = DDRD | (1 << 4); // p�ep�n�n� s�riov�ch linek


}


/// Funkce vynuluje p�ij�mac� buffer
void reset_buffer(void){
  int i=0;
  for (i=0; i < LenOfRecvBuff; i++){
    usart_recv_buff[i]=0;
  }
}


/**
 * Funkce vkládá data do bufferu.
 * Funkce je zalo�en� na stejn� implementaci jako na serveru.
 */
void into_buffer(unsigned char c){
  int i=0;
  for (i=0;i < LenOfRecvBuff - 1;i++){
    usart_recv_buff[i]=usart_recv_buff[i+1];
  }
  usart_recv_buff[i]=c;
}


/// Nov� implementace p�ij�mn� zpr�v
/// Nen� naimplementov�na chyba z chybn� p�ijat�o prvn�ho znaku.
/// @todo Znovuzapojit resetova� bufferu.
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

/// Tahle ��st se spou�t� v�dy kdy� p�ijmeme n�jak� byte po s�riov�m kan�le.
SIGNAL(SIG_UART_RECV){
  read_port();

  
  //zarad_tri_zpravy();
}



/// Tohle se spou�t� kdy� je pr�zdn� odes�lac� buffer.
/// Kdy� je zapot�eb� vys�lat, sta��, kdy� nastav� usart_trans_count = 0 .
/// Odes�l� to po bytech v�ci z usart_trans_bufferu az do konce
/// na konci je mal� odml�en�, kv�li lep�� synchronizaci
SIGNAL(SIG_UART_DATA){
  // Pokud neni co odesilat, tak se mrnkni jestli neco nema send_msg
  if (usart_trans_count == -1){
    send_msg();
    
  }
  // pokud je neco v bufferu, tak makej
  if (usart_trans_count != -1){
    
    if (usart_trans_count != LenOfTransBuff){ // zde je odes�l�n byte po bytu
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
 

    // te� se po ka�d�m odesl�n� ud�l� pauza
    // dela se to tak, �e se v�dycky sn�� counter a kdy� je nula, tak se v�e
    // nastav� a ukon��
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
/// Zde je provedeno uvoln�n� pam�ti zpr�vy. Je tak u�in�no po nakop�rov�n�
/// zpr�vy do odes�lac�ho bufferu.
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
/// Prostor mus� b�t spr�vn� naalokovan�. Po odesl�n� zpr�vy je pam� uvoln�na.
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

/// V t�le t�to funkce je �e�eno zpracov�n� p�ijat� zpr�vy.
void process_recived_msg(){
  int i = 0;
  
  //zkopirovani zpravy a tim uvoneni prijimaciho bufferu
  for (i = 0; i < LenOfMsg; i++){
    recived_msg[i] = usart_recv_buff[i + 1];
  }
  recv_msg(recived_msg);
}


/**
 * Funkce slou�� k odes�l�n� zpr�v, kter� jsou ulo�eny v pam�ti programu.
 * Funkce naalokuje pam� pro ulo�en� �et�zce do SRAM, �et�zec tam zkop�ruje
 * a zpr�vu ode�le
 * @param s_p Ukazatel do programov� pam�ti.
 * @return Ukazatel v pam�ti SRAM, kam je zkop�rov�n �et�zec z \a s_p
 **/
void new_msg_P( PGM_P s_p){
  char * p_pom;

  p_pom = (char *) malloc(LenOfMsg);
  if(p_pom == NULL){
    new_error(88); //Do�la pam�
    return ;
  }
  else{
    memcpy_P(p_pom, s_p,LenOfMsg);
    new_msg(p_pom);
  }

  return;
}



/**
 * Funkce ode�l� zpr�vu, na kterou je o�ek�v�na odpov��.
 * V okam�iku, kdy odpov�� p�ijde, bude zavol�na funkce
 * v argumentu (fcn).
 * 
 * @param msg Parametr je ukazatelem na �e�ezec, kter� bude tvo�it
 * zpr�vu. Na tuto zpr�vu bude o�ek�v�na odpov��.
 *
 * @param repeats Parametr ur�uje po�et pokus� o odesl�n� zpr�vy.
 * 
 * @param p_fcn Ukazatel na funkci, kter� je spou�t�na p�i odpov�di na
 * na zpr�vu. Nesm� b�t NULL, pak by nefungoval �asova�, kter� toto testuje.
 *
 * @param p_fcn_nresponse Ukazatel na funkci, kter� bude vol�na v
 * p��pad� ne�p�n�ho �ek�n� na odpov��.
 *
 * @bug
 * Podez�en� na chyby v uvol�ov�n� pam�ti. P�i opakovan�m odesl�n� asi
 * doch�z� k uvoln�n� pam�ti po �et�zci msg, p�i dal��m odes�l�n� se
 * v�ak odes�laj� data op�t z ukazatele msg, kter� jsou v�ak ji�
 * uvoln�n�. 
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
 * Funkce je vol�na p�i p��jmu potvrzovac� zpr�vy. Zavol� funkci,
 * kter� m� b�t vol�na po potvrzen�.
*/
void msg_ack_fcn(char * msg){
  //static int i = 0;
  if (p_ack_fcn != NULL){
    void (*p_fcn_tmp)(char*s);
    p_fcn_tmp = p_ack_fcn;
    p_ack_fcn = NULL;
    p_nack_fcn = NULL; // tohle mus� b�t p�ed vol�n�m t� funkce, proto�e v n� je
                //znovu zah�jeno vys�l�n� zpr�vy a tady by se to zase zastavilo.
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
 * Funkce uzav�r� komunikaci p�i ne�sp�n�m pokusu o odesl�n� zpr�vy s
 * odpov�d�.
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

/// Zde je i�et�eno �ek�n� na potvrzovanou zpr�vu.
/// Funkce zobsluhuje ��ta� doby �ek�n� na odezvu zpr�vy a
/// ��ta� po�tu pokus� o odesl�n�. Na odpov�� na ka�dou potvrzovanou zpr�vu se �ek�
/// MAX_MSG_ACK_DELAY cykl�. Po uplynut� t�to doby je zpr�va zopakov�na. Po
/// MAX_MSG_ACK_REPEATS - t�m opakov�n� je vytvo�en error.
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
    
/// Funkce vrac� p��slu�n� znak z p�ij�mac�ho bufferu s�riov�ho kan�lu.
char usart_get_recv_buff(int i){
  return usart_recv_buff[i];
}
  
/* end of usart.c */
