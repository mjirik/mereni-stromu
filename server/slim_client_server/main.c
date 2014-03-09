/*
 * $Id: main.c 120 2007-09-21 05:43:42Z Mira $
 */
/**
 * @file main.c
 * @brief
 * Tento modul je hlavní hlavní částí programu pro zařízení na vozíku v rámci
 * projektu měření objemu řeziva pro pilu v Němčicích. Zařízení na vozíku má
 * za úkol zpracovávat informace, které dostává po sériové lince ze zařízení na
 * katru, komunikovat s navazujícím databázovým systémem, obsluhovat měření
 * vzdálenosti vozíku od katru pomoocí IRC a zajišťovat komunikaci s uživatelem
 * prostřednictvím znakového displeje (4x20 znaků) a standartní počítačové
 * klávesnice.
 *
 * V tomto modulu je hlavní funkce main. Jednotlivé části programu jsou volány
 * z ní. Nejprve probíhají nutné inicializace ostatních modulů. Pak program
 * vejde do nekonečné smyčky, která opakovaně volá funkce pro obsluhu
 * jednotlivých částí. Obsluhu inkrementálního čidla je nutné provádět co možná
 * nejčastěji. Důležité je hlavně nepřekročení určitého času mezi voláním
 * obslužné funkce. Ta je proto volána prostřednictvím přerušení čítače. Volání
 * displeje zase naopak vyžaduje mezi voláním určitý čas. Proto je řešeno v
 * závislosti na počtu přerušení generovaných čítačem(který je používán i pro
 * IRC).
 *
 * Pro případ výpadku napájení je v obvodu zařazen záložní kondenzátor. Ten
 * zajistí napájení obvodu po nějaký čas během výpadku. V tomto čase dochází k
 * ukládání životně důležitých dat do EEPROM. To je řešeno ve funkci
 * test_shutdown(). Pokud dojde k obnovení napájení před vyčerpáním záložních
 * zdrojů, fuunkce vyčká, aby napětí na kondenzátoru vystoupilo nad 4.5 V
 * (kvůli LCD), znovu inicializuje LCD a obnoví provoz zařízení.
 */

/**
 * popis konfiguracniho souboru
 * log_file - umisteni a jmeno logovaciho souboru. Pokud neni zadano je
 * /var/log/pila_connector.log
 *
 *
 *
 */

//#define PCDEBUG

// tyto definice jsou tu jen pro to aby v NetBeans nebyl kód šedivý
// při překladu není QQQ nadefinováno a tak zde nejsou zavedeny tyto definice


// definice zavádí makefile (to už neplatí)
// definice jsou zaváděny při překlady pomocí netBeans
        //(Vlastnosti projektu - Build -> C Compiler -> Preprocessor definitons)

//#ifndef MODUL_NOTALL
//#define MODUL_COMMUNICATION
//#define MODUL_DATABASE
//#define MODUL_INTERFACE
//#define MODUL_HARDWARE
//#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "conf.h"
#include "log.h"
#include "inout.h"
#include "data.h"
#include "memory.h"

#ifdef MODUL_HARDWARE
//#include "hardware_config.h"
#endif


#ifdef MODUL_DATABASE
#include "mysql_connect.h"
#endif

#ifdef MODUL_COMMUNICATION
#include "serial.h"
#endif

#ifdef MODUL_CAMERA
#include "camera.h"
#endif

// global
char data_msg[SizeOfMsg];
char str_buf[41];
//char * getstr_P( PGM_P s_p);
//void * mmalloc (unsigned int size);
void my_watchdog(void);
//global


#ifdef WIN32
#define DEFAULF_LOGFILE "d:\\pila_connector.log"
#elif
#define DEFAULF_LOGFILE "/var/log/pila_connector.log"
#endif

//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/signal.h>

//#include <stdio.h>
//#include <math.h>
//#include <util/delay.h>
//#include <avr/pgmspace.h>
//#include <avr/wdt.h>

#include "main.h"
/*
#include "usart.h"
*/
//#include "irc.h"
//#include "eeprom.h"
#include "measure.h"
#include "lcd.h"
#include "convert.h"
#include "data.h"
#include "gui.h"
#include "error.h"
#include "keyboard.h"
#include "dat_scr.h"
#include "buttons.h"
#include "slowloop.h"
#include "vizualizace.h"
#include "data_file.h"
        
//#include "watchdog.h"


//#define SizeOfMsg 5
//#define F_CPU 8000000UL ///< Frekvence oscil�toru 8MHz
//#define T_LCD_MAX 5 ///< Kolikr�t pomaleji bude b�et p�ekreslov�n� lcd oproti kontrole irc





/**
 * LCD není časově tak kritická jako konrola irc, ale je třeba zaručit minimální dobu mezi obsluhou lcd.
 * Poběží tedy v závislosti na počtu cyklů kontoly irc, ale spouštět se nebude
 * každá n-tý cyklus, ale vždy v normální časové smyčce, až na něj dojde řada.
 */
uint16_t t_lcd = 0;



/** 
 * Toto je hlavní funkce programu. Na počátku jsou provedeny inicializace,
 * pak běží program v nekonečné smyčce. V té jsou postupně volány funkce,
 * které obsluhují LCD, klávesnici, zajišťují měření atd.
 *
 * část funkcí (např. obsluha LCD, nebo kontrola IRC) je časově kritická. Jejich
 * volání je proto prováděno v závislosti na časocači.
 */
int main(int argc, char * argv[]) {
  //char c = 0;
  //char i;

  char * port = NULL;
  char * str = NULL;

  load_conf();
  set_log_file((str = param_gets("log_file") != NULL ? str : DEFAULF_LOGFILE));
  new_log("Start");


#ifndef PCDEBUG
  port = param_gets("port");
  if (port == NULL) {
    fputs("Nebyl zadan port", stderr);
    return -1;
  }


  open_port(port);
  printf(port);
#endif
#ifdef MODUL_DATABASE
  sql_params(param_gets("db_user"), param_gets("db_psswd"), "Pila");

#endif


#ifdef MODUL_HARDWARE
  if ((argc > 1) && (argv[1][1] == 'c')) { /* parametry TODO */
    hardware_config();
  }
#endif
  //sql_query("select * from Zakaznik;");


  /*  for(i=0 ; i <200;i++){
      _delay_ms(1);
    }
   */
  // ioinit prehodit lcd_control, zapnout cas na zacatku
  /*ioinit();

 
  
  eeprom_load();
   */
  gui_init();

  //keyboard_init();

  buttons_init();
  keyboard_init();
  msr_init();
#ifdef MODUL_CAMERA
  camera_init("http://192.168.1.20/snapshot.jpg", "");
#endif

  datf_load();
  vizualizace_init();
  // test int
  while (1) {
    //c++;

    gui_control();
    lcd_control();
    vizualizace();
   //zmena_na_lcd =0;
    if (time4slowloop() > 0) {
      io_all_lcd2vozik();
      #ifdef MODUL_CAMERA
      camera();
      #endif
    }
    

    io_lcd2vozik();
    read_port();
#ifdef PCDEBUG
    //read_key_ncurses();
#endif
    measure();

    err_control();
  }
#ifdef MODUL_CAMERA
  camera_close();
  #endif
}




/* ------------------ */


/**
 * Funkce slouží pro načtení řetězce z programové paměti. Funkce naalokuje
 * pamť pro uložení řetězce do SRAM, řetězec tam zkopíruje a vrátí ukazatel na
 * toto místo.
 * @param s_p Ukazatel do programové paměti.
 * @return Ukazatel v paměti SRAM, kam je zkopírován řetězec z \a s_p
 **/
char * getstr_P( char* s_p){
  char * p_pom;

  p_pom = (char *) mmalloc(strlen(s_p) + 1);
  if(p_pom == NULL){
    new_error(88);
    return NULL;
  }
  else{
    strcpy(p_pom, s_p);
  }

  return p_pom;
}

/*
/// Má alokační funkce. Testuje, zda nebyla naplněna pamť. Pokud ano, vypše
/// chybovou hlákšu.
void * mmalloc (unsigned int size){
  void * pointer;
  pointer = mmalloc (size);
  if (pointer == NULL){
    new_error(88);
    return NULL;
  }
  else {
    return pointer;
  }
}

*/

/**
Tato funkce je volána při startu jednočipu. Jejím úkolem je získat data z katru.
*/
void get_data_from_katr(void){
/*
    char * msg1;
    char * msg2;
    msg1 =(void*) mmalloc (DATASIZE);

    msg1[0] = '\x09';
    msg1[1] = (unsigned char) 1;
    new_msg(msg1);


    msg2 =(void*) mmalloc (DATASIZE);
    
    msg2[0] = '\x16';
    msg2[1] = (unsigned char) 1;
    new_msg(msg2);
*/
}

void err102(void){
  new_error(102); // chyba při komunikaci se serverem
}

void prazdna(char* s){
}

/**
Funkce je volána v případě, že je nastaveno získávání dat o tloušťce ze serveru.
Zajišťuje výzvu k odesílání pro server a čekání na odpověď.
@todo Přepsat. Chce to zajistit umlčení katru.
*/
void get_data_from_server(void){
/*  char * msg = (char *)mmalloc(DATASIZE);
  msg[0] = '\x46';
  new_ack_msg(msg, 0,prazdna,err102);
 */
}


/* end of main.c */
