/**
 * @brief
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "log.h"


//global
void set_log_file(char * this_log_file);
void new_log(char * log);
//global

char * log_file = NULL;
time_t tim;
/// Funkce nastavuje jmeno logovaciho souboru
void set_log_file(char * this_log_file){
  log_file = this_log_file;
}

/// Zapis logu do logovaciho souboru. Musi byt nastaveno jeho jmeno
/// pomoci funkce set_log_file
void new_log(char * log){
  FILE *fw;
  if ((fw = fopen(log_file,"w")) == NULL){
    fputs("Chyba při načítání logovacího souboru\n",stderr);
    exit(-1);
  }
  time(&tim);
  fprintf(fw,"--  %s%s\n",ctime(&tim),log);
  fclose(fw);
}
