/*
 * $Id: main.c 120 2007-09-21 05:43:42Z Mira $
 */
/**
 * @file main.c
 * @brief 
 * Tento modul je hlavn� hlavn� ��st� programu pro za��zen� na voz�ku v r�mci
 * projektu m��en� objemu �eziva pro pilu v N�m�ic�ch. Za��zen� na voz�ku m�
 * za �kol zpracov�vat informace, kter� dost�v� po s�riov� lince ze za��zen� na
 * katru, komunikovat s navazuj�c�m datab�zov�m syst�mem, obsluhovat m��en�
 * vzd�lenosti voz�ku od katru pomooc� IRC a zaji��ovat komunikaci s u�ivatelem
 * prost�ednictv�m znakov�ho displeje (4x20 znak�) a standartn� po��ta�ov�
 * kl�vesnice.
 *
 * V tomto modulu je hlavn� funkce main. Jednotliv� ��sti programu jsou vol�ny
 * z n�. Nejprve prob�haj� nutn� inicializace ostatn�ch modul�. Pak program
 * vejde do nekone�n� smy�ky, kter� opakovan� vol� funkce pro obsluhu
 * jednotliv�ch ��st�. Obsluhu inkrement�ln�ho �idla je nutn� prov�d�t co mo�n�
 * nej�ast�ji. D�le�it� je hlavn� nep�ekro�en� ur�it�ho �asu mezi vol�n�m
 * obslu�n� funkce. Ta je proto vol�na prost�ednictv�m p�eru�en� ��ta�e. Vol�n�
 * displeje zase naopak vy�aduje mezi vol�n�m ur�it� �as. Proto je �e�eno v
 * z�vislosti na po�tu p�eru�en� generovan�ch ��ta�em(kter� je pou��v�n i pro
 * IRC).
 *
 * Pro p��pad v�padku nap�jen� je v obvodu za�azen z�lo�n� kondenz�tor. Ten
 * zajist� nap�jen� obvodu po n�jak� �as b�hem v�padku. V tomto �ase doch�z� k
 * ukl�d�n� �ivotn� d�le�it�ch dat do EEPROM. To je �e�eno ve funkci
 * test_shutdown(). Pokud dojde k obnoven� nap�jen� p�ed vy�erp�n�m z�lo�n�ch
 * zdroj�, fuunkce vy�k�, aby nap�t� na kondenz�toru vystoupilo nad 4.5 V
 * (kv�li LCD), znovu inicializuje LCD a obnov� provoz za��zen�.
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
#define F_CPU 8000000UL ///< Frekvence oscil�toru 8MHz
#define T_LCD_MAX 5 ///< Kolikr�t pomaleji bude b�et p�ekreslov�n� lcd oproti kontrole irc





/**
 * LCD nen� �asov� tak kritick� jako konrola irc, ale je t�eba zaru�it minim�ln� dobu mezi obsluhou lcd.
 * Pob�� tedy v z�vislosti na po�tu cykl� kontoly irc, ale spou�t�t se nebude
 * ka�d� n-t� cyklus, ale v�dy v norm�ln� �asov� smy�ce, a� na n�j dojde �ada.
 */
uint16_t t_lcd = 0;


static char err98[] PROGMEM = "Error 98: Doslo ke kr�tk�mu v�padku     nap�jen�. Data ulozena. Reset.";


void get_data_from_katr(void);


/**
 * Funkce p�i sv�m zavol�n� ov���, zda je na p��slu�n�m pinu nap�t� (nap�jen�).
 * Pokud ne, zah�j� se ukl�d�n� dat do eeprom.
 */
void test_shutdown(void){
  uint8_t time = 0;
  uint16_t i = 0;
//  char * s = __FILE__;

  if (((PIND >> 2) & (0x01)) == 0){
    eeprom_save();
    //wtch_disable2();

    //Tenhle while b�h� dokola. Pokud alesp� na 5 ms nasko�� na pinu 2 portu d
    //jedni�ka, �ek�n� skon��.
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

    // po�k�me vte�inku
    // to je kv�li tomu, aby nap�t� na kondenz�toru vylezlo nad 4.5 V, co� je nutn� pro chod LCD
    for(i=0 ; i < 500;i++){
      wtch_reset2();
      _delay_ms(1);
    }
    lcd_w_inst(0x01);
    printnt_P((void *)err98); // V�padek nap�jen� - reset.

    for(i=0 ; i < 5000;i++){
       wtch_reset2();
      _delay_ms(1);
      
    }

    while (1){
    }
    //znovu nainicializujeme LCD
    //lcd_init();


    //scr_ch(MSR_SCR); // zajist� vyps�n� popisk� ("Zakaz:", "Drevo:" ,atd.
    //wtch_enable();
    // a pokra�ujeme v programu
  }
}

/// Obsluha inkrement�ln�ho �idla je vol�na v p�eru�en� ��ta�e
SIGNAL (SIG_OUTPUT_COMPARE1A) {
  t_lcd++;
  
  irc_watch();
  wtch_main();
  
}



/// Inicializace ��ta�e pro vol�n� inkrement�ln�ho �idla
void ioinit (void) {
  int pocet_cyklu = 500;

  // citac bude citat v ka�d�m cyklu, re�im - Clear Timer on Compare (CTC)
  TCCR1B = (1 << CS10) | (1 << WGM12);
  // nastaveni p�eru�en� p�i porovn�n�
  TIMSK = (1 << OCIE1A);

  // Nastaven� jak �asto se bude volat 
  OCR1AH = (unsigned char)(pocet_cyklu >> 8);
  OCR1AL = (unsigned char) pocet_cyklu;
}

/** 
 * Toto je hlavn� funkce programu. Na po��tku jsou provedeny inicializace,
 * pak b�� program v nekone�n� smy�ce. V T� jsou postupn� vol�ny funkce,
 * kter� obsluhuj� LCD, kl�vesnici, zaji�_tuj� m��en� atd.
 *
 * ��st funkc� (nap� obsluha LCD, nebo kontrola IRC) je �asov� kritick�. Jejich
 * vol�n� je proto prov�d�no v z�vislosti na �asoca�i.
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
 * Funkce slou�� pro na�ten� �et�zce z programov� pam�ti. Funkce naalokuje
 * pam� pro ulo�en� �et�zce do SRAM, �et�zec tam zkop�ruje a vr�t� ukazatel na
 * toto m�sto.
 * @param s_p Ukazatel do programov� pam�ti.
 * @return Ukazatel v pam�ti SRAM, kam je zkop�rov�n �et�zec z \a s_p
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

/// M� aloka�n� funkce. Testuje, zda nebyla napln�na pam�. Pokud ano, vyp�e
/// chybovou hl�ku.
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
Tato funkce je vol�na p�i startu jedno�ipu. Jej�m �kolem je z�skat data z katru.
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
  new_error(102); // chyba p�i komunikaci se serverem
}

void prazdna(char* s){
}

/**
Funkce je vol�na v p��pad�, �e je nastaveno z�sk�v�n� dat o ���ce ze serveru.
Zaji��uje v�zvu k odes�l�n� pro server a �ek�n� na odpov��.
@todo P�epsat. Chce to zajistit uml�en� katru.
*/
void get_data_from_server(void){
  char * msg = (char *)mmalloc(DATASIZE);
  msg[0] = '\x46';
  new_ack_msg(msg, 0,prazdna,err102);
}


/* end of main.c */
