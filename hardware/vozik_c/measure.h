/*
 * File name: measure.h
 * Date:      2006/05/01 13:25
 * Author:    Mira
 */
// measure.c
void measure(void);
void msr_start(void);
void msr_storno(void);
void msr_init(void);
void msr_unpause(void);
void msr_pause(void);

uint16_t prumer_v;
uint16_t prumer_h;
uint16_t msr_prumer_v;
uint16_t msr_prumer_h;
uint16_t msr_prumer;
uint16_t msr_delka;
uint16_t msr_objem;

//uint8_t msr_pocet_mereni;

typedef enum{
  VOLNO = 0, 
  L_NEZMERENO = 1, 
  L_ZMERENO = 2, 
  ZMERENY_PRUMERY = 3,
  PAUSE_L_NEZMERENO = 4,
  PAUSE_L_ZMERENO = 5
} STAV_MERENI;

STAV_MERENI msr_status;

unsigned char width_src;


/* end of measure.h */
