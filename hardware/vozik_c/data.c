/* $Id: data.c 115 2007-09-11 14:12:58Z Mira $
 */
/**
 * @file data.c
 * @brief
 * Modul spravuje nam��en� hodnoty strom� a jm�na z�kazn�k�.
 *
 * Vzhledem k nedostatku pam�ti je nutn� maxim�ln� �et�it.
 * Z toho d�vodu jsou z�znamy ulo�eny na 3.5 Bytu. 
 */

/*
 * Zp�sob ulo�n� dat:
 * B0_1:B0_7 - pr�m�r
 * B1_0:B0_0 - d�lka
 * B2_3:B2_7 - z�kazn�k
 * B2_0:B2_2 - druh d�eva
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

///@todo odstranit includov�n� lcd.c

/// 
#define DELKA_POLE_DAT 105
/// Datov� pole, kam se ukl�danj� nam��en� data.
char data[DELKA_POLE_DAT][7];
/// Ukazuje na posledn� z�znam + 1.
int16_t data_top = 0;
/// Ukazuje na aktu�ln� z�znam
char data_akt_zakaznik = 0;

uint16_t data_delka = 0;
char data_zakaznik = 0;
uint16_t data_prumer = 0;
char data_druh_dreva  = 0;
char data_operace = 0;  // jednou, dvakr�t  // manipulace, nemanipulace //nic, s�movka, jin� pila




/**
 * Funkce odstan� v�echny z�znamy dat a jm�na z�kazn�k�. 
 */
void dat_del_all(void){
  data_top = 0;
  dat_free_m_jmena();
  dat_free_m_jmena();
}


/// Funkce sma�e z�znam @a id
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

/// Funkce ulo�� data do eeprom a na v�stupu se objev� prvn� voln� adresa v eprom za uk�dan�mi daty.
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
}

/// Ulo�� z�znam do pam�ti. 
void new_record(char zakaznik,char druh_dreva,char operace,uint16_t delka, uint16_t prumer){
  w_record(data_top,zakaznik,druh_dreva,operace,delka, prumer);
  data_top++;
}

/**
 * Funkce ukl�d� do datov�ho pole z�znam. Z�znam je ulo�en na 3.5 bytech.
 * P�lbajty z posob� jsouc�ch z�znam� jsou ulo�eny do jednoho bytu.
 */
void w_record(uint16_t zaznam, char zakaznik, char druh_dreva, char operace, uint16_t delka, uint16_t prumer){
  char posun = ((zaznam & 0x01) * 4) ; // pokud je cislo zaznamu sud� bude to nula, pokud lich�, bude to �ty�i
  zaznam = zaznam >> 1;



  // p�epo�et - ukl�d� zaokrouhlen� na 5 cm
  delka = (delka + 2) / 5; // ta plus trojka je tam aby to zaokrouhlovalo
  if (delka > 511){
    new_error(89); //prekrocen rozsah delky
    delka = 511;
  }
  // ukl�d� zaokrouhlen� na 5 mm
  prumer = (prumer + 2) / 5;
  if (prumer > 127){
    new_error(90); //prekrocen rozsah prumeru
    prumer = 127;
  }

  //pozor pokud bude d�lka v�t�� ne� 512, bude doch�zet k p�episov�n� �daje o pr�m�ru
  data[zaznam][0 + posun] = (prumer << 1) | ((delka >> 8)&1); //prvnich 7 bit� je pr�m�r, poslen� bit(0) pat�� k d�lce
  data[zaznam][1 + posun] = (255 & delka);
  data[zaznam][2 + posun] = (zakaznik << 3)|(druh_dreva);
  data[zaznam][3] = (data[zaznam][3] & (0xf0 >> posun)) | (operace << posun);
  
}

/**
 * Funkce na�te z�znam s id @a zaznam. Data tohoto z�znamu jsou na�teny do
 * glob�ln�ch prom�nn�ch data_prumer , data_delka , data_zakaznik
 * data_druh_dreva , data_operace.
 */
void r_record(uint16_t zaznam){
  char posun = ((zaznam & 0x01) * 4) ; // pokud je cislo zaznamu sud� bude to nula, pokud lich�, bude to �ty�i
  //char a , b;
  zaznam = zaznam >> 1;
  
 // b = data[zaznam][1 + posun];

  data_prumer = (data[zaznam][0 + posun] >> 1) * 5;
  data_delka = (((data[zaznam][0 + posun] & 1) * 256) + data[zaznam][1 + posun]) * 5; 
  
  data_zakaznik = (data[zaznam][2 + posun] >> 3);
  data_druh_dreva = data[zaznam][2 + posun] & 0x07;
  data_operace = (data[zaznam][3] >> posun) & 0x0f;
  
}

/// Funkce na�te d�lku ze z�znamu s id @a zaznam.
/// @param zaznam ID z�znamu.
/// @return Vrac� d�lku.
uint16_t r_rec_delka(uint16_t zaznam) {
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sud� bude to nula, 
                                        // pokud lich�, bude to �ty�i
  zaznam = zaznam >> 1;
  
  return (((data[zaznam][0 + posun] & 1) * 256) + data[zaznam][1 + posun]) * 5; 
  
}

/// Funkce vrac� pr�m�r.
uint16_t r_rec_prumer(uint16_t zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sud� bude to nula, 
                                        // pokud lich�, bude to �ty�i
  zaznam = zaznam >> 1;

  return (data[zaznam][0 + posun] >> 1) * 5;
}

/// Vrac� id z�kazn�ka.
char r_rec_zakaznik(uint16_t zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sud� bude to nula, 
                                        // pokud lich�, bude to �ty�i
  zaznam = zaznam >> 1;

  return (data[zaznam][2 + posun] >> 3);
}


/// Funkce vrac� druh d�eva.
char r_rec_druh_dreva(uint16_t zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sud� bude to nula, 
                                        // pokud lich�, bude to �ty�i
  zaznam = zaznam >> 1;

  return  data[zaznam][2 + posun] & 0x07;
}

/// Funkce vrac� operaci. Je zde zak�dov�n druh dal�� operace, manipulace.
char r_rec_operace(uint16_t zaznam){
  char posun = ((zaznam & 0x01) * 4) ;  // pokud je cislo zaznamu sud� bude to nula, 
                                        // pokud lich�, bude to �ty�i
  zaznam = zaznam >> 1;

  return  (data[zaznam][3] >> posun) & 0x0f;
}

/// Funkce nalezne z�znam ur�it�ho z�kazn�ka.
/// Krom� toho jsou do "ve�ejn�ch" prom�nn�ch na�teny data z tohoto posledn�ho
/// z�znamu.
/// @param zakaznik Vstupem je ��slo z�kazn�ka.
/// @return V�stupem je ��slo m��en�.
///
/// @return V p��pad�, �e ��dn� z�znam nebyl nalezen, vrac� -1
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

// Funkce ulo�� daata do eeprom
//ukl�d� do pevn� d�lky, //TODO �lo by p�epsat na ukl�d�n� do prom�nn� d�lky
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


// pozor venku mus� b�t o�et�eno, zda je nen� na dan� jm�no 
// odkaz od nam��en�ch hodnot
void dat_odstran_zakaznika(int cislo){
  //int i;
//  char *c = ;
  free((void *)m_jmena[cislo]);
  m_jmena[cislo] = NULL;
}

/// P�id� do seznamu z�kazn�k� nov�ho z�kazn�ka.
char dat_add_zakaznik(char * jmeno){
  //int len = menu[0] -> len;
  int i=0;

  while ((m_jmena[i] != NULL) && (i < (MAX_ZAKAZNIK - 1))){
    i++;
  }

  m_jmena[i] = jmeno; // pokud nebude venku o�et�eno maxim�ln� p�id�n�,                     
                      // bude se p�episovat posledn� hodnota

  if (i == (MAX_ZAKAZNIK - 1)){// posledn� a m�me plno
  //  m_jmena[MAX_ZAKAZNIK - 1] = jmeno;//menu_pridat;
  return MAX_ZAKAZNIK - 1;
  }
  return (char)i;
}


/* end of data.c */


