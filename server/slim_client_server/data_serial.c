/* $Id: data_serial.c 119 2007-09-20 14:05:51Z Mira $ 
 */
/**
 * @file data_serial.c
 * @brief
 * V tomto modulu je řešena synchrinizace dat mezi databází na serveru a
 * daty v jednočipu prostřednictvím sériové linky.
 * Po zahájení odesílání je nejprve vzslána zpráva pro vozík. Ta zastaví jeho vysílání.
 * Pak je zaslána inisializační zpráva na server. Tím dojde k přípravě databáze.
 * Příprava spočívá v tom, že systém považuje všechna data od
 * příchodu konečné zprávy minulého příjmu až do této inicializační zprávy
 * považují za "záložní".
 * Pak je zahájeno odesílání jednotlivých jmen zákazníků. Po odeslání všech
 * jsou odesílána data naměřených stromů. V závěru je odeslána ukočovací zpráva.
 * Ta způsobí přesun dat z dočasné databáze do databáze s níž již pracuje
 * webové rozhraní. Tím je komunikace se serverem ukončena. Posledí odesílanou
 * zprávou je znovuobnovení vysílání vozíku.
 * Na závěr jsou vymazána data z databáze v provozu.
 * */
#include <stdint.h>
#include <string.h>
//#include <avr/pgmspace.h>
#include <stdlib.h>
#include "msr_scr.h"
//#include "usart.h"
#include "data_serial.h"
#include "data.h"
#include "error.h"
#include "main.h"
#include "measure.h"
//#include "lcd.h"

//global
void send_data(void);
void send_next_data(char*s);
// global

/* Je třeba hlídat při změně velikosti zprávy v usart.c */

/// Počet opakování zpráv při odesílání dat.
#define MSG_REPEATS 2

//static char server_data_buffer[DATA + 1];
static unsigned int id_counter = 0;
static unsigned char stav = 0;

//static char * name_msg(void);
//static char * mereni_msg(void);
//static void err91(void);


/**
 * Funkce je volána v případě neúspěšného čekání na odpověď. Vypíše
 * chybovou hlášku a obnoví komunikaci se zařízením na katru.
 */
/*static void err91(void){
  char * msg;
  new_error(91);
  msg = over_msg();
  //new_msg(msg);

}*/

/** Funkce zahajuje odeslílání dat */
void send_data(void){
  id_counter = 0;
  stav = 1;
  
  send_next_data(NULL);
  //if (name_msg() == 1){
  //new_ack_msg(server_data_buffer,send_next_data);
  //}
}

/**
 * Funkce odešle další data.
 * Je volána z venku od seriové linky.
 */
void send_next_data(char*s){  
/*


  if (stav == 1){// umlceni katru //Zatim nefunkcni pocatecni stav neni 1, ale 2
    char * msg;
    msg =(void*) mmalloc (LenOfMsg);
    
    msg[0] = '\x09';
    msg[1] = 0;
    new_ack_msg(msg,MSG_REPEATS,send_next_data, err91);
    stav++;
    return;
  }
  if (stav == 2){ // informace pro server o zah�jen� odes�l�n� dat
    char * msg;
    msg =(void*) mmalloc (LenOfMsg);

    msg[0] = '\x44'; //TODO nevim jestli je ��slo zpr�vy spr�vn�
    new_ack_msg(msg,MSG_REPEATS,send_next_data,err91);
    stav++;
    return;
  }
  if (stav == 3){//odes�l�n� jmen
    char * msg;
    if ((msg = name_msg()) != NULL){
      new_ack_msg(msg,MSG_REPEATS,send_next_data,err91);
    }
    else{
      stav++;
      id_counter = 0;
    }
  } 
  if (stav == 4){ //odes�l�n� vlastn�ch dat
    char * msg;
    if ((msg = mereni_msg()) != NULL){
      new_ack_msg(msg,MSG_REPEATS,send_next_data,err91);
    }
    else{
      stav++;
    }
  }
  if (stav == 5){ //z�v�re�n� zpr�va
    char * msg;
    msg = over_msg();
    new_ack_msg(msg,MSG_REPEATS,send_next_data,err91);
    stav++;
    return;
  }
  if (stav == 6){ // TODO zat�m neaktivn� - odkomentovat
/// @todo odstran�n� z�znam�.
// Zde by m�lo doj�t k vymaz�n� datab�ze.
// Je to toti� nejbl�e od posledn� zpr�vy
// pro server.
    char * msg;
    msg =(void*) mmalloc (LenOfMsg);

    msg[0] = '\x09';
    msg[1] = 1;
    new_ack_msg(msg,MSG_REPEATS,send_next_data,err91);
    stav++;
    return;
  }

  if (stav == 7){
    new_error(200); // V�pis : Data odesl�na.
    // prisla odpoved na posledni zpravu -> delete zaznamy
    dat_del_all();
    msr_storno();
  }
*/

}


