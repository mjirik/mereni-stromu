/**
 * popis konfiguracniho souboru
 * log_file - umisteni a jmeno logovaciho souboru. Pokud neni zadano je
 * /var/log/pila_connector.log
 * 
 * 
 *
 */

#include <stdio.h>
/* #include <io.h> */
#include "conf.h"
#include "mysql.h"
#include "serial.h"
#include "log.h"
#include "hardware_config.h"



int main(int argc, char * argv[]){
  char * port = NULL;
  char * str = NULL; 

  load_conf();
  set_log_file((str = param_gets("log_file") != NULL ? str : "/var/log/pila_connector.log"));
  new_log("Start");

  sql_params(param_gets("db_user"),param_gets("db_psswd"),"Pila");
  port = param_gets("port");
  if (port == NULL){
    fputs("Nebyl zadan port",stderr);
    return -1;
  }
  open_port(port);
  if ((argc > 1 ) && (argv[1][1] == 'c')){	/* parametry TODO */
    hardware_config();
  }
  //sql_query("select * from Zakaznik;");

  while(1){
    read_port();
  }
}


 
void get(void){
}
