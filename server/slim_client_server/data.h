/* $Id: data.h 115 2007-09-11 14:12:58Z Mira $
 */
// data.c


/// polovina počtu záznamů, celkem je možno zaznamenat DELKA_POLE_DAT * 2 záznamů
#define DELKA_POLE_DAT 105

/// Maximální počet záznamů. Musí být sudý. Při změně je nutno změnit také parametr DELKA_POLE_DAT.
#define DATA_MAX_POCET_ZAZNAMU DELKA_POLE_DAT * 2

extern void new_record(char zakaznik,char druh_dreva,char operace,unsigned int delka, unsigned int prumer);
extern void w_record(unsigned int zaznam, char zakaznik, char druh_dreva, char operace, unsigned int delka,  unsigned int prumer);
extern void r_record(unsigned int zaznam);

extern int find_last_record_of_zakaznik(char zakaznik);

extern void dat_odstran_zakaznika(int cislo);
extern char dat_add_zakaznik(char * jmeno);

extern void dat_del_all(void);
extern void dat_del_record(unsigned int id);

extern void dat_free_m_jmena(void);
extern int dat_prvni_zakaznik(void);

extern void dat_load(void);

extern unsigned int data_delka;
extern char data_zakaznik;
extern unsigned int data_prumer;
extern char data_druh_dreva;
extern char data_operace;


extern uint8_t data_top;


/// pozor toto je nově zavedeno, většina starých funkcí má natvrdo nastaveno 8
#define MAX_LEN_ZAKAZNIK 8

#define MAX_ZAKAZNIK 16
extern char * m_jmena[MAX_ZAKAZNIK + 1];

/// Datové pole, kam se ukládanjí naměřená data.
extern unsigned char data[DELKA_POLE_DAT][7];



/* end of data.h */
