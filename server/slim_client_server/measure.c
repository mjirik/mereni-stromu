/* $Id: measure.c 120 2007-09-21 05:43:42Z Mira $
 */
/**
 * @file measure.c
 * @brief
 * Modul slouží k obstarání funkcí pro měření.
 * 
 * Měření je zahajováno obsluhou. Stiskuntí tlačítka zavolá funkci msr_start().
 * Je zaznamenáne délka kmene. Z té jsou vypočteny vzdálenosti dílčích měření.
 * Polohy nejkrajnějších měření jsou dány proměnnými MIN_VZDAL_OD_KATRU a
 * MIN_VZDAL_OD_VOZIKU. Mezi nimi je (včetně krajních bodů) zaznamenáno tolik
 * vzorků, kolik uvádí POCET_MERENI. 
 * Funkce measure() je volána z hlavního programu v nekonečné syčce. V případě
 * dosažení měřícího bodu, se stará o zaznamenání hodnot průměrů. Pokud
 * začne vozík couvat od katru o více než BACK_MOVE_TOLERANCE cm, je měření
 * přerušeno. Pokud je naměřeno dostatek (MIN_POCET_MERENI) měření, jsou
 * data zpracována bezeslov. Pokud je měření méně, je vypsáno varování o
 * možné nepřesnosti dat. Jestliže nebylo ještě dosaženo ani
 * jediného měřícího bodu, je měření zrušeno (msr_storno()).
 * Zpracování naměřených dat probíhá tak, že jsou neměřenými hodnotami
 * průměru na výšku i na šířku proloženy přímky. Do rovnice přímek je
 * pak dosazena hodnota poloviny délky měřeného stromu a je tak získán odhad
 * příslušných průměrů.
 *
 * Fáze měření je indikována výčtovou proměnnou msr_status. Na jejím základě
 * pak probíhá ukládání dat v msr_scr.c 
 */


#include <time.h>
#include <inttypes.h>
#include <stdint.h>
//#include <avr/io.h>
#include "measure.h"
//#include "irc.h"
#include "error.h"
#include "lcd.h"
#include "convert.h"
#include "data.h"
#include "inout.h"

// global
void measure(void);
void msr_start(void);
void msr_storno(void);
void msr_init(void);
void msr_unpause(void);
void msr_pause(void);

unsigned int prumer_v;
unsigned int prumer_h;
unsigned int vzdalenost_katr;
unsigned int msr_prumer_v;
unsigned int msr_prumer_h;
unsigned int msr_prumer;
unsigned int msr_delka;
unsigned int msr_objem;

//uint8_t msr_pocet_mereni;
STAV_MERENI msr_status;

unsigned char width_src;

// global



#define POCET_MERENI 8
#define MIN_POCET_MERENI 4
#define MIN_VZDAL_OD_KATRU 50 //cm
#define MIN_VZDAL_OD_VOZIKU 150
#define BACK_MOVE_TOLERANCE 50 //cm
#define MAX_INT32 2147483638

/// průměr kterým je spuštěno měření délky po stisknutí tlačítka START
#define MIN_START_PRUMER 10


static void vypocti_vzdalenosti(void);
static void msr_end(void);

inline void indikace_stavu_control(void);

//char msr_run = 0;
//static char last_start = 0;
unsigned int msr_delka = 0;
unsigned int msr_prumer = 0;
unsigned int msr_objem = 0;
//static unsigned int pulka = 0;
static unsigned int mereni_d_v[POCET_MERENI];
static unsigned int mereni_d_h[POCET_MERENI];
static unsigned int mereni_l[POCET_MERENI];
static uint8_t n_mereni = POCET_MERENI;
// Pokud budu chtít změnit počet měření, tak
//static float skok_mereni = (float)1.0/(POCET_MERENI + 2);


/// Používá se pro kontrolu, zda vozík už neodjíždí od katru.
static unsigned int min_vzdal = 16000; //MAX_INT32; // max int32

STAV_MERENI msr_status = VOLNO;

/// Určuje, zda je odkud je brán horizontální průměr kmene.
/// Pokud je 0, průměr není měřen, pokud je 1, je měřen zařízením na katru,
/// je-li 2, jsou data přiváděna ze serveru (možné rozšíření).
unsigned char width_src = 1;

/**
 * Funkce zajišťuje správné zahájení měřícího procesu. V případě, že bylo měření
 * pozastaveno, dojde k obnově.
 */
 void msr_start(void) {
  switch (msr_status) {
    case VOLNO:
    case ZMERENY_PRUMERY:
      // osetreni naplneni pameti
      if (data_top >= (DATA_MAX_POCET_ZAZNAMU - 2)) {
        new_error(101); // paměť mereni plna
      } else {
        if (data_top >= (DATA_MAX_POCET_ZAZNAMU - 11)) {
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
 * Funkce přeruší měření. 
 */
void msr_storno(void) {
  msr_status = VOLNO;
  n_mereni = 0;
}

void msr_pause(void) {
  msr_status = (msr_status + 3);
}

void msr_unpause(void) {
  msr_status = (msr_status - 3);
  min_vzdal = vzdalenost_katr;
}

void msr_zmer_l(void) {
  msr_delka = vzdalenost_katr;
  if (msr_delka > (MIN_VZDAL_OD_VOZIKU + MIN_VZDAL_OD_KATRU)) {
    //  n_mereni = POCET_MERENI;
    //pristi_mereni = (unsigned int) (skok_mereni * msr_delka * (n_mereni + 1));
    n_mereni = 0;
    vypocti_vzdalenosti();

    msr_status = L_ZMERENO;
    min_vzdal = msr_delka;
  } else {
    new_error(99);
    msr_storno();
  }
}


//// Funkce před započetím měření určí vzdálenosti, ve kterých dojde k měření
static void vypocti_vzdalenosti(void) {
  int i;
  for (i = 0; i < POCET_MERENI; i++) {
    mereni_l[i] = msr_delka - MIN_VZDAL_OD_KATRU -
            (i * ((float) (1.0 / (POCET_MERENI - 1))) * (msr_delka - (MIN_VZDAL_OD_KATRU + MIN_VZDAL_OD_VOZIKU)));
  }
}

/// Funkce průměuje hodnoty pole
unsigned int prumeruj(unsigned int pole[], uint8_t len) {
  int i;
  uint32_t suma = 0;
  for (i = 0; i < len; i++) {
    suma += pole[i];
  }
  return (suma / len);
}


/// Výpočet metody lineární regrese.
/// @returns Vrací hodnotu k
float get_k(unsigned int pole_d[], unsigned int pole_l [], uint8_t len) {
  int i;
  float l_str = 0;
  float citatel = 0; // \sum (I - \overline{I}) F_I
  float jmenovatel = 0; // \sum( (I - \overline{I})^2 )

  if (len < 2) { /* Osetreni nizkeho poctu mereni */
    return 0;
  }


  for (i = 0; i < len; i++) {
    l_str += pole_l[i];
  }

  l_str = l_str / len; // st�edn� hodnota l

  for (i = 0; i < len; i++) {
    citatel += (pole_l[i] - l_str) * pole_d[i];
    jmenovatel += (pole_l[i] - l_str) * (pole_l[i] - l_str);
  }
  return (float) (((float) citatel) / jmenovatel);
}

float get_q(unsigned int pole_d[], unsigned int pole_l [], uint8_t len, float k) {
  int i;
  float l_str = 0;
  float d_str = 0;

  if (len < 1) {
    return 0;
  }

  for (i = 0; i < len; i++) {
    l_str += pole_l[i];
    d_str += pole_d[i];
  }
  l_str = l_str / len; // st�edn� hodnota l
  d_str = d_str / len;

  return d_str - (k * l_str);
}

void measure(void) {
  //msr_pocet_mereni = n_mereni;
  switch (msr_status) {
    case L_NEZMERENO:
      if (width_src == 1) {
        if (prumer_h > MIN_START_PRUMER) {
          msr_zmer_l();
        }
      } else {
        msr_zmer_l();
      }
      break;
    case L_ZMERENO:
    {
      unsigned int vzdalenost;
      vzdalenost = vzdalenost_katr;

      if (min_vzdal > vzdalenost) { /* zji�tujeme, zda se voz�k u� nevrac� */
        min_vzdal = vzdalenost;
      } else { // znamen� to, �e s voz�k pohnul sm�rem vzad
        if ((vzdalenost - min_vzdal) > BACK_MOVE_TOLERANCE) {
          if (n_mereni < 2) {
            new_error(93); // Mereni zruseno
            msr_storno();
          } else {
            if (n_mereni < MIN_POCET_MERENI) {
              new_error(92); // varovani: mereni nepresne
              msr_end();
            } else { // jinak je mereni normalne ukoceno
              msr_end();
            }
          }
        }
      }
      if (vzdalenost < mereni_l[n_mereni]) { // pokud nastal okam�ik m��en� ...
        // ulo� okam�it� hodnoty do pole nam��en�ch dat
        mereni_d_v[n_mereni] = prumer_v; //TODO
        mereni_d_h[n_mereni] = prumer_h;

        n_mereni++;
        if (n_mereni == POCET_MERENI) { // posledn� m��en�
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
  // @todo
  // indikace stavu světlem
  indikace_stavu_control();
}

/// Provádí výpočty po ukončení měření.
void msr_end(void) {
  float k1, q1, k2, q2;

  k1 = get_k(mereni_d_v, mereni_l, n_mereni);
  q1 = get_q(mereni_d_v, mereni_l, n_mereni, k1);

  msr_prumer_v = (unsigned int) ((k1 * (msr_delka / 2.0)) + q1);

  k2 = get_k(mereni_d_h, mereni_l, n_mereni);
  q2 = get_q(mereni_d_h, mereni_l, n_mereni, k2);

  msr_prumer_h = (unsigned int) ((k2 * (msr_delka / 2.0)) + q2);

  //msr_prumer = (unsigned int) ((((k1 * (msr_delka / 2.0)) + q1 +(k2 * (msr_delka / 2.0)) + q2)) / 2);
  switch (width_src) {
    case 1:
    case 2:
      msr_prumer = (unsigned int) ((((k1 * (msr_delka / 2.0)) + q1 + (k2 * (msr_delka / 2.0)) + q2)) / 2);
      break;
    case 0:
      msr_prumer = (unsigned int) ((k1 * (msr_delka / 2.0)) + q1);
      break;
    default:
      break;
  }


  msr_objem = (uint32_t) (((3.1415 * (msr_prumer * msr_prumer) / 400000.0) * msr_delka)); //00000000

  msr_status = ZMERENY_PRUMERY;
  n_mereni = 0;
}

/** 
 * Funkce má za úkol zajišťovat indikaci stavu prostřednictvím diody.
 * @todo dopsat  
 */
void indikace_stavu_control(void) {
  static time_t casovac = 0;
  time_t cas_tmp;

  switch (msr_status) {
    case L_NEZMERENO:
    case L_ZMERENO:
      io_control_zmereno(1);
      // jako I_PORT = I_PORT | (1 << I_BIT)
      break;
    case VOLNO:
    case ZMERENY_PRUMERY:
      io_control_zmereno(0);
      

      break;
    case PAUSE_L_NEZMERENO:
    case PAUSE_L_ZMERENO:
      cas_tmp = time(NULL);
      if (difftime(cas_tmp,casovac) < 2) {
        if (difftime(cas_tmp,casovac) < 1)
          io_control_zmereno(0);
        else
          io_control_zmereno(1);
      } else {
        casovac = cas_tmp;
      }
      break;
    default:
      break;
  }
}

void msr_init(void) {
  io_control_zmereno(0);
  //INDIKACE_DDR |= _BV(INDIKACE_BIT);
}

/* nepot�ebn�
void set_width_src(unsigned char c){
  width_src = c;
  eeprom_w8((void *)ee_,c);
}*/
/* end of measure.c */

