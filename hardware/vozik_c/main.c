/*
 * $Id: main.c 120 2007-09-21 05:43:42Z Mira $
 */
/**
 * @file main.c
 * @brief 
 * Tento modul je hlavní hlavní èástí programu pro zaøízení na vozíku v rámci
 * projektu mìøení objemu øeziva pro pilu v Nìmèicích. Zaøízení na vozíku má
 * za úkol zpracovávat informace, které dostává po sériové lince ze zaøízení na
 * katru, komunikovat s navazujícím databázovım systémem, obsluhovat mìøení
 * vzdálenosti vozíku od katru pomoocí IRC a zajišovat komunikaci s uivatelem
 * prostøednictvím znakového displeje (4x20 znakù) a standartní poèítaèové
 * klávesnice.
 *
 * V tomto modulu je hlavní funkce main. Jednotlivé èásti programu jsou volány
 * z ní. Nejprve probíhají nutné inicializace ostatních modulù. Pak program
 * vejde do nekoneèné smyèky, která opakovanì volá funkce pro obsluhu
 * jednotlivıch èástí. Obsluhu inkrementálního èidla je nutné provádìt co moná
 * nejèastìji. Dùleité je hlavnì nepøekroèení urèitého èasu mezi voláním
 * obsluné funkce. Ta je proto volána prostøednictvím pøerušení èítaèe. Volání
 * displeje zase naopak vyaduje mezi voláním urèitı èas. Proto je øešeno v
 * závislosti na poètu pøerušení generovanıch èítaèem(kterı je pouíván i pro
 * IRC).
 *
 * Pro pøípad vıpadku napájení je v obvodu zaøazen záloní kondenzátor. Ten
 * zajistí napájení obvodu po nìjakı èas bìhem vıpadku. V tomto èase dochází k
 * ukládání ivotnì dùleitıch dat do EEPROM. To je øešeno ve funkci
 * test_shutdown(). Pokud dojde k obnovení napájení pøed vyèerpáním záloních
 * zdrojù, fuunkce vyèká, aby napìtí na kondenzátoru vystoupilo nad 4.5 V
 * (kvùli LCD), znovu inicializuje LCD a obnoví provoz zaøízení.
 */


#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "main.h"
#include "usart.h"
#include "irc.h"
#include "eeprom.h"
#include "measure.h"
#include "lcd.h"
#include "convert.h"
#include "data.h"
#include "gui.h"
#include "error.h"
#include "keyboard.h"
#include "dat_scr.h"
#include "buttons.h"
#include "watchdog.h"


//#define SizeOfMsg 5
#define F_CPU 8000000UL ///< Frekvence oscilátoru 8MHz
#define T_LCD_MAX 5 ///< Kolikrát pomaleji bude bìet pøekreslování lcd oproti kontrole irc





/**
 * LCD není èasovì tak kritické jako konrola irc, ale je tøeba zaruèit minimální dobu mezi obsluhou lcd.
 * Pobìí tedy v závislosti na poètu cyklù kontoly irc, ale spouštìt se nebude
 * kadı n-tı cyklus, ale vdy v normální èasové smyèce, a na nìj dojde øada.
 */
uint16_t t_lcd = 0;


static char err98[] PROGMEM = "Error 98: Doslo ke krátkému vıpadku     napájení. Data ulozena. Reset.";


void get_data_from_katr(void);


/**
 * Funkce pøi svém zavolání ovìøí, zda je na pøísluèném pinu napìtí (napájení).
 * Pokud ne, zahájí se ukládání dat do eeprom.
 */
void test_shutdown(void){
  uint8_t time = 0;
  uint16_t i = 0;
//  char * s = __FILE__;

  if (((PIND >> 2) & (0x01)) == 0){
    eeprom_save();
    //wtch_disable2();

    //Tenhle while bìhá dokola. Pokud alespò na 5 ms naskoèí na pinu 2 portu d
    //jednièka, èekání skonèí.
    while(1){
      _delay_ms(1);
      wtch_reset2();

      if (((PIND >> 2) & (0x01)) == 0)
        time = 0;
      else
        time++;

      if (time > 5)
        break;
    }

    // poèkáme vteøinku
    // to je kvùli tomu, aby napští na kondenzátoru vylezlo nad 4.5 V, co je nutné pro chod LCD
    for(i=0 ; i < 500;i++){
      wtch_reset2();
      _delay_ms(1);
    }
    lcd_w_inst(0x01);
    printnt_P((void *)err98); // Vıpadek napájení - reset.

    for(i=0 ; i < 5000;i++){
       wtch_reset2();
      _delay_ms(1);
      
    }

    while (1){
    }
    //znovu nainicializujeme LCD
    //lcd_init();


    //scr_ch(MSR_SCR); // zajistí vypsání popiskù ("Zakaz:", "Drevo:" ,atd.
    //wtch_enable();
    // a pokraèujeme v programu
  }
}

/// Obsluha inkrementálního èidla je volána v pøerušení èítaèe
SIGNAL (SIG_OUTPUT_COMPARE1A) {
  t_lcd++;
  
  irc_watch();
  wtch_main();
  
}



/// Inicializace èítaèe pro volání inkrementálního èidla
void ioinit (void) {
  int pocet_cyklu = 500;

  // citac bude citat v kadém cyklu, reim - Clear Timer on Compare (CTC)
  TCCR1B = (1 << CS10) | (1 << WGM12);
  // nastaveni pøerušení pøi porovnání
  TIMSK = (1 << OCIE1A);

  // Nastavení jak èasto se bude volat 
  OCR1AH = (unsigned char)(pocet_cyklu >> 8);
  OCR1AL = (unsigned char) pocet_cyklu;
}

/** 
 * Toto je hlavní funkce programu. Na poèátku jsou provedeny inicializace,
 * pak bìí program v nekoneèné smyèce. V Té jsou postupnì volány funkce,
 * které obsluhují LCD, klávesnici, zajiš_tují mìøení atd.
 *
 * Èást funkcí (napø obsluha LCD, nebo kontrola IRC) je èasovì kritická. Jejich
 * volání je proto provádìno v závislosti na èasocaèi.
 */
int main(void){
  char c = 0;
  char i;


  for(i=0 ; i <200;i++){
    _delay_ms(1);
  }

  // ioinit prehodit lcd_control, zapnout cas na zacatku
  ioinit();

  DDRC = 128; //testy
  PORTC=0;
  PORTD = _BV(7);
  
  irc_init();
  lcd_init();
  
  eeprom_load();

  gui_init();

  keyboard_init();

  buttons_init();
  USART_Init(51);
  msr_init();
  wtch_init();



  sei();

  get_data_from_katr();
  while(1){
    c++;

    if (t_lcd > T_LCD_MAX){
      lcd_control();
      t_lcd = 0;
      gui_control();
      keyboard_recv_watchdog();
      serial_msg_ack_timer();
     
      
    } 

    buttons();
    measure();
    test_shutdown();
    err_control();
    wtch_reset2();
    //my_watchdog_counter = 0; //my_watchdog
  }
}




/* ------------------ */


/**
 * Funkce slouí pro naètení øetìzce z programové pamìti. Funkce naalokuje
 * pamì pro uloení øetìzce do SRAM, øetìzec tam zkopíruje a vrátí ukazatel na
 * toto místo.
 * @param s_p Ukazatel do programové pamìti.
 * @return Ukazatel v pamìti SRAM, kam je zkopírován øetìzec z \a s_p
 **/
char * getstr_P( PGM_P s_p){
  char * p_pom;

  p_pom = (char *) malloc(strlen_P(s_p) + 1);
  if(p_pom == NULL){
    new_error(88);
    return NULL;
  }
  else{
    strcpy_P(p_pom, s_p);
  }

  return p_pom;
}

/// Má alokaèní funkce. Testuje, zda nebyla naplnìna pamì. Pokud ano, vypíše
/// chybovou hlášku.
void * mmalloc (unsigned int size){
  void * pointer;
  pointer = malloc (size);
  if (pointer == NULL){
    new_error(88);
    return NULL;
  }
  else {
    return pointer;
  }
}



/**
Tato funkce je volána pøi startu jednoèipu. Jejím úkolem je získat data z katru.
*/
void get_data_from_katr(void){
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
}

void err102(void){
  new_error(102); // chyba pøi komunikaci se serverem
}

void prazdna(char* s){
}

/**
Funkce je volána v pøípadì, e je nastaveno získávání dat o šíøce ze serveru.
Zajišuje vızvu k odesílání pro server a èekání na odpovìï.
@todo Pøepsat. Chce to zajistit umlèení katru.
*/
void get_data_from_server(void){
  char * msg = (char *)mmalloc(DATASIZE);
  msg[0] = '\x46';
  new_ack_msg(msg, 0,prazdna,err102);
}


/* end of main.c */
