/* $Id: data.c 115 2007-09-11 14:12:58Z Mira $
 */
/**
 * @file data.c
 * @brief
 * Modul spravuje namìøené hodnoty stromù a jména zákazníkù.
 *
 * Vzhledem k nedostatku pamìti je nutné maximálnì šetøit.
 * Z toho dùvodu jsou záznamy uloženy na 3.5 Bytu. 
 */

/*
 * Zpùsob uložní dat:
 * B0_1:B0_7 - prùmìr
 * B1_0:B0_0 - délka
 * B2_3:B2_7 - zákazník
 * B2_0:B2_2 - druh døeva
 * B3_0:B3_3 - operace
 */


#include <inttypes.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
//#include <util/delay.h>
#include "data.h"
#include "eeprom.h"
#include "error.h"
#include "main.h"
#include "lcd.h" 
#include "gui.h" 

///@todo odstranit includování lcd.c

/// 
#define DELKA_POLE_DAT 105
/// Datové pole, kam se ukládanjí namìøená data.
char data[DELKA_POLE_DAT][7];
/// Ukazuje na poslední záznam + 1.
int16_t data_top = 0;
/// Ukazuje na aktuální záznam
char data_akt_zakaznik = 0;

uint16_t data_delka = 0;
char data_zakaznik = 0;
uint16_t data_prumer = 0;
char data_druh_dreva  = 0;
char data_operace = 0;  // jednou, dvakrát  // manipulace, nemanipulace //nic, sámovka, jiná pila




/**
 * Funkce odstaní všechny záznamy dat a jména zákazníkù. 
 */
void dat_del_all(void){
  data_top = 0;
  dat_free_m_jmena();
  dat_free_m_jmena();
}


/// Funkce smaže záznam @a id
void dat_del_record(uint16_t id){
  uint16_t i = 0;

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

/// Funkce uloží data do eeprom a na výstupu se objeví první volná adresa v eprom za ukádanými daty.
void *dat_write_eeprom(void *kam){
  eeprom_busy_wait();
  eeprom_write_block((void *)&(data[0]),kam, DELKA_POLE_DAT * 7);
  eeprom_busy_wait();
  eeprom_w16((void *) kam + 1 + (7 * DELKA_POLE_DAT), data_top);

  return (void *) kam + 1 + (7 * DELKA_POLE_DAT) + 2;
}

/// Naète data z EEPROM.
void *dat_read_eeprom(void * odkud){
  eeprom_read_block((void *)&(data[0]),odkud , DELKA_POLE_DAT * 7);
  data_top = eeprom_r16(odkud + 1 + (7 * DELKA_POLE_DAT));
  return (void *)odkud + 1 + (7 * DELKA_POLE_DAT) + 2;
}

/// Uloží záznam do pamìti. 
void new_record(char zakaznik,char druh_dreva,char operace,uint16_t delka, uint16_t prumer){
  w_record(data_top,zakaznik,druh_dreva,operace,delka, prumer);
  data_top++;
}

/**
 * Funkce ukládá do datového pole záznam. Záznam je uložen na 3.5 bytech.
 * Pùlbajty z posobì jsoucích záznamù jsou uloženy do jednoho bytu.
 */
void w_record(uint16_t zaznam, char zakaznik, char druh_dreva, char operace, uint16_t delka, uint16_t prumer){
  char posun = ((zaznam & 0x01) * 4) ; // pokud je cislo zaznamu sudé bude to nula, pokud liché, bude to ètyøi
  zaznam = zaznam >> 1;



  // pøepoèet - ukládá zaokrouhlené na 5 cm
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

  //pozor pokud bude délka vìtší než 512, bude docházet k pøepisování údaje o prùmìru
  data[zaznam][0 + posun] = (prumer << 1) | ((delka >> 8)&1); //prvnich 7 bitù je prùmìr, poslení bit(0) patøí k délce
  data[zaznam][1 + posun] = (255 & delka);
  data[zaznam][2 + posun] = (zakaznik << 3)|(druh_dreva);
  data[zaznam][3] = (data[zaznam][3] & (0xf0 >> posun)) | (operace << posun);
  
}

/**
 * Funkce naète záznam s id @a zaznam. Data tohoto záznamu jsou naèteny do
 * globálních promìnných data_prumer , data_delka , data_zakaznik
 * data_druh_dreva , data_operace.
 */
void r_record(uint16_t zaznam){
  char posun = ((zaznam & 0x01) * 4) ; // pokud je cislo zaznamu sudé bude to nula, pokud liché, bude to ètyøi
  //char a , b;
  zaznam = zaznam >> 1;
  
 // b = data[zaznam][1 + posun];

  data_prumer = (data[zaznam][0 + posun] >> 1) * 5;
  data_delka = (((data[zaznam][0 + posun] & 1) * 256) + data[zaznam][1 + posun]) * 5; 
  
  data_zakaznik = (data[zaznam][2 + posun] >> 3);
  data_druh_dreva = data[zaznam][2 + posun] & 0x07;
  data_operace = (data[zaznam][3] >> posun) & 0x0f;
  
}

/// Funkce naète délku ze záznamu s id @a zaznam.
/// @param zaznam ID záznamu.
/// @return Vrací délku.
uint16_t r_rec_delka(uint16_t zaznam) {
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sudé bude to nula, 
                                        // pokud liché, bude to ètyøi
  zaznam = zaznam >> 1;
  
  return (((data[zaznam][0 + posun] & 1) * 256) + data[zaznam][1 + posun]) * 5; 
  
}

/// Funkce vrací prùmìr.
uint16_t r_rec_prumer(uint16_t zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sudé bude to nula, 
                                        // pokud liché, bude to ètyøi
  zaznam = zaznam >> 1;

  return (data[zaznam][0 + posun] >> 1) * 5;
}

/// Vrací id zákazníka.
char r_rec_zakaznik(uint16_t zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sudé bude to nula, 
                                        // pokud liché, bude to ètyøi
  zaznam = zaznam >> 1;

  return (data[zaznam][2 + posun] >> 3);
}


/// Funkce vrací druh døeva.
char r_rec_druh_dreva(uint16_t zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sudé bude to nula, 
                                        // pokud liché, bude to ètyøi
  zaznam = zaznam >> 1;

  return  data[zaznam][2 + posun] & 0x07;
}

/// Funkce vrací operaci. Je zde zakódován druh další operace, manipulace.
char r_rec_operace(uint16_t zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sudé bude to nula, 
                                        // pokud liché, bude to ètyøi
  zaznam = zaznam >> 1;

  return  (data[zaznam][3] >> posun) & 0x0f;
}

/// Funkce nalezne záznam urèitého zákazníka.
/// Kromì toho jsou do "veøejných" promìnných naèteny data z tohoto posledního
/// záznamu.
/// @param zakaznik Vstupem je èíslo zákazníka.
/// @return Výstupem je èíslo mìøení.
///
/// @return V pøípadì, že žádný záznam nebyl nalezen, vrací -1
int16_t find_last_record_of_zakaznik(char zakaznik){
  int16_t zaznam = data_top - 1;

  while (zaznam >= 0){
    if(r_rec_zakaznik(zaznam) == zakaznik){
      r_record((uint16_t) zaznam);
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
      free((void *) m_jmena[i]);
      m_jmena[i] = NULL;
    }
  }
  m_jmena[MAX_ZAKAZNIK] = menu_pridat;
}

// Funkce uloží daata do eeprom
//ukládá do pevné délky, //TODO šlo by pøepsat na ukládání do promìnné délky
void *dat_zak_write_eeprom(void *kam){
  int i;
  uint16_t prirustek = 0;
  
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
  uint16_t prirustek = 0;
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
        s = (char *)malloc(len + 1);
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


// pozor venku musí být ošetøeno, zda je není na dané jméno 
// odkaz od namìøených hodnot
void dat_odstran_zakaznika(int cislo){
  //int i;
//  char *c = ;
  free((void *)m_jmena[cislo]);
  m_jmena[cislo] = NULL;
}

/// Pøidá do seznamu zákazníkù nového zákazníka.
char dat_add_zakaznik(char * jmeno){
  //int len = menu[0] -> len;
  int i=0;

  while ((m_jmena[i] != NULL) && (i < (MAX_ZAKAZNIK - 1))){
    i++;
  }

  m_jmena[i] = jmeno; // pokud nebude venku ošetøeno maximální pøidání,                     
                      // bude se pøepisovat poslední hodnota

  if (i == (MAX_ZAKAZNIK - 1)){// poslední a máme plno
  //  m_jmena[MAX_ZAKAZNIK - 1] = jmeno;//menu_pridat;
  return MAX_ZAKAZNIK - 1;
  }
  return (char)i;
}


/* end of data.c */


