/* $Id: data.c 115 2007-09-11 14:12:58Z Mira $
 */
/**
 * @file data.c
 * @brief
 * Modul spravuje naměřené hodnoty stromů a jména zákazníků.
 *
 * Vzhledem k nedostatku paměti je nutné maximálně šetřit.
 * Z toho důvodu jsou záznamy uloženy na 3.5 Bytu. 
 */

/*
 * Způsob uložní dat:
 * B0_1:B0_7 - průměr
 * B1_0:B0_0 - délka
 * B2_3:B2_7 - zákazník
 * B2_0:B2_2 - druh dřeva
 * B3_0:B3_3 - operace
 */


#include <inttypes.h>
//#include <avr/eeprom.h>
#include <stdio.h>
//#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
//#include <util/delay.h>
#include "data.h"
/*
#include "eeprom.h"
*/
#include "error.h"
#include "main.h"
#include "lcd.h" 
#include "gui.h"
#include "data_file.h"
#include "memory.h"
#include "msr_scr.h"



        ///@todo odstranit includování lcd.c

/* globalni proměnné */

void new_record(char zakaznik,char druh_dreva,char operace,unsigned int delka, unsigned int prumer);
void w_record(unsigned int zaznam, char zakaznik, char druh_dreva, char operace, unsigned int delka,  unsigned int prumer);
void r_record(unsigned int zaznam);

int find_last_record_of_zakaznik(char zakaznik);

void dat_odstran_zakaznika(int cislo);
char dat_add_zakaznik(char * jmeno);

void dat_del_all(void);
void dat_del_record(unsigned int id);

void dat_free_m_jmena(void);


void dat_load(void);
int dat_prvni_zakaznik(void);

unsigned int data_delka;
char data_zakaznik;
unsigned int data_prumer;
char data_druh_dreva;
char data_operace;
char * m_jmena[MAX_ZAKAZNIK + 1];
unsigned char data[DELKA_POLE_DAT][7];

/// Ukazuje na posledn� z�znam + 1.
uint8_t data_top = 0;


/* end - globální proměnné*/

/// Ukazuje na aktu�ln� z�znam
char data_akt_zakaznik = 0;

unsigned int data_delka = 0;
char data_zakaznik = 0;
unsigned int data_prumer = 0;
char data_druh_dreva  = 0;
char data_operace = 0;  // jednou, dvakr�t  // manipulace, nemanipulace //nic, s�movka, jin� pila




/**
 * Funkce odstaní všechny záznamy dat a jména zákazníků. 
 */
void dat_del_all(void){
  data_top = 0;
  dat_free_m_jmena();
  dat_free_m_jmena();
}


/// Funkce smaže záznam @a id
void dat_del_record(unsigned int id){
  unsigned int i = 0;

  if (id >= data_top){
    new_error(10);
    return;
  }

  for (i = id; i < (data_top - 1); i++){
    r_record(i + 1);
    w_record((unsigned int) i ,data_zakaznik, data_druh_dreva, data_operace, data_delka, data_prumer);
  }
  data_top--;
}


void dat_save(void){
  datf_save();

}

void dat_load(void){
  datf_load();

}
/// Funkce uloží data do eeprom a na výstupu se objeví první volná adresa v eprom za ukádanými daty.
/*
void *dat_write_eeprom(void *kam){
  eeprom_busy_wait();
  eeprom_write_block((void *)&(data[0]),kam, DELKA_POLE_DAT * 7);
  eeprom_busy_wait();
  eeprom_w16((void *) kam + 1 + (7 * DELKA_POLE_DAT), data_top);

  return (void *) kam + 1 + (7 * DELKA_POLE_DAT) + 2;
}

/// Na�te data z EEPROM.
void *dat_read_eeprom(void * odkud){
  eeprom_read_block((void *)&(data[0]),odkud , DELKA_POLE_DAT * 7);
  data_top = eeprom_r16(odkud + 1 + (7 * DELKA_POLE_DAT));
  return (void *)odkud + 1 + (7 * DELKA_POLE_DAT) + 2;
}*/

/// Uloží záznam do paměti. 
void new_record(char zakaznik,char druh_dreva,char operace,unsigned int delka, unsigned int prumer){
  w_record(data_top,zakaznik,druh_dreva,operace,delka, prumer);
  data_top++;
  dat_save();
}

/**
 * Funkce ukládá do datového pole záznam. Záznam je uložen na 3.5 bytech.
 * Půlbajty z posobě jsoucích záznamů jsou uloženy do jednoho bytu.
 */
void w_record(unsigned int zaznam, char zakaznik, char druh_dreva, char operace, unsigned int delka, unsigned int prumer){
  char posun = ((zaznam & 0x01) * 4) ; // pokud je cislo zaznamu sud� bude to nula, pokud lich�, bude to �ty�i
  zaznam = zaznam >> 1;



  // přepočet - ukládá zaokrouhlené na 5 cm
  delka = (delka + 2) / 5; // ta plus trojka je tam aby to zaokrouhlovalo
  if (delka > 511){
    new_error(89); //prekrocen rozsah delky
    delka = 511;
  }
  // ukládá zaokrouhlené na 5 mm
  prumer = (prumer + 2) / 5;
  if (prumer > 127){
    new_error(90); //prekrocen rozsah prumeru
    prumer = 127;
  }

  //pozor pokud bude délka větší než 512, bude docházet k přepisování údaje o průměru
  data[zaznam][0 + posun] = (prumer << 1) | ((delka >> 8)&1); //prvnich 7 bitů je průměr, poslení bit(0)  pat�� k d�lce
  data[zaznam][1 + posun] = (255 & delka);
  data[zaznam][2 + posun] = (zakaznik << 3)|(druh_dreva);
  data[zaznam][3] = (data[zaznam][3] & (0xf0 >> posun)) | (operace << posun);

  dat_save();
}

/**
 * Funkce načte záznam s id @a zaznam. Data tohoto záznamu jsou načteny do
 * globálních proměnných data_prumer , data_delka , data_zakaznik
 * data_druh_dreva , data_operace.
 */
void r_record(unsigned int zaznam){
  char posun = ((zaznam & 0x01) * 4) ;// pokud je cislo zaznamu sudé bude to nula, pokud liché, bude to čtyři
  //char a , b;
//  int a,b,c;
  zaznam = zaznam >> 1;
  
 // b = data[zaznam][1 + posun];

  data_prumer = (data[zaznam][0 + posun] >> 1) * 5;

  data_delka = (((data[zaznam][0 + posun] & 1) * 256) + data[zaznam][1 + posun]) * 5; 
  //a = (data[zaznam][0 + posun] & 1); //@todo odstranit
  //b = (data[zaznam][1 + posun]) * 5;
  //c = a*256 + b;
  data_zakaznik = (data[zaznam][2 + posun] >> 3);
  data_druh_dreva = data[zaznam][2 + posun] & 0x07;
  data_operace = (data[zaznam][3] >> posun) & 0x0f;
  
}

/// Funkce načte délku ze záznamu s id @a zaznam.
/// @param zaznam ID záznamu.
/// @return Vrací délku.
unsigned int r_rec_delka(unsigned int zaznam) {
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sudé bude to nula,
                                        // pokud liché, bude to čtyři
  zaznam = zaznam >> 1;
  
  return (((data[zaznam][0 + posun] & 1) * 256) + data[zaznam][1 + posun]) * 5; 
  
}

/// Funkce vrací průměr.
unsigned int r_rec_prumer(unsigned int zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sudé bude to nula, 
                                        // pokud liché, bude to čtyři
  zaznam = zaznam >> 1;

  return (data[zaznam][0 + posun] >> 1) * 5;
}

/// Vrací id zákazníka.
char r_rec_zakaznik(unsigned int zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sudé bude to nula, 
                                        // pokud liché, bude to čtyři
  zaznam = zaznam >> 1;

  return (data[zaznam][2 + posun] >> 3);
}


/// Funkce vrací druh dřeva.
char r_rec_druh_dreva(unsigned int zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sud� bude to nula, 
                                        // pokud lich�, bude to �ty�i
  zaznam = zaznam >> 1;

  return  data[zaznam][2 + posun] & 0x07;
}

/// Funkce vrací operaci. Je zde zakódován druh další operace, manipulace.
char r_rec_operace(unsigned int zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sud� bude to nula, 
                                        // pokud lich�, bude to �ty�i
  zaznam = zaznam >> 1;

  return  (data[zaznam][3] >> posun) & 0x0f;
}

/// Funkce nalezne záznam určitého zákazníka.
/// Kromě toho jsou do "veřejných" proměnných načteny data z tohoto posledního
/// záznamu.
/// @param zakaznik Vstupem je číslo zákazníka.
/// @return Výstupem je číslo měření.
///
/// @return V případě, že žádný záznam nebyl nalezen, vrací -1
int find_last_record_of_zakaznik(char zakaznik){
  int zaznam = data_top - 1;

  while (zaznam >= 0){
    if(r_rec_zakaznik(zaznam) == zakaznik){
      r_record((unsigned int) zaznam);
      return zaznam;
    }
    zaznam--;
    
  }
  return -1;
}

/*----------------------------------*/

void dat_free_m_jmena(void){
  int i;
  for (i = 0 ; i < MAX_ZAKAZNIK; i++){
    if (m_jmena[i] != NULL){
      mfree((void *) m_jmena[i]);
      m_jmena[i] = NULL;
    }
  }
  m_jmena[MAX_ZAKAZNIK] = menu_pridat;
}

// Funkce uloží daata do eeprom
//ukládá do pevné délky, //TODO šlo by přepsat na ukládání do proměnné délky
/*
void *dat_zak_write_eeprom(void *kam){
  int i;
  unsigned int prirustek = 0;
  
  for (i = 0; i < MAX_ZAKAZNIK; i++){
    eeprom_busy_wait();
    if (m_jmena[i] != NULL){
      char len = 1 + strlen(m_jmena[i]);
      eeprom_write_block(m_jmena[i], kam + prirustek ,len);
      prirustek += len;
    }
    else{
      eeprom_write_block("",kam + prirustek,1);
      prirustek += 1;
    }
  }
  return (void *)kam + prirustek;

}


void *dat_zak_read_eeprom(void *kam){
  int i = 0;
  unsigned int prirustek = 0;
  int j = 0;


  dat_free_m_jmena();

  while (1){  
    str_buf[j] = eeprom_r8(kam + prirustek);
    //printc(str_buf[j]);                              /// @todo
    //for(q=0; q < 500; q++){  /// @todo
    //  _delay_ms(1);
    //}

    if(str_buf[j] == '\0') {
      char *s;
      char len = strlen(str_buf);
      if (len == 0 ){
        m_jmena[i] = NULL;
      }
      else{
        s = (char *)mmalloc(len + 1);
        if (s == NULL){
	  // printc('E');    /// @todo
          new_error(88);
	}
	else{
	  //printc('-');
	  strcpy(s,str_buf);
	  m_jmena[i] = s;
	}
      }
        //msr_scr_add_zakaznik(s);
      j = -1;
      i++;
      if (i >= MAX_ZAKAZNIK){
        break;
      }
    }
    j++;
    if (j > 12)
      break;
    prirustek++;
  }
  return (void *) kam + prirustek;
}
*/

// pozor venku musí být ošetřeno, zda je není na dané jméno 
// odkaz od naměřených hodnot
void dat_odstran_zakaznika(int cislo){
  //int i;
//  char *c = ;
  mfree((void *)m_jmena[cislo]);
  m_jmena[cislo] = NULL;
  dat_save();
}

/// Přidá do seznamu zákazníků nového zákazníka.
char dat_add_zakaznik(char * jmeno){
  //int len = menu[0] -> len;
  int i=0;

  while ((m_jmena[i] != NULL) && (i < (MAX_ZAKAZNIK - 1))){
    i++;
  }

  m_jmena[i] = jmeno; // pokud nebude venku o�et�eno maxim�ln� p�id�n�,                     
                      // bude se p�episovat posledn� hodnota

  dat_save();
  if (i == (MAX_ZAKAZNIK - 1)){// posledn� a m�me plno
  //  m_jmena[MAX_ZAKAZNIK - 1] = jmeno;//menu_pridat;
  return MAX_ZAKAZNIK - 1;
  }
  return (char)i;
}


/**
 * Funkce frátí index prvního zákazníka. Pokud žádný zákazník není, vrací se -1.
 */
int dat_prvni_zakaznik(void){
  int i;
  for (i = 0; i < MAX_ZAKAZNIK; i++) {
    if (m_jmena[i] != NULL){
      return i;
      break;
    }
  }
  return -1;

}

/* end of data.c */


