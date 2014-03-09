/* $Id: measure.c 120 2007-09-21 05:43:42Z Mira $
 */
/**
 * @file measure.c
 * @brief
 * Modul slou�� k obstar�n� funkc� pro m��en�.
 * 
 * M��en� je zahajov�no obsluhou. Stiskunt� tla��tka zavol� funkci msr_start().
 * Je zaznamen�ne d�lka kmene. Z t� jsou vypo�teny vzd�lenosti d�l��ch m��en�.
 * Polohy nejkrajn�j��ch m��en� jsou d�ny prom�nn�mi MIN_VZDAL_OD_KATRU a
 * MIN_VZDAL_OD_VOZIKU. Mezi nimi je (v�etn� krajn�ch bod�) zaznamen�no tolik
 * vzork�, kolik uv�d� POCET_MERENI. 
 * Funkce measure() je vol�na z hlavn�ho programu v nekone�n� sy�ce. V p��pad�
 * dosa�en� m���c�ho bodu, se star� o zaznamen�n� hodnot pr�m�r�. Pokud
 * za�ne voz�k couvat od katru o v�ce ne� BACK_MOVE_TOLERANCE cm, je m��en�
 * p�eru�eno. Pokud je nam��eno dostatek (MIN_POCET_MERENI) m��en�, jsou
 * data zpracov�na bezeslov. Pokud je m��en� m�n�, je vyps�no varov�n� o
 * mo�n� nep�esnosti dat. Jestli�e nebylo je�t� dosa�eno ani
 * jedin�ho m���c�ho bodu, je m��en� zru�eno (msr_storno()).
 * Zpracov�n� nam��en�ch dat prob�h� tak, �e jsou nem��en�mi hodnotami
 * pr�m�ru na v��ku i na ���ku prolo�eny p��mky. Do rovnice p��mek je
 * pak dosazena hodnota poloviny d�lky m��en�ho stromu a je tak z�sk�n odhad
 * p��slu�n�ch pr�m�r�.
 *
 * F�ze m��en� je indikov�na v��tovou prom�nnou msr_status. Na jej�m z�klad�
 * pak prob�h� ukl�d�n� dat v msr_scr.c 
 */


#include <inttypes.h> 
#include <avr/io.h>
#include "measure.h"
#include "irc.h"
#include "error.h"
#include "lcd.h"
#include "convert.h"
#include "data.h"

#define POCET_MERENI 8
#define MIN_POCET_MERENI 4
#define MIN_VZDAL_OD_KATRU 50 //cm
#define MIN_VZDAL_OD_VOZIKU 150
#define BACK_MOVE_TOLERANCE 50 //cm
#define MAX_INT32 2147483638

/// pr�m�r kter�m je spu�t�no m��en� d�lky po stisknut� tla��tka START
#define MIN_START_PRUMER 10

/** @name Pin indikace stavu
 * Prom�nn� definuj� um�st�n� pinu, je� zaji��uje indikaci stavu (msr_status)
 * @todo zjistit spr�vn� pin
 * @{
 */
#define INDIKACE_PORT PORTB
#define INDIKACE_DDR DDRB
#define INDIKACE_PIN PINB
#define INDIKACE_BIT 3
/** @} */

static void vypocti_vzdalenosti(void);
static void msr_end(void);
inline void indikace_stavu_control(void);

//char msr_run = 0;
//static char last_start = 0;
uint16_t msr_delka = 0;
uint16_t msr_prumer = 0;
uint16_t msr_objem = 0;
//static uint16_t pulka = 0;
static uint16_t mereni_d_v[POCET_MERENI];
static uint16_t mereni_d_h[POCET_MERENI];
static uint16_t mereni_l[POCET_MERENI];
static uint8_t n_mereni = POCET_MERENI;
// Pokud budu cht�t zm�nit po�et m��en�, tak 
//static float skok_mereni = (float)1.0/(POCET_MERENI + 2);


/// Pou��v� se pro kontrolu, zda voz�k u� neodj�d� od katru.
static uint16_t min_vzdal = 16000;//MAX_INT32; // max int32

STAV_MERENI msr_status = VOLNO;

/// Ur�uje, zda je odkud je br�n horizont�ln� pr�m�r kmene.
/// Pokud je 0, pr�m�r nen� m��en, pokud je 1, je m��en za��zen�m na katru,
/// je-li 2, jsou data p�iv�d�na ze serveru (mo�n� roz���en�).
unsigned char width_src = 1;


/**
 * Funkce zaji��uje spr�vn� zah�jen� m���c�ho procesu. V p��pad�, �e bylo m��en�
 * pozastaveno, dojde k obnov�.
 */
void msr_start(void){
  switch(msr_status){
  case VOLNO:
  case ZMERENY_PRUMERY:
    // osetreni naplneni pameti
    if (data_top >= (DATA_MAX_POCET_ZAZNAMU - 2)){
      new_error(101); // pam� mereni plna
    }
    else{
      if (data_top >= (DATA_MAX_POCET_ZAZNAMU - 11)){
        new_error(201);
      }
      msr_status = L_NEZMERENO;
    }
    break;
  case L_NEZMERENO:
  case L_ZMERENO:
    new_error(84);
    break;
  default:
    break;
  }
}

/**
 * Funkce p�eru�� m��en�. 
 */
void msr_storno(void){
  msr_status = VOLNO;
  n_mereni = 0;
}

void msr_pause(void){
  msr_status = (msr_status + 3);
}

void msr_unpause(void){
  msr_status = (msr_status - 3);
  min_vzdal = irc_get_l();
}

void msr_zmer_l (void){
  msr_delka = irc_get_l();
  if (msr_delka > (MIN_VZDAL_OD_VOZIKU + MIN_VZDAL_OD_KATRU)){
    //  n_mereni = POCET_MERENI;
    //pristi_mereni = (uint16_t) (skok_mereni * msr_delka * (n_mereni + 1));
    n_mereni = 0;
    vypocti_vzdalenosti();
    
    msr_status = L_ZMERENO;
    min_vzdal = msr_delka;
  }
  else{
    new_error(99);
    msr_storno();
  }
}


/// Funkce p�ed zapo�et�m m��en� ur�� vzd�lenosti, ve kter�ch dojde k m��en�
static void vypocti_vzdalenosti(void){
  int i;
  for (i = 0; i < POCET_MERENI; i++){
    mereni_l[i] = msr_delka - MIN_VZDAL_OD_KATRU - 
    (i * ((float)(1.0 / (POCET_MERENI - 1))) * (msr_delka - (MIN_VZDAL_OD_KATRU + MIN_VZDAL_OD_VOZIKU)));
  }
}

/// Funkce pr�m�uje hodnoty pole
uint16_t prumeruj(uint16_t pole[], uint8_t len){
  int i;
  uint32_t suma = 0;
  for (i = 0; i < len; i++){
    suma += pole[i];
  }
  return (suma / len);
}


/// V�po�et metody line�rn� regrese.
/// @returns Vrac� hodnotu k
float get_k(uint16_t pole_d[],uint16_t pole_l [], uint8_t len){
  int i;
  float l_str = 0;  
  float citatel = 0; // \sum (I - \overline{I}) F_I
  float jmenovatel = 0; // \sum( (I - \overline{I})^2 )

  if (len < 2){			/* Osetreni nizkeho poctu mereni */
    return 0;
  }

  
  for (i = 0; i < len; i++){
    l_str += pole_l[i];
  }

  l_str = l_str / len; // st�edn� hodnota l
  
  for (i = 0; i < len; i++){
    citatel += (pole_l[i] - l_str) * pole_d[i];
    jmenovatel += (pole_l[i] - l_str) * (pole_l[i] - l_str);
  }
  return (float) (((float)citatel) / jmenovatel);
}

float get_q(uint16_t pole_d[],uint16_t pole_l [], uint8_t len, float k){
  int i;
  float l_str = 0;  
  float d_str = 0;

  if (len < 1){
    return 0;
  }

  for (i = 0; i < len; i++){
    l_str += pole_l[i];
    d_str += pole_d[i];
  }
  l_str = l_str / len; // st�edn� hodnota l
  d_str = d_str / len;

  return d_str - (k * l_str);
}


void measure(void) {
  //msr_pocet_mereni = n_mereni;
  switch (msr_status){
  case L_NEZMERENO:
    if (width_src == 1){
      if (prumer_h > MIN_START_PRUMER){
        msr_zmer_l();
      }
    }
    else{
      msr_zmer_l();
    }
    break;
  case L_ZMERENO: 
    {
      uint16_t vzdalenost;
      vzdalenost = irc_get_l();
      
      if (min_vzdal > vzdalenost){ /* zji�tujeme, zda se voz�k u� nevrac� */
        min_vzdal = vzdalenost;
      }
      else{ // znamen� to, �e s voz�k pohnul sm�rem vzad
        if ((vzdalenost - min_vzdal) > BACK_MOVE_TOLERANCE){
          if (n_mereni < 2){
            new_error(93); // Mereni zruseno
            msr_storno();
          }
          else {
            if (n_mereni < MIN_POCET_MERENI){
              new_error(92);// varovani: mereni nepresne
              msr_end();
            }
            else { // jinak je mereni normalne ukoceno
              msr_end();
            }
          }
        }
      }
      if (vzdalenost < mereni_l[n_mereni]){ // pokud nastal okam�ik m��en� ...
        // ulo� okam�it� hodnoty do pole nam��en�ch dat
        mereni_d_v[n_mereni] = prumer_v;     //TODO
        mereni_d_h[n_mereni] = prumer_h;
        
        n_mereni++;
        if (n_mereni == POCET_MERENI){  // posledn� m��en�
          msr_end();
        }
      }
    }
    break;
  case PAUSE_L_NEZMERENO:
  case PAUSE_L_ZMERENO:
    
    break;
  default:
    break;
  }
  
  // indikace stavu sv�tlem
  indikace_stavu_control();
}

/// Prov�d� v�po�ty po ukon�en� m��en�.
void msr_end(void){
  float k1,q1,k2,q2;
  
  k1 = get_k(mereni_d_v, mereni_l, n_mereni);
  q1 = get_q(mereni_d_v, mereni_l, n_mereni, k1); 
  
  msr_prumer_v = (uint16_t) ((k1 * (msr_delka / 2.0)) + q1);
  
  k2 = get_k(mereni_d_h, mereni_l, n_mereni);
  q2 = get_q(mereni_d_h, mereni_l, n_mereni, k2); 
  
  msr_prumer_h = (uint16_t) ((k2 * (msr_delka / 2.0)) + q2);
  
  //msr_prumer = (uint16_t) ((((k1 * (msr_delka / 2.0)) + q1 +(k2 * (msr_delka / 2.0)) + q2)) / 2);
  switch (width_src){
  case 1:
  case 2:
    msr_prumer = (uint16_t) ((((k1 * (msr_delka / 2.0)) + q1 +(k2 * (msr_delka / 2.0)) + q2)) / 2);
    break;
  case 0:
    msr_prumer = (uint16_t) ((k1 * (msr_delka / 2.0)) + q1);
    break;
  default:
    break;
  }
  
  
  msr_objem = (uint32_t)(((3.1415 * (msr_prumer * msr_prumer)/400000.0) * msr_delka)); //00000000
  
  msr_status = ZMERENY_PRUMERY;
  n_mereni = 0;
}

/** 
 * Funkce m� za �kol zaji��ovat indikaci stavu prost�ednictv�m diody.
 * @todo dopsat  
 */
inline void indikace_stavu_control(void){
  static int casovac = 0;
  
  switch(msr_status){
  case L_NEZMERENO:
  case L_ZMERENO:
    INDIKACE_PORT |= _BV(INDIKACE_BIT); // jako I_PORT = I_PORT | (1 << I_BIT)
    break;
  case VOLNO:
  case ZMERENY_PRUMERY:
    INDIKACE_PORT &= ~_BV(INDIKACE_BIT); 
    break;
  case PAUSE_L_NEZMERENO:
  case PAUSE_L_ZMERENO:
    if (casovac < 4000){
      if (casovac < 2000)
        INDIKACE_PORT |= _BV(INDIKACE_BIT);
      else 
        INDIKACE_PORT &= ~_BV(INDIKACE_BIT); 
      casovac++;
    }
    else{
      casovac = 0;
    }
    break;
  default:
    break;
  }
}


void msr_init(void){
  INDIKACE_DDR |= _BV(INDIKACE_BIT);
}

/* nepot�ebn�
void set_width_src(unsigned char c){
  width_src = c;
  eeprom_w8((void *)ee_,c);
}*/
/* end of measure.c */

