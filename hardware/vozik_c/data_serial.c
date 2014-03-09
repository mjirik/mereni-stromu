/* $Id: data_serial.c 119 2007-09-20 14:05:51Z Mira $ 
 */
/**
 * @file data_serial.c
 * @brief
 * V tomto modulu je �e�ena synchrinizace dat mezi datab�z� na serveru a
 * daty v jedno�ipu prost�ednictv�m s�riov� linky.
 * Po zah�jen� odes�l�n� je nejprve vzsl�na zpr�va pro voz�k. Ta zastav� jeho vys�l�n�.
 * Pak je zasl�na inisializa�n� zpr�va na server. T�m dojde k p��prav� datab�ze.
 * P��prava spo��v� v tom, �e syst�m pova�uje v�echna data od
 * p��chodu kone�n� zpr�vy minul�ho p��jmu a� do t�to inicializa�n� zpr�vy
 * pova�uj� za "z�lo�n�".
 * Pak je zah�jeno odes�l�n� jednotliv�ch jmen z�kazn�k�. Po odesl�n� v�ech
 * jsou odes�l�na data nam��en�ch strom�. V z�v�ru je odesl�na uko�ovac� zpr�va.
 * Ta zp�sob� p�esun dat z do�asn� datab�ze do datab�ze s n� ji� pracuje
 * webov� rozhran�. T�m je komunikace se serverem ukon�ena. Posled� odes�lanou
 * zpr�vou je znovuobnoven� vys�l�n� voz�ku.
 * Na z�v�r jsou vymaz�na data z datab�ze v provozu.
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

/* Je t�eba hl�dat p�i zm�n� velikosti zpr�vy v usart.c */
//#define DATA 10

/// Po�et opakov�n� zpr�v p�i odes�l�n� dat.
#define MSG_REPEATS 2

//static char server_data_buffer[DATA + 1];
static unsigned int id_counter = 0;
static unsigned char stav = 0;

static char * name_msg(void);
static char * mereni_msg(void);
static char * over_msg(void);
static void err91(void);


/**
 * Funkce je vol�na v p��pad� ne�sp�n�ho �ek�n� na odpov��. Vyp�e
 * chybovou hl�ku a obnov� komunikaci se za��zen�m na katru.
 */
static void err91(void){
  char * msg;
  new_error(91);
  msg = over_msg();
  new_msg(msg);

}

/** Funkce zahajuje odesl�l�n� dat */
void send_data(void){
  id_counter = 0;
  stav = 1;
  
  send_next_data(NULL);
  //if (name_msg() == 1){
  //new_ack_msg(server_data_buffer,send_next_data);
  //}
}

/**
 * Funkce ode�le dal�� data.
 * Je vol�na z venku od seriov� linky.
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

}

/**
 * Funkce sestav� zpr�vu kter� obsahuje jm�no z�kazn�ka a jeho id.
 * Tato zpr�va je ulo�ena do server_data_buffer
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
 * Funkce vytvo�� zpr�vu, kter� obsahuje nam��en� data. Zpr�va je ulo�ena
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
    msg[8] = (unsigned char)((data_operace >> 3)&1); /* jednou, dvakr�t */
    msg[9] = (unsigned char)((data_operace >> 1)&3); /* dal�� operace */
    msg[10]= (unsigned char)((data_operace)&1); /* manipulace */  
    id_counter++;
    return msg;
  }
  else{
    return NULL;
  }
}

/**
 * Tato zpr�va je odes�l�na jako posledn� po jej�m p�ijet� na stran�
 * servru dojde k ne�ten� dat do datab�ze. Po p��jmu jej�ho potvrzen� zde jsou
 * smaz�ny z�znamy.
 */
char * over_msg(void){
    char * msg;
    msg =(void*) mmalloc (LenOfMsg);

    msg[0] = '\x43';
    return msg;
}

