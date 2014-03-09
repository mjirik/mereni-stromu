/*
 * File name: measure.h
 * Date:      2006/05/01 13:25
 * Author:    Mira
 */
// measure.c
typedef enum{
  VOLNO = 0,
  L_NEZMERENO = 1,
  L_ZMERENO = 2,
  ZMERENY_PRUMERY = 3,
  PAUSE_L_NEZMERENO = 4,
  PAUSE_L_ZMERENO = 5
} STAV_MERENI;


extern void measure(void);
extern void msr_start(void);
extern void msr_storno(void);
extern void msr_init(void);
extern void msr_unpause(void);
extern void msr_pause(void);

extern unsigned int prumer_v;
extern unsigned int prumer_h;
extern unsigned int vzdalenost_katr;
extern unsigned int msr_prumer_v;
extern unsigned int msr_prumer_h;
extern unsigned int msr_prumer;
extern unsigned int msr_delka;
extern unsigned int msr_objem;

//uint8_t msr_pocet_mereni;

extern STAV_MERENI msr_status;

extern unsigned char width_src;


/* end of measure.h */
