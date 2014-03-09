/* $Id: data_serial.c 119 2007-09-20 14:05:51Z Mira $ 
 */
/**
 * @file data_serial.c
 * @brief
 * V tomto modulu je øešena synchrinizace dat mezi databází na serveru a
 * daty v jednoèipu prostøednictvím sériové linky.
 * Po zahájení odesílání je nejprve vzslána zpráva pro vozík. Ta zastaví jeho vysílání.
 * Pak je zaslána inisializaèní zpráva na server. Tím dojde k pøípravì databáze.
 * Pøíprava spoèívá v tom, že systém považuje všechna data od
 * pøíchodu koneèné zprávy minulého pøíjmu až do této inicializaèní zprávy
 * považují za "záložní".
 * Pak je zahájeno odesílání jednotlivých jmen zákazníkù. Po odeslání všech
 * jsou odesílána data namìøených stromù. V závìru je odeslána ukoèovací zpráva.
 * Ta zpùsobí pøesun dat z doèasné databáze do databáze s níž již pracuje
 * webové rozhraní. Tím je komunikace se serverem ukonèena. Posledí odesílanou
 * zprávou je znovuobnovení vysílání vozíku.
 * Na závìr jsou vymazána data z databáze v provozu.
 * */
#include <string.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "msr_scr.h"
#include "usart.h"
#include "data_serial.h"
#include "data.h"
#include "error.h"
#include "main.h"
#include "measure.h"
//#include "lcd.h"

/* Je tøeba hlídat pøi zmìnì velikosti zprávy v usart.c */
//#define DATA 10

/// Poèet opakování zpráv pøi odesílání dat.
#define MSG_REPEATS 2

//static char server_data_buffer[DATA + 1];
static unsigned int id_counter = 0;
static unsigned char stav = 0;

static char * name_msg(void);
static char * mereni_msg(void);
static char * over_msg(void);
static void err91(void);


/**
 * Funkce je volána v pøípadì neúspìšného èekání na odpovìï. Vypíše
 * chybovou hlášku a obnoví komunikaci se zaøízením na katru.
 */
static void err91(void){
  char * msg;
  new_error(91);
  msg = over_msg();
  new_msg(msg);

}

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


  if (stav == 1){// umlceni katru //Zatim nefunkcni pocatecni stav neni 1, ale 2
    char * msg;
    msg =(void*) mmalloc (LenOfMsg);
    
    msg[0] = '\x09';
    msg[1] = 0;
    new_ack_msg(msg,MSG_REPEATS,send_next_data, err91);
    stav++;
    return;
  }
  if (stav == 2){ // informace pro server o zahájení odesílání dat
    char * msg;
    msg =(void*) mmalloc (LenOfMsg);

    msg[0] = '\x44'; //TODO nevim jestli je èíslo zprávy správnì
    new_ack_msg(msg,MSG_REPEATS,send_next_data,err91);
    stav++;
    return;
  }
  if (stav == 3){//odesílání jmen
    char * msg;
    if ((msg = name_msg()) != NULL){
      new_ack_msg(msg,MSG_REPEATS,send_next_data,err91);
    }
    else{
      stav++;
      id_counter = 0;
    }
  } 
  if (stav == 4){ //odesílání vlastních dat
    char * msg;
    if ((msg = mereni_msg()) != NULL){
      new_ack_msg(msg,MSG_REPEATS,send_next_data,err91);
    }
    else{
      stav++;
    }
  }
  if (stav == 5){ //závìreèná zpráva
    char * msg;
    msg = over_msg();
    new_ack_msg(msg,MSG_REPEATS,send_next_data,err91);
    stav++;
    return;
  }
  if (stav == 6){ // TODO zatím neaktivní - odkomentovat
/// @todo odstranìní záznamù.
// Zde by mìlo dojít k vymazání databáze.
// Je to totiž nejblíže od poslední zprávy
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
    new_error(200); // Výpis : Data odeslána.
    // prisla odpoved na posledni zpravu -> delete zaznamy
    dat_del_all();
    msr_storno();
  }

}

/**
 * Funkce sestaví zprávu která obsahuje jméno zákazníka a jeho id.
 * Tato zpráva je uložena do server_data_buffer
 */
char * name_msg(void){
  while ((m_jmena[(int)id_counter] == NULL)&&(id_counter < MAX_ZAKAZNIK - 1)){
    id_counter++;
  }
  if (m_jmena[(int)id_counter] != NULL){
    char * msg;
    msg =(void*) mmalloc (LenOfMsg);
    msg[0]='\x41';
    msg[1] = id_counter;
    strncpy(msg + 3, m_jmena[(int)id_counter], 8);
    id_counter++;
    return msg;
  }
  else{
    id_counter = 0;
    return NULL;
  }
}

/**
 * Funkce vytvoøí zprávu, která obsahuje namìøená data. Zpráva je uložena
 * do server_data_buffer.
 */
char * mereni_msg(void){//todo
  if (id_counter < data_top){
    char * msg;
    msg =(void*) mmalloc (LenOfMsg);

    r_record((unsigned int)id_counter);
    //r_record((uint16_t)0);
    msg[0] = '\x42';
    msg[1] = (unsigned char)id_counter;
    msg[2] = (unsigned char)data_zakaznik;
    msg[3] = (unsigned char)(data_delka / 255);
    msg[4] = (unsigned char)(data_delka % 255);
    msg[5] = (unsigned char)(data_prumer / 255);
    msg[6] = (unsigned char)(data_prumer % 255);
    msg[7] = (unsigned char)data_druh_dreva;
    msg[8] = (unsigned char)((data_operace >> 3)&1); /* jednou, dvakrát */
    msg[9] = (unsigned char)((data_operace >> 1)&3); /* další operace */
    msg[10]= (unsigned char)((data_operace)&1); /* manipulace */  
    id_counter++;
    return msg;
  }
  else{
    return NULL;
  }
}

/**
 * Tato zpráva je odesílána jako poslední po jejím pøijetí na stranì
 * servru dojde k neètení dat do databáze. Po pøíjmu jejího potvrzení zde jsou
 * smazány záznamy.
 */
char * over_msg(void){
    char * msg;
    msg =(void*) mmalloc (LenOfMsg);

    msg[0] = '\x43';
    return msg;
}

