/* $Id: data.h 115 2007-09-11 14:12:58Z Mira $
 */
// data.c

/// Maximální poèet záznamù. Musí být sudý. Pøi zmìnì je nutno zmìnit také parametr DELKA_POLE_DAT.
#define DATA_MAX_POCET_ZAZNAMU 210

void new_record(char zakaznik,char druh_dreva,char operace,uint16_t delka, uint16_t prumer);
void w_record(uint16_t zaznam, char zakaznik, char druh_dreva, char operace, uint16_t delka,  uint16_t prumer);
void r_record(uint16_t zaznam);

int16_t find_last_record_of_zakaznik(char zakaznik);

void dat_odstran_zakaznika(int cislo);
char dat_add_zakaznik(char * jmeno);

void dat_del_all(void);
void dat_del_record(uint16_t id);

//eeprom
void *dat_read_eeprom(void * odkud);
void *dat_write_eeprom(void *kam);

void *dat_zak_read_eeprom(void *kam);
void *dat_zak_write_eeprom(void *kam);

void dat_free_m_jmena(void);

uint16_t data_delka;
char data_zakaznik;
uint16_t data_prumer;
char data_druh_dreva;
char data_operace;

int16_t data_top;

#define MAX_ZAKAZNIK 16
char * m_jmena[MAX_ZAKAZNIK + 1];

/* end of data.h */
