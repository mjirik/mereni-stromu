#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include "mysql.h"
#include "log.h"

//#define DEBUG

MYSQL m;
char *user = NULL;
char *psswd = NULL;
char *db_name = NULL;

static char print_row(void);

void sql_params(char * this_user, char * this_psswd, char * this_db_name){
  user = this_user;
  psswd = this_psswd;
  db_name = this_db_name;
}


int sql_open(void) {
  /*
    Inicializace struktury
  */
  if (!mysql_init(&m)) {
    fputs("mysql_init\n", stderr);
#ifdef DEBUG
    printf("Chyba priopujeni\n");
#endif
      return -1;
  }
  /*
    Pipojen�k (nebezpe��nastaven� datab�i jako root bez hesla
  */
  if (
      !mysql_real_connect(
			  &m,
			  NULL /* = localhost*/,
			  user, /* uivatel */
			  psswd, /* heslo */
			  db_name, /* datab�e */
			  0, /* defaultn�port*/
			  NULL, /* �n socket */
			  0) /* �n�flagy */
      )
    {
      fputs("mysql_real_connect(\n", stderr);
      new_log("Selhalo pripojeni k databazi");
      return -1;
    }

  //mysql_close(&m);
  return 0;
}

void sql_close(void){
  mysql_close(&m);
}


/* Funkce vrati posledni vysledek z mysql-spojeni m */
char print_row(void){
  MYSQL_ROW row;
  MYSQL_RES *result;
  result = mysql_store_result(&m);
  if (!result) {
    fputs("mysql_store_result\n", stderr);
    return -1;
  }
  /*
    Cyklus pes vechny �ky
  */
  while ((row = mysql_fetch_row(result)) != 0){
    /*
      Vpis �ky
    */
    puts(row[0]);
  }
  /*
    Uvoln��vsledku
  */
  mysql_free_result(result);

  return 0;
}


int sql_query(char * s){
  sql_open();
  if (mysql_query(&m, s)) {
    char * s = NULL;
    sprintf(s,"MySQL query: %s\nMySQL error: %s",s,mysql_error(&m));
    fputs("mysql_query\n", stderr);
    new_log(s);

    return -1;
  }
  sql_close();
  return 0;
}

void insertZakaznik(int id, char * jmeno){
  char *s = NULL;
  s = (char *) malloc (100);
#ifdef DEBUG
  printf("Nacitani jmena : %s\n",jmeno);
#endif
  sprintf(s,"call sp_insertLoadZakaznik(%i, \"%s\");",id,jmeno);

  sql_query(s);
  mfree (s);
}

/* Funkce zapisuje do databaze mereni.
   Pokud je 'manipulace' a 'dvakrat' rovno 0, je prevedeno na NULL
*/
void insertMereni(int idzak, float delka, float prumer, int druh_dreva, int manipulace, int dalsi_zpracovani, int dvakrat ){
  char *s = NULL;
  char *s_manipulace = NULL;
  char *s_dvakrat = NULL;

  s = (char *) malloc (200);
  s_manipulace = (char *) malloc (10);
  s_dvakrat = (char *) malloc (10);

  if (manipulace == 0){
    sprintf(s_manipulace,"NULL");
  }
  else {
    sprintf(s_manipulace,"%i",manipulace);
  }

  if (dvakrat == 0){
    sprintf(s_dvakrat,"NULL");
  }
  else {
    sprintf(s_dvakrat,"%i",dvakrat);
  }
  
  //Manipulace
  //dvakrat
  sprintf(s,"call sp_insertLoadMereni(%i,%f,%f,%i,%s,%i,%s);",idzak,delka,prumer, druh_dreva + 1, s_manipulace, dalsi_zpracovani + 1, s_dvakrat);
#ifdef DEBUG
  printf("%s\n",s);
#endif
  sql_query(s);

  mfree (s);
  mfree (s_manipulace);
  mfree (s_dvakrat);
}

void insertTimemark(void){
#ifdef DEBUG
  printf("Start nacitani");
#endif
  sql_query("call sp_Timemark(\"load_start\");");
}

void dataLoad(void){
#ifdef DEBUG
  printf("Stop nacitani");
#endif
  sql_query("call sp_Load();");
}
