/*
 * $Id: main.c 149 2009-10-25 17:36:42Z mjirik $
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


#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <stdlib.h> // kvuli mmalloc
//#include <stdio.h>
//#include <math.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "main.h"
#include "usart.h"
#include "irc.h"
#include "irc2.h"
#include "eeprom.h"
//#include "measure.h"
#include "lcd.h"
#include "convert.h"
//#include "data.h"
//#include "gui.h"
#include "error.h"
#include "keyboard.h"
//#include "dat_scr.h"
#include "buttons.h"
#include "watchdog.h"
#include "common/msgn.h"
#include "inout.h"
#include "default_scr.h"
#include "memory.h"
#include "sysinfo.h"
#include "common\msgn.h"

// global
//char data_msg[SizeOfMsg];
char str_buf[41];
char * getstr_P( PGM_P s_p);

void my_watchdog(void);

//#define _TICTOC

#ifdef _TICTOC
void tic(void);
uint16_t toc(void);
#endif
// global


//#define SizeOfMsg 5
#define F_CPU 8000000UL ///< Frekvence oscilátoru 8MHz
#define T_LCD_MAX 1 ///< Kolikrát pomaleji bude běžet překreslování lcd oproti kontrole irc





/**
 * LCD není časově tak kritické jako konrola irc, ale je třeba zaručit minimální dobu mezi obsluhou lcd.
 * Poběží tedy v závislosti na počtu cyklů kontoly irc, ale spouštět se nebude
 * každý n-tý cyklus, ale vždy v normální časové smyčce, až na něj dojde řada.
 */
static uint16_t t_lcd = 0;

//static int qqq=0;

static char err98[] PROGMEM = "Error 98: Doslo ke krátkému výpadku     napájení. Data ulozena. Reset.";


void get_data_from_katr(void);
void send_irc_values_to_PC(void);
void send_start_msg(void);

// Funkce pro odlazení
#ifdef _TICTOC
uint16_t t_tictoc;

void tic(void){
  t_tictoc = 0;
}

uint16_t toc(void){
  return t_tictoc;
}

void toclcd(void){
  sprintf(str_buf,"%8d",t_tictoc);
  printxyd(0,20,str_buf);
}
#endif







/**
 * Funkce při svém zavolání ověří, zda je na příslučném pinu napětí (napájení).
 * Pokud ne, zahájí se ukládání dat do eeprom.
 */
void test_shutdown(void){
  uint8_t time = 0;
  uint16_t i = 0;
//  char * s = __FILE__;

  wtch_dbg_info();

  if (((PIND >> 2) & (0x01)) == 0){
    printcxyd(1,1,'x');
    eeprom_save();
    //wtch_disable2();

    //Tenhle while běhá dokola. Pokud alespň na 5 ms naskočí na pinu 2 portu d
    //jednička, čekání skončí.
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

    // počkáme vteřinku
    // to je kvůli tomu, aby napští na kondenzátoru vylezlo nad 4.5 V, což je nutné pro chod LCD
    for(i=0 ; i < 500;i++){
      wtch_reset2();
      _delay_ms(1);
    }
    lcd_w_inst(0x01);
    printnt_P((void *)err98); // Výpadek napájení - reset.

    for(i=0 ; i < 5000;i++){
       wtch_reset2();
      _delay_ms(1);
      
    }

    while (1){
    }
    //znovu nainicializujeme LCD
    //lcd_init();


    //scr_ch(MSR_SCR); // zajistí vypsání popisků ("Zakaz:", "Drevo:" ,atd.
    //wtch_enable();
    // a pokračujeme v programu
  }
}

/// Obsluha inkrementálního čidla je volána v přerušení čítače
SIGNAL (SIG_OUTPUT_COMPARE1A) {
  t_lcd++;
//  qqq++;


  irc1_watch();
  irc2_watch();
  wtch_main();
  
  #ifdef _TICTOC
  t_tictoc++;
  #endif
//wtch_reset2();
  
  
}



/// Inicializace čítače pro volání inkrementálního čidla
void ioinit (void) {
  int pocet_cyklu = 2500;

  // citac bude citat v každém cyklu, režim - Clear Timer on Compare (CTC)
  TCCR1B = (1 << CS10) | (1 << WGM12);
  // nastaveni přerušení při porovnání
  TIMSK = (1 << OCIE1A);

  // Nastavení jak často se bude volat 
  OCR1AH = (unsigned char)(pocet_cyklu >> 8);
  OCR1AL = (unsigned char) pocet_cyklu;
}

/** 
 * Toto je hlavní funkce programu. Na počátku jsou provedeny inicializace,
 * pak běží program v nekonečné smyčce. V Té jsou postupně volány funkce,
 * které obsluhují LCD, klávesnici, zajiš_tují měření atd.
 *
 * Část funkcí (např obsluha LCD, nebo kontrola IRC) je časově kritická. Jejich
 * volání je proto prováděno v závislosti na časocači.
 */
int main(void){
  //char c = 0;
  int i;


  for(i=0 ; i <200;i++){
    _delay_ms(1);
  }

  // ioinit prehodit lcd_control, zapnout cas na zacatku
  ioinit();

  DDRC = 128; //testy
  PORTC = 0;
  PORTD = _BV(7);
  
  irc1_init();
  irc2_init();
  lcd_init();


  // pozor - možná užitečné
  //eeprom_load();

  //gui_init();

  keyboard_init();

  buttons_init();
  USART_Init(51);
  //msr_init();
  
  


  //io_msr_led_on();

  

  //
  //get_data_from_katr();
  printnt("Start");

  for(i=0 ; i <1000;i++){
    _delay_ms(1);
  }

  wtch_init();
  wtch_dbg_info();
  sei();
  
  printxyd(1,5, "Cekani na spojeni...");
  send_start_msg();

  while(1){
//    char st[2]=" ";
    //static char prekreslit = 1;
    static char cc = 0;

    wtch_dbg_info();
    
    si_print_run(cc);
    
    cc++;


    // běží 5x pomaleji než obsluha usart
    //if (t_lcd > T_LCD_MAX){
      //static char counter1 = 0;
      //uint16_t a;
     
     //tic();
 

      //a = t_lcd;
 //     counter1++;


  if (t_lcd > T_LCD_MAX){
    msg_time_pp();
        //printcxyd(2,2,cc);
        lcd_control();
        //printxyd(1,0,__FILE__);
        //printnt(st);
        t_lcd = 0;
      }
      
      keyboard_recv_watchdog();
      //serial_msg_ack_timer();
     
      //běží 25x pomaleji než usart
      //if (counter1 > 1){
        //counter1 = 0;
        //gui_control();

      //sprintf(str_buf, "%d",(int) (get_msg_time()));
      //printxyd(1,1,str_buf);


        // zde je vykreslování obrazovky pokud není po nějakou dobu dosažen server
        //  tato doba je dána konstantou v if níže
        //if (get_msg_time() > 20e4){
      /*
        if (msg_time > 20e4){

          // tohle zajišťuje, že se displej vykreslí jen jednou
          if (prekreslit == 1){
            
            default_scr_draw();
            prekreslit = 0;
          }
          default_scr_control();
    
        }
        else{
          prekreslit = 1;
        }*/



        send_irc_values_to_PC();



        //sprintf(str_buf,"%i",qqq);
        //printxyd(3,15,str_buf);
        //qqq=0;

    buttons();
    
    //measure();
    test_shutdown();
    //err_control();
    sysinfo();
    serial_read_buffer();
    wtch_reset2();
    
    wtch_dbg_info();
    
    //my_watchdog_counter = 0; //my_watchdog
  }
}


void send_irc_values_to_PC(void){
  static uint16_t irc1_tmp = 0;
  static uint16_t irc2_tmp = 0;
  uint16_t irc1;
  uint16_t irc2;

  
  wtch_dbg_info();

  irc1 = irc1_get_l();
  irc2 = irc2_get_l();

  // došlo ke změně
  if ((irc1_tmp != irc1)||(irc2_tmp != irc2)){
    send_uint32(MSGN_IRC,(uint32_t) irc1, (uint32_t)irc2);

    //send_msg...
    irc1_tmp = irc1;
    irc2_tmp = irc2;
  }
   wtch_dbg_info();
}

/* ------------------ */


/**
 * Funkce slouží pro načtení řetězce z programové paměti. Funkce naalokuje
 * paměť pro uložení řetězce do SRAM, řetězec tam zkopíruje a vrátí ukazatel na
 * toto místo.
 * @param s_p Ukazatel do programové paměti.
 * @return Ukazatel v paměti SRAM, kam je zkopírován řetězec z \a s_p
 **/
char * getstr_P( PGM_P s_p){
  char * p_pom;

  p_pom = (char *) mmalloc(strlen_P(s_p) + 1);
  if(p_pom == NULL){
    new_error(88);
    return NULL;
  }
  else{
    strcpy_P(p_pom, s_p);
  }

  return p_pom;
}



/**
Tato funkce je volána při startu jednočipu. Jejím úkolem je získat data z katru.
*/
void send_start_msg(void){
    char * msg1;

    msg1 =(void*) mmalloc (DATASIZE);

    msg1[0] = MSGN_START;
    msg1[1] = (unsigned char) 1;
    new_msg(msg1);

}

/**
Tato funkce je volána při startu jednočipu. Jejím úkolem je získat data z katru.
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
  new_error(102); // chyba při komunikaci se serverem
}

void prazdna(char* s){
}

/**
Funkce je volána v případě, že je nastaveno získávání dat o šířce ze serveru.
Zajišťuje výzvu k odesílání pro server a čekání na odpověď.
@todo Přepsat. Chce to zajistit umlčení katru.
*/
void get_data_from_server(void){
  char * msg = (char *)mmalloc(DATASIZE);
  msg[0] = '\x46';
  new_ack_msg(msg, 0,prazdna,err102);
}


/* end of main.c */
