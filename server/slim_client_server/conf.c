/* 
   $Id: conf.c 39 2007-05-12 10:19:26Z Mira $
*/
/**
   @file conf.c
   @brief
   Tento modul zajišťuje pohodlné načítání z konfiguračního souboru.
   Hodnoty jsou odděleny enterem,
   řádky lze komentovat tak, že je prvním znakem řádku buď '%' nebo '#'
*/


/// Definice konfiguracniho souboru.
#ifdef WIN32
#define CONF_FILE "d:\\pila_connector"
#elif
#define CONF_FILE "/usr/local/etc/pila_connector"
#endif
#define MAX_LINE 100
/* definuje velikost pole parametru */
#define MAX_PARAMS 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conf.h"
#include "memory.h"

/* globální funkce */
void load_conf(void);
char * param_gets(char *s);
int param_geti(char *s);
float param_getf(char *s);
void param_close(void);
/* globální funkce */

//#define DEBUG

//static char *port;
char * params[2][MAX_PARAMS];
int param_counter = 0;	/* pocitadlo parametru */

void load_conf(void){
  FILE*fr;
  char *line;
  char *rovnase;
  char *promenna;
  char *hodnota;
  char *komentar;

  // kdyby došlo k opakovanému volání, je napřed uvolněno z paměti vě minulé.
  param_close();

  line = mmalloc(MAX_LINE);
  promenna = mmalloc(40);
  hodnota = mmalloc(40);

  if ((hodnota == NULL) | (promenna == NULL)){
    fputs("Nedostatek pameti!",stderr);
    exit(-1);
  }

  //printf("Čtení konfigurace\n");

  if ((fr = fopen(CONF_FILE,"r")) == NULL){
    fputs("Chyba při načítání konfiguračního souboru pila.conf",stderr);
    exit(-1);
  }
  while((fgets(line,MAX_LINE,fr) != NULL)){
    int delka = 0;
    int promenna_len = 0;

    delka = strlen(line);
    if ((delka == MAX_LINE) && (line[MAX_LINE - 1] != '\n') ){
      fputs("Chyba: příliš dlouhý řádek v konfiguračním souboru",stderr);
      exit(-1);
    }

    // testujeme, zda řádek není zakomentován. Přípusné jsou pouze komentáře od
    // prvního znaku řádku
    komentar = strchr(line,'#');
    //if ((line[0]=='%')||(line[0]=='#')){

    // znak # je nahrazen symbolem konce řetězce. Zbytek za ním je tak zapomenut.
    if(komentar != NULL){
      komentar[0] = '\0';
      //continue;
    }

    rovnase = strchr(line,'=');
    if (rovnase == NULL){
      continue;
    }
    rovnase[0] = '\0';
    sscanf(line,"%s",promenna);
    sscanf(rovnase + 1,"%s",hodnota);

    /* zkopirujem hodnotu a parametr do pole */
    if ((promenna_len = strlen(promenna)) > 0){
      params[0][param_counter] = mmalloc(promenna_len + 1);
      params[1][param_counter] = mmalloc(strlen(hodnota) + 1);
      if ((params[0][param_counter] == NULL)| (params[1][param_counter] == NULL)){
	fputs("Nedostatek pameti",stderr);
	exit(-1);
      }

      strcpy (params[0][param_counter],promenna);
      strcpy (params[1][param_counter],hodnota);
      param_counter++;
#ifdef DEBUG 
      printf("%s = %s\n",promenna, hodnota);
#endif
    }

  }
  fclose(fr);
  mfree(promenna);
  mfree(hodnota);
  mfree(line);
#ifdef DEBUG
  printf("Konfigurace načtena\n");
  printf("Test nastaveni\n");
  //  printf("%s",param_gets("port"));
  //  printf("hu ");
#endif
}
/* funkce vrati hodnotu promenne jako string
   pokud pozadovanou promenou nenalezne, vrati funkce null.
*/
char * param_gets(char *s){
  int i = 0;
  //printf("jeste zijem param_gets: %s \n",s);
  for (i=0; i < param_counter; i++){
    if (strlen(s) == strlen(params[0][i])){
      if (0 == strcmp(s, params[0][i])){
	return params[1][i];
      }
    }
    //    printf("blba dylka\n");
  }
  return NULL;
}

/* Funkce vrati int odpovidajici honote parametru.
   V pripade, ze hodnota neni typu int, nebo nebyla
   hodnota vubec nalezena, vraci funkce NULL. */
int param_geti(char *s){
  char * str = NULL;
  int out = 0;

  str = param_gets(s);
  if (str == NULL){
    return 0;
  }


  if (sscanf(str,"%i",&out) != 1){
    return 0;
  }
  return out;
}


float param_getf(char *s){
  char * str = NULL;
  float out = 0;

  str = param_gets(s);
  if (str == NULL){
    return 0;
  }
  if (sscanf(str,"%f",&out) != 1){
    return 0;
  }
  return out;
}


/**
 * Funkce odstraní z paměti pole parametrů.
 */
void param_close(void){
  int i = 0;
  for (i = 0; i < param_counter; i++) {
    mfree((void *)params[0][i]);
    mfree((void *)params[1][i]);
    params[0][i] = NULL;
    params[1][i] = NULL;
  }
}
