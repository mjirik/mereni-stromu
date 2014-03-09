#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "log.h"
#include "data.h"
#include "msr_scr.h"
#include "data_file.h"
#include "memory.h"
#include "common.h"

// global
void datf_save(void);
void datf_load(void);
// global

FILE *fw = NULL;
FILE *fr = NULL;
char * dat_file_name = "records.dat";

static unsigned char data_tmp[DELKA_POLE_DAT][7];
static char * m_jmena_tmp[MAX_ZAKAZNIK];
static int chyba_nacitani = 0;
static uint8_t data_top_tmp = 0;

static void datf_open_w(void);
static void datf_open_r(void);
static void datf_close_w(void);
static void datf_close_r(void);

static void datf_read_zakz(void);
//static void datf_read_records(void);
static void datf_read_records(char obsah_souboru[], int velikost);
static void datf_tmp2work(void);

void datf_init(void){

}

/**
 * Funkce vytvoří zálohu souboru
 */
void datf_backup(void){
  FILE * fb = NULL;
  char fname [150] = "";
  int c;
  //datf_open_r();

  sprintf(fname,"%s.bck",dat_file_name);





  fr = fopen(dat_file_name, "rb");
  fb = fopen(fname, "wb");

  if ((fr == NULL)||(fb == NULL)){
    char  * s;
    s=(char *) mmalloc(50);
    sprintf(s,"Chyba pri vytvareni zalohy souboru %s",dat_file_name);
    printf("%s\n",s);
    new_log(s);
    mfree((void *)s);
  }
  else {

    while ((c = getc(fr)) != EOF) {
      putc(c, fb);
    }
  }
  fclose(fb);
  fclose(fr);
  fb = NULL;
  fr = NULL;

}

/**
 * Funkce ukládá naměřená data a jména zákazníků. Používá přitom ukazatele z
 * hlavičkového souboru data.h . Nejdříve jsou ukládány záznamy měření a pak
 * jsou zapsány jména zákazníků oddělená novými řádky.
 */
void datf_save(void){
  int i,j;

  datf_backup();
  //char [4] cislo;

  datf_open_w();

  // uložení počtu naměřených dat

  putc((unsigned char)data_top, fw);
  putc('\n',fw);

    // ukládání dat


  for (i = 0; i < (DELKA_POLE_DAT); i++){
    //putc(*(&(data[0][0])+i),fw); //toto je méně čitelné ale rychlejší

    for (j=0; j < 7;j++){
      putc(data[i][j], fw);
    }

  }


  putc('\n',fw);
  // ukládání jmen zákazníků
  for (i = 0; i < MAX_ZAKAZNIK; i++){
    // prochází jména
    if (m_jmena[i] != NULL){
      //char len = 1 + strlen(m_jmena[i]);
      //for (j=0; j<len; j++){       }
      fprintf(fw,"%s\n", m_jmena[i]);
    }
    else{
      // prázný řetězec je zde protože je nutné zachovat i pořadí a index jména
      // pátý řetězec musí být pořád pátý.
      fprintf(fw, "\n");
    }
  }


    




  datf_close_w();
}

/**
 * Funkce načte seznam zákazníků a naměřená data ze souboru.
 * Data jsou napřed načtena do dočasných polí. Až když je jisté, že jsou v
 * pořádku, jsou zkopírována do polí skutečně pracovních.
 */
void datf_load(void){
  datf_open_r();
  int c;
  int k = 0;
//  int i,j;

  int velikost = (DELKA_POLE_DAT * 7) + (10 * MAX_ZAKAZNIK)+18;
  char obsah_souboru[velikost];
  chyba_nacitani = 0;

//  datf_read_zakz();


  // načtení souboru do pole
  while((c = getc(fr)) != EOF){
    obsah_souboru[k] = (char) c;
    
    k++;
    
    if (k >= velikost){
      new_log("prekrocena velikost vstupniho souboru");
      return;
    }
  }

  if (k < ((DELKA_POLE_DAT *7) +2)){
    // moc malo znaku
    return;
  }
  velikost = k;

  datf_close_r();

  // načtení velikosti dat
  

  datf_read_records(obsah_souboru, velikost);
  
  

  // kopírování z dočasných polí do pracovních
  if (chyba_nacitani == 0) {
    datf_tmp2work();
    // žádná chyba během načítání
  }
}

void datf_tmp2work(void){
  int i, j;

  // kopírování seznamu zákazníků;
  for (i = 0; i < MAX_ZAKAZNIK; i++) {
    m_jmena[i] = m_jmena_tmp[i];
  }

  //kopírování pole dat
  for (i = 0; i < (DELKA_POLE_DAT); i++) {
    //putc(*(&(data[0][0])+i),fw); //toto je méně čitelné ale rychlejší

    for (j = 0; j < 7; j++) {
      data[i][j]=data_tmp[i][j];
    }
  }

  // počet záznamů
  data_top = data_top_tmp;
}


void datf_read_records(char obsah_souboru[], int velikost){

  int i,j;
//  char c;
  int k = 0;

  // načtení velikosti dat
  data_top_tmp = (unsigned char) obsah_souboru[0];


  // načtení dat
  k = 2;
  for (i = 0; i < (DELKA_POLE_DAT); i++){
    //putc(*(&(data[0][0])+i),fw); //toto je méně čitelné ale rychlejší

    for (j=0; j < 7;j++){
        data_tmp[i][j]=obsah_souboru[k];
        k++;
        if (k > velikost){
          chyba_nacitani++;
          new_log("chyba pri parserovani souboru s daty");
          return;
        }
    }
  }

  // zde je znak nového řádku
  k++;

  //načítání zákazníků
  for(i=0; i < MAX_ZAKAZNIK; i++){
    char * jmeno;
    jmeno = (char *) mmalloc(MAX_LEN_ZAKAZNIK + 2 + 1); // +2 '\n'  , +1 '\0'
    j = 0;
    while (obsah_souboru[k] != '\n'){
      jmeno[j] = obsah_souboru[k];
      j++;
      k++;
      if ((k > velikost)||(j > MAX_LEN_ZAKAZNIK)){
          chyba_nacitani++;
          new_log("chyba pri parserovani souboru s daty");
          mfree((void *) jmeno);
          return;
      }
    }
    if (j == 0){
      mfree((void *) jmeno);
      m_jmena_tmp[i] = NULL;
    }
    else{
      m_jmena_tmp[i] = jmeno;
    }

    // aby začalo ukazovat za '\n'
    k++;


  }



}

/**
 * Funkce načte zákazníky ze souboru do dočasného pole.
 */
void datf_read_zakz(void){
  int i;
  for(i=0; i < MAX_ZAKAZNIK; i++){
    char * jmeno;
    jmeno = (char *) mmalloc(MAX_LEN_ZAKAZNIK + 2 + 1); // +2 '\n'  , +1 '\0'
    if (fgets(jmeno, MAX_LEN_ZAKAZNIK + 3, fr) != NULL){
      int len;
      len =  strlen(jmeno);


      if (jmeno[len-1] == '\n'){
        if (len == 1) {
          // je tam jen '\n'
          m_jmena_tmp[i] = NULL;
        }else {
          jmeno[len-1] = '\0';
          m_jmena_tmp[i] = jmeno;
        }
      }
      else{
        char s[]="chybi znak \'\\n\' na konci jmena";
        printf(s);
        new_log(s);
        chyba_nacitani++;
        //exit(-1);
      }

    }
    else {
      char s[] = "chyba pri nacitani z datoveho souboru";
      mfree((void *) jmeno);
      printf(s);
      new_log(s);chyba_nacitani++;
      //exit(-1);
    }

  }
}


/**
 * Funkce pro otevření datového souboru pro zápis
 */
void datf_open_w(void){

  fw = fopen(dat_file_name, "wb");


  if ((fw == NULL)){
    char  * s;
    s=(char *) mmalloc(50);
    sprintf(s,"Chyba pri otevirani zapisu do datoveho souboru %s",dat_file_name);
    printf("%s\n",s);
    new_log(s);
    mfree((void *)s);
    exit(-1);

  }


}


/**
 * Funkce se používá pro ukončení zápisu do datového souboru.
 */
void datf_close_w(void){
  fclose(fw);
  fw = NULL;
  //fclose(fr);
}


/**
 * Funkce pro otevření datového souboru pro zápis
 */
void datf_open_r(void){

  fr = fopen(dat_file_name, "rb");

  if ((fr == NULL)){
    char  * s;
    s=(char *) mmalloc(50);
    sprintf(s,"Chyba pri otevirani cteni z datoveho souboru %s",dat_file_name);
    printf("%s\n",s);
    new_log(s);
    mfree((void *)s);
    exit(-1);

  }


}


/**
 * Funkce se používá pro ukončení zápisu do datového souboru.
 */
void datf_close_r(void){
  fclose(fr);
  fr = NULL;
}

