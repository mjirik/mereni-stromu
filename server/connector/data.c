/* $Id$ */

/* V tomto modulu je resena prace se ziskanymi daty.
   Se zaznamy se pracuje primo, tj. pracuje se s poli
   zakaznici a zaznamy. Pri pridavani zakaznika, nebo zaznamu  je nutno zvysit
   couter max_zakaznik, resp. max_zaznam.
*/

#include <stdio.h>
#include <stdlib.h>
#include "data.h"

static int hledej_dupl_zakaznik(int id);

int max_zakaznik = 0;
int max_zaznam = 0;

void del_zakaznici(void){
  int i= 0;

  for (i = 0; i< max_zakaznik; i++){
    clr_zakaznik(i);
  }
}

void del_zaznamy(void){
  int i= 0;

  for (i = 0; i< max_zaznam; i++){
    clr_zaznam(i);
  }
}


/* Funkce vynuluje zakaznika, nesmaze ho! */
void clr_zakaznik (int i){
  zakaznici[i].id = 0;
  zakaznici[i].id_mysql = 0;
  mfree(zakaznici[i].jmeno);
  zakaznici[i].jmeno = NULL;
}

/* Funkce vynuluje zaznam, nesmaze ho! */
void clr_zaznam (int i){
  zaznamy[i].id = 0;
  zaznamy[i].id_mysql = 0;
  zaznamy[i].prumer = 0;
  zaznamy[i].delka = 0;
  zaznamy[i].druh_drev = 0;
  zaznamy[i].operace = 0;
  zaznamy[i].manipulace = 0;
  zaznamy[i].dvakrat = 0;
}

  
int hledej_dupl_zakaznik(int id){
  int i;
  for (i = 0; i < max_zakaznik;i++){
    if (id == zakaznici[i].id){
      return i;
    }
  }
  return -1;
}


char add_zakaznik(int id, int id_mysql, char * jmeno){
  int i = 0;

  if ((max_zakaznik + 1) >= MAX_ZAKAZNIK){
    return 0;
  }
  
  if ((i = hledej_dupl_zakaznik(id)) > 0){
    clr_zakaznik(i); // mfree na jmeno
    zakaznici[i].id = id;
    zakaznici[i].id_mysql = id_mysql;
    zakaznici[i].jmeno = jmeno;
  }
  else{
    zakaznici[max_zakaznik].id = id;
    zakaznici[max_zakaznik].id_mysql = id_mysql;
    zakaznici[max_zakaznik].jmeno = jmeno;
    max_zakaznik++;
  }

  return 1;
}

/* Funkce odstrani zakaznika za seznamu pokud vse dopadne ok, vraci
   0, jinak 1*/
char del_zakaznik(int id){
  int i = 0;

  if ((id >= max_zakaznik)||(max_zakaznik == 0)){
    return 1;
  }
  
  mfree(zakaznici[id].jmeno);

  for (i = id; i < max_zakaznik; i++){
    zakaznici[i-1] = zakaznici[i];
  }
  max_zakaznik--;

  return 0;
}

ZAKAZNIK get_zakaznik(int id){
  if ((id >= max_zakaznik) && (id >= 0)){
    printf("get_zakaznik: Prekrocen rozsah pole!");
  }
  return zakaznici[id];
}

void print_zakaznik(int id){
  printf("id = %i\n",zakaznici[id].id);
  printf("id_mysql = %i\n",zakaznici[id].id_mysql);
  printf("jmeno = %s\n", zakaznici[id].jmeno);
}

void print_zakaznici(void){
  int i = 0;

  for (i = 0; i < max_zakaznik; i++){
    print_zakaznik(i);
  }
}
