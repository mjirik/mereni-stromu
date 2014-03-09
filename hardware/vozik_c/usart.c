/* $Id: usart.c 149 2009-10-25 17:36:42Z mjirik $
 */
/**
 * @file usart.c
 * @brief
 * Modul zajišťuje odesílání zpráv po sériové lince.
 * Funkce new_ack_msg() vytvoří zprávu, na níž je očekávána
 * odpověď. Při přijentí zprávy je zavolána funkce, na níž u
 * ukazuje poínter v parametru funkce. V případě, že je
 * překročena určitá doba (daná MAX_MSG_ACK_DELAY) je odeslání
 * zprávy zopakováno. Kolikrát toto opakování proběhne ukazuje
 * proměnná MAX_MSG_ACK_REPEATS (celkem je zpráva odeslána
 * MAX_MSG_ACK_REPEATS + 1 krát). Pokud ani na opakované odesílání
 * není obdržena odpověď, je vyhlášen error.
 *
 * Odesilani zprav pracuje tak, ze funkce new_msg() vytvari a udrzuje seznam
 * zprav a funkce send_msg() je užívána při odesílání pro vybírání z tohoto seznamu
 * Používají se zde počítadla.
 * msg_buffer_counter ukazuje na zpravu, kterou je treba odeslat. Je-li -1, pak
 * není žádná zpráva k odeslání.
 *
 *
 * Došlo k úpravě. Byl naimplementován lower_buffer. Při příjmu je každý byte
 * nejpve přesunut do tohoto bufferu. Další zpracování se provádí až později,
 * pomocí dosavadních funkcí.
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
#include "common/msgn.h"
#include "memory.h"
#include "sysinfo.h"
#include "watchdog.h" // kvuli wtch_dbg_info


// global

void USART_Init(unsigned int baud);
void new_msg(char *);
void new_ack_msg(char * msg, unsigned char repeats, void (*p_fcn)(char*s), void (*p_fcn_nresponse)(void));
void msg_ack_fcn(char * msg);
void serial_msg_ack_timer(void);
void new_msg_P(PGM_P s_p);
char usart_get_recv_buff(int i);
void serial_read_buffer(void);


void send_uint32(char msgn, uint32_t number1, uint32_t number2);
void recive_uint32(char * msg, uint32_t * number1, uint32_t * number2);

void msg_time_0(void);
void msg_time_pp(void);
uint32_t get_msg_time(void);
char* get_recv_buffer(void);


uint32_t msg_time;
// global

//#include "lcd.h"

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
#define SIZE_OF_LOWER_BUFFER 250


#define MAX_MSG_ACK_DELAY 5000
//#define MAX_MSG_ACK_REPEATS 2



/// Proměnné kruhového zásobníku.
///
/// @{
char lower_buffer[SIZE_OF_LOWER_BUFFER];
//char lower_buffer_counter = 0;
/// Ukazuje na byte za posledím zapsaným bytem.
int lower_buffer_newest = 0;
int lower_buffer_oldest = 0;
/// @}

//int pocitadlo = 0; //pocitadlo pro testy. Odstranit!
int usart_recv_count = -1;
char usart_recv_buff[LenOfRecvBuff];
char usart_trans_buff[LenOfTransBuff];
char prepinani = 0;

/// proměnná detekuje zda zároveň běží čtení a zápis do bufferu
//static char kolikrat_spusteno = 0;

//* Pokud je usart_trans_buff = -1 znamena to, ze je buffer prazdny
int usart_trans_count = -1;

//* Pole udržuje seznam zpráv k odeslaní.
char * msg_buffer[LenOfMsgBuffer];
int msg_buffer_counter = -1;

/// Buffer pro přijatou zprávu.
char recived_msg[LenOfMsg];

/// Odmlka po odeslání.
int after_trans_delay_counter = Delay ;

/// Časovač. Pokud přeteče, znamená to, že nedorazila včas odpověď
/// na potvrzovanou zprávu.
static unsigned int msg_ack_timer = 0;

/// zde je poslední odeslaná zpráva pro účel opakování v případě nedoručení
/// @todo pole by šlo zmenšit na LenOfMsg
static char last_msg_ack[LenOfTransBuff];

///počet opakování odeslání zprávy
static unsigned char msg_ack_repeats = 0;

/** Ukazatel na funkci, která bude volána po pčijetí potvrzení zprávy */
static void (*p_ack_fcn)(char*s) = NULL;

/// Ukazatel na funkci, kter bude volána v případě, že čekání na
/// odpověď je neúspěšné.
static void (*p_nack_fcn)(void) = NULL;

/// počítadlo od poslední doručené zprávy, takový je, aby hned od začátku fungoval default_scr
uint32_t msg_time = 30e4;

// Deklarace funkci
void zarad_tri_zpravy(void);
void send_msg (void);
int test_message(char *msg);
void process_recived_msg(void);

static void reset_buffer(void);
static void into_buffer(unsigned char c);
//static void read_port(void);

char* get_recv_buffer(void){
  return usart_recv_buff;
}



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
  DDRD = DDRD | (1 << 4); // přepínání sériových linek


}



/// funkce inkrementuje počítadlo od poslední doručené zprávy
void msg_time_pp(void){
  if (msg_time < (100e4))
  msg_time++;
}

/// Funkce nuluje počítadlo
void msg_time_0(void){
  msg_time = 0;
}

/// funkce vrací hodnotu počítadla
uint32_t get_msg_time(void){
  return (msg_time);
}


/// Funkce vynuluje přijímací buffer
void reset_buffer(void){
  int i=0;
  for (i=0; i < LenOfRecvBuff; i++){
    usart_recv_buff[i]=0;
  }
}


/**
 * Funkce vkládá data do bufferu.
 * Funkce je založená na stejné implementaci jako na serveru.
 */
void into_buffer(unsigned char c){
  int i = 0;
  for (i = 0;i < LenOfRecvBuff - 1;i++){
    usart_recv_buff[i]=usart_recv_buff[i+1];
  }
  usart_recv_buff[i]=c;
}


/**
 * Funkce zvýší (nebo protočí na začátek) "ukazatel" na nejstarší byte
 * lower_bufferu. Pokud by bylo toto zvýšení přímo naimplementováno ve funkcích,
 * nebylo by možné zjistit, zda k tomuto nedochází ve více "vláknech". Například
 * z programu a z přerušení. Problém by to byl zejména na konci těsně před
 * přerušením, kdy by v jednom volání mohlo dojít k testování konce, pak k
 * přerušení, novému testování a následně dvojitému zvýšení čítače. Toto je nyní
 * ošetřeno.
 */
void inc_lower_buffer_oldest(void) {
  static char instance_counter = 0;
  instance_counter++;

  // předpokládá se, že funkce nebude volána více než 5x najednou, takže není
  // prováděno testování instance_counter < SIZE_OF_LOWER_BUFFER


  // Nastavení hodnoty ukazatele na 2. nejstarší přijatý byte
  //if (lower_buffer_oldest < (SIZE_OF_LOWER_BUFFER - 1)) {

  if (lower_buffer_oldest < (SIZE_OF_LOWER_BUFFER - instance_counter)) {
    // problém je pokud dojde k přerušení právě ZDE
    lower_buffer_oldest++;
  } else {
    // pokud jsme na konci paměti bufferu, jdeme na začátek
    //lower_buffer_oldest = 0;
    lower_buffer_oldest = lower_buffer_oldest - (SIZE_OF_LOWER_BUFFER - 1);
  }


  // toto je jen informační error, situace je ošetřena v kódu výše
  // k chybě dojde jen při naplnění bufferu, bufferu na konci a současném volání
  //funkce z programu i přerušení
  if (instance_counter > 1) {
    new_error(ERRN_RECIVE_END_OF_BUFFER_BUG);
  }

  instance_counter--;
}

/**
 * Funkce vloží byte do lower_buffer. Buffer je kruhový. V případě že dojde k
 * jeho naplnění, je vyhlášen error a je zapomenut nejstarší byte.
 * 
 */
void into_lower_buffer(char recived_byte) {

  lower_buffer[(int)lower_buffer_newest] = recived_byte;

  // Nastavení hodnoty ukazatele za nejnovější přijatý buffer.
  if (lower_buffer_newest < (SIZE_OF_LOWER_BUFFER-1)) {
    lower_buffer_newest++;
  } else {
    // pokud jsme na konci paměti bufferu, jdeme na začátek
    lower_buffer_newest = 0;
  }

  // Došlo-li k dostižení ukazatele na nejstarší byte, je zásobník přeplněn.
  if (lower_buffer_newest == lower_buffer_oldest){
    // zásobník přeplněn -> dojde k zapomenutí nejstaršího byte


    inc_lower_buffer_oldest();


    
    new_error(ERRN_RECIVE_BUFFER_FULL);
  }

}


/**
 * Funkce načítá z lower_buffer
 * @return vrací nejstarí byte z bufferu, pokud je buffer prázdný, vrací -1
 */
int getc_lb(void) {
  int out;
  
  // test, zda je prázdný buffer
  if (lower_buffer_oldest == lower_buffer_newest) {
    // prázný buffer

    out = -1;
  } else {
    // neprázdný buffer

    out = (int) lower_buffer[(int) lower_buffer_oldest];

    inc_lower_buffer_oldest();


  }

  
  return out;
}

/// Nová implementace přijímní zpráv
/// Není naimplementována chyba z chybně přijatéo prvního znaku.
void serial_read_buffer(void) {
  static unsigned char byte_counter = 0;
  int c;

  wtch_dbg_info();
  //  unsigned char c;
  //  unsigned char typ=0;

  // načtení z lower_buffer
  while ((c = getc_lb()) != -1) {
    into_buffer((char) c);

    // obsah bufferu se tiskne jen pokud je to povoleno
    si_print_recv_buffer();
    byte_counter++;

    if ((byte_counter >= LenOfRecvBuff) && (test_message(usart_recv_buff) == 1)) {
      process_recived_msg();
      //reset_buffer(); // reset naplní nulami a pak je možné, že dorazí jen jeden znak 255 a ostatní nuly
      byte_counter = 0;
      //    zpracuj_zpravu();
      //dorazilo
    }
    if ((usart_recv_buff[0] == 255) && (byte_counter > LenOfRecvBuff)) {
      //si_print_recv_buffer();
      // chyba prenosu - neshoda v kontrolnim souctu
      new_error(86);
    }
  }
  

}

/// Tahle část se spouští vždy když přijmeme nějaký byte po sériovém kanále.
SIGNAL(SIG_UART_RECV){
  
  /*kolikrat_spusteno++;

  if (kolikrat_spusteno > 1){
    new_error(ERRN_RECIVER_OVERLOADED);
  }
  else{*/
    into_lower_buffer(UDR);
  
  /*
  }
  kolikrat_spusteno--;
  */
  
}



/// Tohle se spouští když je prázdný odesílací buffer.
/// Když je zapotřebí vysílat, stačí, když nastaví usart_trans_count = 0 .
/// Odesílá to po bytech věci z usart_trans_bufferu az do konce
/// na konci je malé odmlčení, kvůli lepší synchronizaci
SIGNAL(SIG_UART_DATA) {
  wtch_dbg_info();
  // Pokud neni co odesilat, tak se mrnkni jestli neco nema send_msg
  if (usart_trans_count == -1) {

    send_msg();

  }

  // pokud je neco v bufferu, tak makej
  if (usart_trans_count != -1) {

    if (usart_trans_count != LenOfTransBuff) { // zde je odesílán byte po bytu

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


    // teď se po každým odeslání udělá pauza
    // dela se to tak, že se vždycky sníží counter a když je nula, tak se vše
    // nastaví a ukončí
    //*
    if (usart_trans_count == LenOfTransBuff) {

      if (after_trans_delay_counter != 0) {
        after_trans_delay_counter--;


      } else {
        after_trans_delay_counter = Delay;
        usart_trans_count = -1;
      }
    }
    //*/

  } else {

    UCSRB = UCSRB & (~(1 << UDRIE));
  }

}




/// Tato funkce obaluje zpravu kodem pouzivanym pro prenos po seriovem kanale.
/// Zde je provedeno uvolnění paměti zprávy. Je tak učiněno po nakopírování
/// zprávy do odesílacího bufferu.

void to_transmitt_buffer(char *msg) {
  int i = 0;

  wtch_dbg_info();
  usart_trans_buff[0] = 255;

  for (i = 0; i < LenOfMsg; i++) {
    usart_trans_buff[i + 1] = msg[i];
  }
  usart_trans_buff[LenOfMsg + 1] = usart_trans_buff[0] ^ msg[0];

  for (i = 0; i < DATASIZE; i = i + 2) {
    usart_trans_buff[DATASIZE + 3 + (i / 2)] = msg[i + 1]^msg[i + 2];
  }

  mfree((void *) msg);

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
/// Prostor musí být správně naalokovaný. Po odeslání zprávy je paměť uvolněna.
void new_msg (char *msg){
   wtch_dbg_info();
//pokud nedochazi k prepinani
  //if (prepinani == 0){
  if (1){

  // pokud zadna zprava v seznamu neni, tak ji strc na nulty misto
  // a nastartuj citac a zapni preruseni od datovaho bufferu.
    if (msg_buffer_counter == -1){
      
      msg_buffer_counter = 0;

      msg_buffer[msg_buffer_counter] = msg;
/*      if (msg != NULL){
        char mystr[40];
        sprintf(mystr, "msg = %x %x %x %x", (unsigned int)msg[0],(unsigned int)msg[1],(unsigned int)msg[2],(unsigned int)msg[3]);
        printnt(mystr);
      }*/
      UCSRB = UCSRB | (1 << UDRIE);
    
    }
    // pokud jsou v seznamu zpravy, strc ji na konec.
    else{
      int i = 0;

      // testujeme, zda uz zprava s tímto kódem neni v seznamu
      // pokud je, nadradíme starší tou novější
      for (i = 0; i < LenOfMsgBuffer; i++) {
        if (msg_buffer[i][0] == msg[0]) {
          mfree((void *) msg_buffer[i]);
          msg_buffer[i] = msg;


          // funkční bez nahrazení starší zprávy
          //mfree((void *) msg);
          //msg = NULL;
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
  wtch_dbg_info();
  if (msg_buffer_counter != -1){
    char *poiter_to_msg;

    wtch_dbg_info();
    // zkopirujeme obsah do bufferu
    poiter_to_msg = msg_buffer[msg_buffer_counter];
    wtch_dbg_info();
    to_transmitt_buffer(poiter_to_msg);
    wtch_dbg_info();

    /*
    for (i = 0; i < LenOfTransBuff; i++){
      usart_trans_buff[i] = poiter_to_msg[i];
    }
    */
    // 
    msg_buffer[msg_buffer_counter] = NULL;

    // zahajime odesilani
    usart_trans_count = 0;

    wtch_dbg_info();
    // Pokusime se nalezt dalsi zpravu
    if (msg_buffer_counter == (LenOfMsgBuffer - 1)){
      msg_buffer_counter = 0;
    }
    else{
      msg_buffer_counter++;
    }

    // Pokud zprava neexistuje, nastavime pocitadlo na -1;
    if (msg_buffer[msg_buffer_counter] == NULL){
      wtch_dbg_info();
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

/// V těle této funkce je řešeno zpracování přijaté zprávy.
void process_recived_msg(){
  int i = 0;
  
  //zkopirovani zpravy a tim uvoneni prijimaciho bufferu
  for (i = 0; i < LenOfMsg; i++){
    recived_msg[i] = usart_recv_buff[i + 1];
  }
  recv_msg(recived_msg);
}


/**
 * Funkce slouží k odesílání zpráv, které jsou uloženy v paměti programu.
 * Funkce naalokuje paměť pro uložení řetězce do SRAM, řetězec tam zkopíruje
 * a zprávu odešle
 * @param s_p Ukazatel do programové paměti.
 * @return Ukazatel v paměti SRAM, kam je zkopírován řetězec z \a s_p
 **/
void new_msg_P( PGM_P s_p){
  char * p_pom;

  p_pom = (char *) mmalloc(LenOfMsg);
  if(p_pom == NULL){
    new_error(88); //Došla paměť
    return ;
  }
  else{
    memcpy_P(p_pom, s_p,LenOfMsg);
    new_msg(p_pom);
  }

  return;
}



/**
 * Funkce odešlě zprávu, na kterou je očekávána odpověď.
 * V okamžiku, kdy odpověď přijde, bude zavolána funkce
 * v argumentu (fcn).
 * 
 * @param msg Parametr je ukazatelem na žeťezec, který bude tvořit
 * zprávu. Na tuto zprávu bude očekávána odpověď.
 *
 * @param repeats Parametr určuje počet pokusů o odeslání zprávy.
 * 
 * @param p_fcn Ukazatel na funkci, která je spouštěna při odpovědi na
 * na zprávu. Nesmí být NULL, pak by nefungoval časovač, který toto testuje.
 *
 * @param p_fcn_nresponse Ukazatel na funkci, která bude volána v
 * případě neúpěšného čekání na odpověď.
 *
 * @bug
 * Podezření na chyby v uvolňování paměti. Při opakovaném odeslání asi
 * dochází k uvolnění paměti po řetězci msg, při dalším odesílání se
 * však odesílají data opět z ukazatele msg, která jsou však již
 * uvolněná. 
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
 * Funkce je volána při příjmu potvrzovací zprávy. Zavolá funkci,
 * která má být volána po potvrzení.
*/
void msg_ack_fcn(char * msg){
  //static int i = 0;
  if (p_ack_fcn != NULL){
    void (*p_fcn_tmp)(char*s);
    p_fcn_tmp = p_ack_fcn;
    p_ack_fcn = NULL;
    p_nack_fcn = NULL; // tohle musí být před voláním té funkce, protože v ní je
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
 * Funkce uzavírá komunikaci při neúspěšném pokusu o odeslání zprávy s
 * odpovědí.
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

/// Zde je išetřeno čekání na potvrzovanou zprávu.
/// Funkce zobsluhuje čítač doby čekání na odezvu zprávy a
/// čítač počtu pokusů o odeslání. Na odpověď na každou potvrzovanou zprávu se čeká
/// MAX_MSG_ACK_DELAY cyklů. Po uplynutí této doby je zpráva zopakována. Po
/// MAX_MSG_ACK_REPEATS - tém opakování je vytvořen error.
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
    
/// Funkce vrací příslušný znak z přijímacího bufferu sériového kanálu.
char usart_get_recv_buff(int i){
  return usart_recv_buff[i];
}
  
/* end of usart.c */


/**
 * Funkce odešle jedno až dvě čísla uint16 po sériové lince
 */
void send_uint32(char msgn, uint32_t number1, uint32_t number2){
  /*
   *   unsigned int i = 4;
  unsigned char * c0;
  unsigned char * c1;
  unsigned char * c2;
  unsigned char * c3;

  c0 = (unsigned char *) &i;
  c1 = (unsigned char *) &i+1;
  c2 = (unsigned char *) &i+2;
  c3 = (unsigned char *) &i+3;
  printf("cislo %i , cisla %i %i %i %i", i ,(int)*c0,(int)*c1,(int)*c2,(int)*c3 );
   * */


  char * msg;

  uint32_t * uint32;

   wtch_dbg_info();
//  unsigned char num;

  // alokace místa pro novou zprávu, nastavení vlajky zprávy
  msg =(void*) mmalloc (DATASIZE);
  msg[0] = msgn;


  // zde se vytvoří ukazatel na první datový byte zprávy, řekne se že je to
  // uint32 a přiřadí se do něj hodnota
  uint32 = ((uint32_t *)&(msg[1]));
  *uint32 = number1;

  uint32 = ((uint32_t *)&(msg[5]));
  *uint32 = number2;


  //msg[1] = *((unsigned char*) (&number1+0));
/*
  //number1 = number1;
  msg[1] = number1 % 256;
  number1 = number1 / 256;
  msg[2] = number1 % 256;
  number1 = number1 / 256;
  msg[3] = number1 % 256;
  number1 = number1 / 256;
  msg[4] = number1 % 256;


  // druhý číslo
    //num = number2;
    msg[5] = number2 % 256;
    number2 = number2 / 256;
    msg[6] = number2 % 256;
    number2 = number2 / 256;
    msg[7] = number2 % 256;
    number2 = number2 / 256;
    msg[8] = number2 % 256;
*/

  new_msg(msg);
  
}

/// Funkce získá ze zprávy dvě čísla uint32_t
/// @param number1 ukazatel na číslo, kam se má vrátit první číslo
/// @param number2 ukazatel na číslo, kam se má vrátit druhé číslo, pokud
//     je NULL, neprovádí se nic
/// @param msg zpráva z níž mají být čísla získána
void recive_uint32(char * msg, uint32_t * number1, uint32_t * number2){
  uint32_t * uint32;


  uint32 = (uint32_t *) (msg);
  *number1 = *uint32;

  uint32 = (uint32_t *) (msg+4);
  *number2 = *uint32;

}

