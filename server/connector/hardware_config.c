#include <stdio.h>
#include <unistd.h> //prinasi funci usleep()
#include "conf.h"
#include "serial.h"
#include "log.h"

#define SLEEP_TIME_us 50000

static void irc_vozik_otacka_cm(void);
static void irc_katr_otacka_cm(void);
void width_src(void);
//static unsigned char msg[11];

void hardware_config(void){
  new_log("Nastaveni paramteru");
  //printf("int %i\n",param_geti("int"));
  irc_katr_otacka_cm();
  usleep(SLEEP_TIME_us);
  irc_vozik_otacka_cm();
  usleep(SLEEP_TIME_us);
  width_src();

  
}

void irc_vozik_otacka_cm(void){
#ifdef DEBUG
  printf("irc %f\n",param_getf("irc_vozik_otacka_cm"));
#endif
  if (param_gets("irc_vozik_otacka_cm") != NULL){ 
    float f = 0;
    unsigned char msg[11];
      
    f = param_getf("irc_vozik_otacka_cm"); 
    //printf("f = %f\n",f);
    f = f * 10; 		/* Do hardwaru se to posila v mm */
    msg[0] = '\x0E'; 
    msg[1] = (unsigned char) (f / 256); 
    msg[2] = (unsigned char) f % 256; 
#ifdef DEBUG
    printf("i = %i", msg[1]*256 + msg[2]  );
#endif
    write_port((char *)msg); 
  }
}

void irc_katr_otacka_cm(void){
  if (param_gets("irc_katr_otacka_cm") != NULL){ 
    float f = 0;
    unsigned char msg[11];

    f = param_getf("irc_katr_otacka_cm");
    //printf("f = %f\n",f);
    f = f * 10;
    msg[0] = '\x10'; 
    msg[1] = (unsigned char) (f / 256); 
    msg[2] = (unsigned char) f % 256; 
    write_port((char *)msg); 
  }
}

void width_src(void){
  unsigned char msg[10];

  msg[0] = '\x1A';
  msg[1] = (unsigned char)param_geti("width_src"); // funkce vraci nulu pokud nenalezne konstantu
  write_port((char *)msg);
}
