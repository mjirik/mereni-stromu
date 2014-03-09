/* $Id: measure.c 120 2007-09-21 05:43:42Z Mira $
 */
/**
 * @file measure.c
 * @brief
 * Modul slouží k obstarání funkcí pro mìøení.
 * 
 * Mìøení je zahajováno obsluhou. Stiskuntí tlaèítka zavolá funkci msr_start().
 * Je zaznamenáne délka kmene. Z té jsou vypoèteny vzdálenosti dílèích mìøení.
 * Polohy nejkrajnìjších mìøení jsou dány promìnnými MIN_VZDAL_OD_KATRU a
 * MIN_VZDAL_OD_VOZIKU. Mezi nimi je (vèetnì krajních bodù) zaznamenáno tolik
 * vzorkù, kolik uvádí POCET_MERENI. 
 * Funkce measure() je volána z hlavního programu v nekoneèné syèce. V pøípadì
 * dosažení mìøícího bodu, se stará o zaznamenání hodnot prùmìrù. Pokud
 * zaène vozík couvat od katru o více než BACK_MOVE_TOLERANCE cm, je mìøení
 * pøerušeno. Pokud je namìøeno dostatek (MIN_POCET_MERENI) mìøení, jsou
 * data zpracována bezeslov. Pokud je mìøení ménì, je vypsáno varování o
 * možné nepøesnosti dat. Jestliže nebylo ještì dosaženo ani
 * jediného mìøícího bodu, je mìøení zrušeno (msr_storno()).
 * Zpracování namìøených dat probíhá tak, že jsou nemìøenými hodnotami
 * prùmìru na výšku i na šíøku proloženy pøímky. Do rovnice pøímek je
 * pak dosazena hodnota poloviny délky mìøeného stromu a je tak získán odhad
 * pøíslušných prùmìrù.
 *
 * Fáze mìøení je indikována výètovou promìnnou msr_status. Na jejím základì
 * pak probíhá ukládání dat v msr_scr.c 
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

/// prùmìr kterým je spuštìno mìøení délky po stisknutí tlaèítka START
#define MIN_START_PRUMER 10

/** @name Pin indikace stavu
 * Promìnné definují umístìní pinu, jež zajišuje indikaci stavu (msr_status)
 * @todo zjistit správný pin
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
// Pokud budu chtít zmìnit poèet mìøení, tak 
//static float skok_mereni = (float)1.0/(POCET_MERENI + 2);


/// Používá se pro kontrolu, zda vozík už neodjíždí od katru.
static uint16_t min_vzdal = 16000;//MAX_INT32; // max int32

STAV_MERENI msr_status = VOLNO;

/// Urèuje, zda je odkud je brán horizontální prùmìr kmene.
/// Pokud je 0, prùmìr není mìøen, pokud je 1, je mìøen zaøízením na katru,
/// je-li 2, jsou data pøivádìna ze serveru (možné rozšíøení).
unsigned char width_src = 1;


/**
 * Funkce zajišuje správné zahájení mìøícího procesu. V pøípadì, že bylo mìøení
 * pozastaveno, dojde k obnovì.
 */
void msr_start(void){
  switch(msr_status){
  case VOLNO:
  case ZMERENY_PRUMERY:
    // osetreni naplneni pameti
    if (data_top >= (DATA_MAX_POCET_ZAZNAMU - 2)){
      new_error(101); // pamì mereni plna
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
 * Funkce pøeruší mìøení. 
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


/// Funkce pøed zapoèetím mìøení urèí vzdálenosti, ve kterých dojde k mìøení
static void vypocti_vzdalenosti(void){
  int i;
  for (i = 0; i < POCET_MERENI; i++){
    mereni_l[i] = msr_delka - MIN_VZDAL_OD_KATRU - 
    (i * ((float)(1.0 / (POCET_MERENI - 1))) * (msr_delka - (MIN_VZDAL_OD_KATRU + MIN_VZDAL_OD_VOZIKU)));
  }
}

/// Funkce prùmìuje hodnoty pole
uint16_t prumeruj(uint16_t pole[], uint8_t len){
  int i;
  uint32_t suma = 0;
  for (i = 0; i < len; i++){
    suma += pole[i];
  }
  return (suma / len);
}


/// Výpoèet metody lineární regrese.
/// @returns Vrací hodnotu k
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

  l_str = l_str / len; // støední hodnota l
  
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
  l_str = l_str / len; // støední hodnota l
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
      
      if (min_vzdal > vzdalenost){ /* zjištujeme, zda se vozík už nevrací */
        min_vzdal = vzdalenost;
      }
      else{ // znamená to, že s vozík pohnul smìrem vzad
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
      if (vzdalenost < mereni_l[n_mereni]){ // pokud nastal okamžik mìøení ...
        // ulož okamžité hodnoty do pole namìøených dat
        mereni_d_v[n_mereni] = prumer_v;     //TODO
        mereni_d_h[n_mereni] = prumer_h;
        
        n_mereni++;
        if (n_mereni == POCET_MERENI){  // poslední mìøení
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
  
  // indikace stavu svìtlem
  indikace_stavu_control();
}

/// Provádí výpoèty po ukonèení mìøení.
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
 * Funkce má za úkol zajišovat indikaci stavu prostøednictvím diody.
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

/* nepotøebné
void set_width_src(unsigned char c){
  width_src = c;
  eeprom_w8((void *)ee_,c);
}*/
/* end of measure.c */

