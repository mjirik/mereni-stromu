#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#ifdef WIN32
#include <windows.h>
#elif
#include <termios.h>
#endif
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "data.h"
#ifdef MODUL_DATABASE
#include "mysql_connect.h"
#endif
#include "log.h"
#include "keyboard.h"
#include "msgn.h"
#include "measure.h"
#include "common.h"
#include "error.h"
#include "memory.h"
#include "vizualizace.h"


//global
void open_port(char * port);
void write_port(char *s);
void read_port(void);
void close_port(void);
void zpracuj_zpravu(void);
//global

//#define DEBUG
//#define DEBUG2


#define BUFFER_SIZE (2 + DATASIZE + 1 + (DATASIZE/2))

#define POST_LOAD_SCRIPT "/usr/local/bin/pila_connector.pst_ld_scr.sh"

static void reset_buffer(void);
static void into_buffer(unsigned char c);
static int msg_type();
static int msg_type();


static FILE*fw = NULL;
static FILE*fr = NULL ;
static unsigned char buffer[BUFFER_SIZE];
static unsigned char trn_buffer[BUFFER_SIZE];

#ifdef WIN32
HANDLE hPort;
#endif

/**
 * Funkce otevírá sériopvý port
 */
void open_port(char * port){
  
  // Windows
  /*

  hPort = CreateFile(portName, // Specify port device: default "COM1"
          GENERIC_READ | GENERIC_WRITE, // Specify mode that open device.
          0, // the devide isn't shared.
          NULL, // the object gets a default security.
          OPEN_EXISTING, // Specify which action to take on file.
          0, // default.
          NULL); // default.

  if (hPort == INVALID_HANDLE_VALUE){
    char  * s;
    sprintf(s,"Chyba pri otevirani portu %s",port);
    printf("Chyba při otevírání portu %s ! ",port);
    new_log(s);
    exit(-1);
  }
   // error opening port; abort


  */
   //printf("otvirame port %s",port);
   // je důležité používat binární režim, jinak dochází k nahrazování znaku \n, znaky 10,13
  //Linux
  #ifdef WIN32
  fw = fopen(port, "wb+");
  fr = fw;
  #elif
  fw = fopen(port,"wb");
  #endif
  
  //printf("zapis\n");


  #ifdef WIN32
  
  #elif
  fr = fopen(port,"rb");
  #endif


  //printf("cteni\n");
 /* if (fr == NULL){

    fputs("cteni chyba\n",stderr);
  }

  if (fw == NULL){
    fputs("zapis chyba\n",stderr);
  }*/

  if ((fw == NULL)||(fr == NULL)){
    char   s[50];
    sprintf(s,"Chyba pri otevirani portu %s",port);
    fputs(s,stderr);
    new_log(s);
    //exit(-1);
    
  }

    //printf("Port Otevren");
     //printf("velikost bufferu %i", BUFFER_SIZE);
  reset_buffer();


/*   fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY ); */
/*   if (fd == -1){ */
/*     printf("open_port: Nelze otevrit port"); */
/*     //exit (1); */
/*   } */
/*   printf("Zatim to jde\n"); */
/*   fcntl(fd, F_SETFL); */

/*   printf("porad to jede"); */
/*   write (fd, "Ahoj",4); */

}


/** Funkce obali zpravu ochrannym kodem a odesle ji po seriove lince */
void write_port(char *s) {
  if (fw == NULL) {
    fputs("Neni otevren port", stderr);
    //printf();
  } else {
    int i = 0;

    trn_buffer[0] = 255;
    for (i = 0; i < (DATASIZE + 1); i++) {
      trn_buffer[i + 1] = s[i];

    }

    for (i = 0; i < DATASIZE; i = i + 2) {
      unsigned char a, b, c;
      a = trn_buffer[i + 2];
      b = trn_buffer[i + 3];
      c = 2 + DATASIZE + 1 + (i / 2);
      trn_buffer[c] = a^b;
    }
    //printf("\n");
    for (i = 0; i < BUFFER_SIZE; i++) {
      // zde je odesláno do odesílacího bufferu, odeslání provádí fflush
      putc(trn_buffer[i], fw);
      //printf("%3i ",(unsigned char)trn_buffer[i]);

      //debug
      //if ( EOF == fflush(fw)){
      //fputs("write_port: Chyba pri zapisu na port",stderr);
      //}
      //debug
    }
    //printf("\n");
    //fprintf(fw,"");
    if (EOF == fflush(fw)) {
      fputs("write_port: Chyba pri zapisu na port", stderr);
      new_log("Chyba pri zapisu na port");
      exit(-1);
    }

  }
}

void read_port(){

  unsigned char c;
  int ic;
  unsigned char typ=0;

  //while(1){

  if (fr != NULL) {
    // port je otevřený
    ic = getc(fr);
    if (ic != EOF) {
      c = (unsigned char) ic;

      into_buffer(c);

      //printf("%c, %3i  , %3i \n",c,(int)c,ic);

      typ = msg_type();
      if (typ != 0) {
        //printf("Dorazilo");
        zpracuj_zpravu();
      }
      //}
#ifdef DEBUG
      printf("Dorazilo");
#endif
    }
  }

}

void close_port(void) {
  fclose(fr);
#ifndef WIN32
  // na platformě win32 se zavírají oba najednou
  fclose(fw);
#endif
}

void reset_buffer(void){
  int i=0;
  for (i=0; i < BUFFER_SIZE; i++){
    buffer[i]=0;
  }
}

/* Funkce vkládá data do bufferu
 */
void into_buffer(unsigned char c){
  int i=0;
  for (i=0;i < BUFFER_SIZE - 1;i++){
    buffer[i]=buffer[i+1];
  }
  buffer[i]=c;
}

// funkce vrací typ zprávy v případě že je zpráva ok, jinak vrací 0
int msg_type(){
  int i=0;

  if (buffer[0] != 255){
    return 0;
  }
  //printf("zacatek ok!\n");
#ifdef DEBUG
  printf("zacatek ok!\n");
#endif
  for(i=0;i < DATASIZE;i=i+2){
    unsigned char a,b,c,d;
    a = buffer[i+2];
    b = buffer[i+3];
    c = 2 + DATASIZE + 1 + (i/2);
    d = buffer[c];
    //printf("%i ^ %i = %i, compare with buffer[%i] = %i\n",a,b,a^b,c,d);
#ifdef DEBUG
    printf("%i ^ %i = %i, compare with buffer[%i] = %i\n",a,b,a^b,c,d);
#endif
    if ((a^b) != d){
      return 0;
    }
  }
  return buffer[1];      
}

/// Funkce ridi reakce na dosle zpravy.
void zpracuj_zpravu(){
  char * jmn = NULL;
  switch (buffer[1]){
  case '\x41':

    jmn = (char *)mmalloc((unsigned int)strlen((void *)(buffer + 4)) + 1);
    jmn = strcpy(jmn,(char *)buffer + 4);
#ifdef DEBUG1
    printf("dorazilo  jmeno : %s ",jmn);
#endif

    //    add_zakaznik (buffer[2],0,jmn);
    //print_zakaznici();
    #ifdef MODUL_DATABASE
    insertZakaznik((int) buffer[2],jmn);
    #endif
    mfree(jmn);
    write_port("\x14                    ");
    break;
  case '\x42':{
    int idmer = 0;
    int idzak = 0;
    float delka = 0;
    float prumer = 0;
    int druh_dreva = 0;
    int manipulace = 0;
    int dalsi_zpracovani = 0;
    int dvakrat = 0;
#ifdef DEBUG
    printf ("[5]: %i, [6]: %i \n",(int)buffer[5], (int) buffer[6]);
#endif

#ifdef DEBUG1
    printf("Data\n");
#endif
    idmer = (int) buffer[2];
    idzak = (int) buffer[3];
    
    delka = (float) ((float)(((float)buffer[4])*255 + buffer[5]))*(0.01);
    prumer = (float) ((float)(((float)buffer[6])*255 + buffer[7]))*(0.1);
    druh_dreva = (int) buffer[8];
    manipulace = (int) buffer[11];
    dalsi_zpracovani = (int) buffer[10];
    dvakrat = (int) buffer[9];
    //    printf("dorazila data \n");
    //    printf("id: %i : ",(int)buffer[3]);
    //    printf("id: %i\n",(int)buffer[4]);
    #ifdef MODUL_DATABASE
    insertMereni(idzak,delka,prumer,druh_dreva,manipulace,dalsi_zpracovani,dvakrat);
    #endif
    write_port("\x14                   ");
    break;
  }
  case '\x43':
#ifdef DEBUG1
    printf("dataLoad()");
#endif
    #ifdef MODUL_DATABASE
    dataLoad();
    #endif
    write_port("\x14                    ");
    system(POST_LOAD_SCRIPT);
    break;
  case '\x44':
#ifdef DEBUG1
    printf("call sp_Timemark(\"load_start\");");
#endif
    //printf("heh");
    #ifdef MODUL_DATABASE
    sql_query("call sp_Timemark(\"load_start\");");
    #endif
    write_port("\x14                        ");
    break;
  case '\x46':
    /// @todo Zde je mozno pred odeslanim teto zpravy odeslat nejaka data.
    /// Napriklad informace o vodorovnem prumeru stromu ziskanem kamerou.
    write_port("\x47                        ");
      break;
    case MSGN_KEY:
    {
      char s[20];
      sprintf(s,"prijata klavesa %c", buffer[3]);
      vis_print(s);
      keyb_in_buff(buffer[2], buffer[3]);
    }
      break;
    case MSGN_IRC:
      vis_print("prijata vzdaleost ");
      vzdalenost_katr = buffer[2]+(256*buffer[3]);//+(256*256*buffer[4])+(256*256*256*buffer[5]);
      prumer_v = buffer[6]+(256*buffer[7]);//+(256*256*buffer[8])+(256*256*256*buffer[9]);
      break;
    case MSGN_ERR:
    {
      char s[20];
      sprintf(s,"prijat error %i ",(unsigned int) buffer[2]);
      vis_print(s);
      new_error(buffer[2]);
    }
      break;
      

  default:
    //    printf("doslo\n");
    break;
  }
}



/**
 * Funkce odešle jedno až dvě čísla uint16 po sériové lince
 */
void send_uint32(char msgn, uint32_t number1, uint32_t number2){
  char msg[DATASIZE];

//  uint32_t * uint32;
//  unsigned char num;

  // alokace místa pro novou zprávu, nastavení vlajky zprávy
  //msg =(void*) mmalloc (DATASIZE);
  msg[0] = msgn;

  uint2string(msg+1,number1);
  uint2string(msg+5,number2);

  write_port(msg);

}
