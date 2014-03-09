/**
 * popis konfiguracniho souboru
 * log_file - umisteni a jmeno logovaciho souboru. Pokud neni zadano je
 * /var/log/pila_connector.log
 * 
 * 
 *
 */
// tyto definice jsou tu jen pro to aby v NetBeans nebyl kód šedivý
// při překladu není QQQ nadefinováno a tak zde nejsou zavedeny tyto definice
// definice zavádí makefile
#ifndef MODUL_NOTALL
#define MODUL_COMMUNICATION
#define MODUL_DATABASE
#define MODUL_INTERFACE
#define MODUL_HARDWARE
#endif

#include <stdio.h>
/* #include <io.h> */
#include "conf.h"
#include "log.h"

#ifdef MODUL_HARDWARE
//#include "hardware_config.h"
#endif

#ifdef MODUL_DATABASE
#include "mysql_connect.h"
#endif

#ifdef MODUL_COMMUNICATION
#include "serial.h"
#endif

#ifdef WIN32
#define DEFAULF_LOGFILE "d:\\pila_connector.log"
#elif
#define DEFAULF_LOGFILE "/var/log/pila_connector.log"
#endif

int main(int argc, char * argv[]) {

  char * port = NULL;
  char * str = NULL;

  load_conf();
  set_log_file((str = param_gets("log_file") != NULL ? str : DEFAULF_LOGFILE));
  new_log("Start");

  port = param_gets("port");
  if (port == NULL) {
    fputs("Nebyl zadan port", stderr);
    return -1;
  }


  open_port(port);
  printf(port);
#ifdef MODUL_DATABASE
  sql_params(param_gets("db_user"), param_gets("db_psswd"), "Pila");

#endif


#ifdef MODUL_HARDWARE
  if ((argc > 1) && (argv[1][1] == 'c')) { /* parametry TODO */
    hardware_config();
  }
#endif
  //sql_query("select * from Zakaznik;");

  while (1) {
   // static int q = 0;
    read_port();
 //   q++;
/*
    if (q == 1000) {
      printf("x");
      q = 0;
    }
*/
  }
}

void get(void) {
}
