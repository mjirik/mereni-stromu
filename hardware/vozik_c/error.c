/*
 * $Id: error.c 148 2009-10-25 13:54:05Z mjirik $
 *
 * Modul poskytuje funkci, která vypíše na obrazovku errorů příslušnou chybu.
 * 
 */
/**
 * @file error.c
 * @brief
 * Modul spravuje výpisy chybových hlášek. Chybové hlášky jsou uloženy
 * v programové paměti. V případě potřeby jsou načteny do SRAM a vypsány
 * na LCD.
 *
 * Od indexu 150 jsou errory s dynamickým řetězcem.
 * 
 * Kromě chyb jsou prostřednictvím prostředků tohoto modulu předávány obsluze i
 * ryze "nechybové" zprávy. Jsou uloženy od indexu 200.
 */

#include <inttypes.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "lcd.h"
//#include "gui.h"
#include "main.h"
#include "keyboard.h"
//#include "keyfcn.h"
//#include "msr_scr.h"
//#include "measure.h"
//#include "data_serial.h"
#include "memory.h"
#include "usart.h"
#include "common/msgn.h"
#include "common/errn.h"
#include "sysinfo.h"

#define ERR_TIME 300

/// @name Chybové výpisy
/// Jsou uloženy v programové paměti. Je to kvůli úspoře
/// paměti SRAM.
/// @{
static char err1[] PROGMEM = "Error 1: Doslo k chybe v kontrolnim\nsouctu pri prijmu zpravy na katru. \nNemusi to znamenat zadne potize.";
static char err4[] PROGMEM = "Error 4: Chyba IRC\nRychle otoceni IRC na katru";
static char err6[] PROGMEM = "Error 6: Chyba IRC\nRychle otoceni IRC na voziku";
static char err7[] PROGMEM = "Error 7: Plna odesilaci fronta na katru.\nNemusi to znamenat zadny problem.";
static char err10[] PROGMEM = "Error 10: \n DEBUG";
static char err11[] PROGMEM = "Error 11: \n DEBUG 2";
static char err80[] PROGMEM = "Error 80: Chyba LCD\nPrekroceni rozsahu\nLCD po odradkovani";
static char err81[] PROGMEM = "Error 81: Chyba LCD\nText presahuje\nokraj LCD";
static char err82[] PROGMEM = "Error 82: Chyba LCD\nPocatek textu\nmimo displej";
static char err83[] PROGMEM = "Error 83: Chyba IRC\nOprava na referen.\nbode o vice nez\n 10 cm";
static char err84[] PROGMEM = "Error 84: \nNelze zahajit mereni\ndokud neni\nukonceno predchozi.";
static char err85[] PROGMEM = "Error 85: Chyba konverze int2string.\nPrevadene cislo je prilis dlouhe.\nDochazi k pretekani ve vypisu."; //convert
static char err86[] PROGMEM = "Error 86: Doslo k chybe v kontrolnim\nsouctu pri prijmu zpravy\nna voziku.\nNemusi to znamenat zadne potize.";
static char err87[] PROGMEM = "Error 87: Pri prijmu zpravy na\nvoziku byla komunikace zahajena\nnesprvnym znakem";
static char err88[] PROGMEM = "Error 88: Nedostatek pameti";
static char err89[] PROGMEM = "Error 89: \nPrekrocen rozsah ulozitelne delky.\nMaximalni ulozitelna delka je 25.5 m";
static char err90[] PROGMEM = "Error 90: \nPrekrocen rozsah ulozitelneho prumeru.\nMaximalni ulozitelny prumer je 63.5 cm";
static char err91[] PROGMEM = "Error 91: \nPrekrocena maximalni doba odezvy na \nzpravu.";
static char err92[] PROGMEM = "Error 92: \nV dusledku odjezdu voziku od katru \nse nepodarilo provest dostatek mereni.\nNamerena data mohou byt nepresna.";
static char err93[] PROGMEM = "Error 93: \nV dusledku odjezdu voziku od katru, \nnebylo zajisteno dostatek dat.\nMereni je zruseno.";
static char err94[] PROGMEM = "Error 94: \nJe nutne nejdrive zadat jmeno zakaznika.";
static char err95[] PROGMEM = "Error 95: Zakaznika nelze odstranit. \nBylo pro nej jiz namereno nekolik kmenu.\nPred odstranenim je nutne tato mereni\nsmazat.";
static char err96[] PROGMEM = "Error 96: \nDoslo k preplneni odesilaciho bufferu \nserioveho kanalu na katru. Nemusi to \nznamenat zadne potize.";
//error 97 - kritická chyba - reset - implementováno ve watchdog.c
/// error 98 - výpadek napájení - reset - implementováno v main.c
static char err99[] PROGMEM = "Error 99: \nPokus o mereni prilis kratkeho kmenu.\nNejkratsi meritelny kmen je 2m.";
static char err100[] PROGMEM = "Error 100: \nNespravne zadane cislo.";
static char err101[] PROGMEM = "Error 101: Pamet zaznamu zaplnena.\nNelze ukladat dalsi zaznamy. Odeslete\ndata na server.";
static char err102[] PROGMEM = "Error 102: \nChyba pri komunikaci se serverem.";
// 103 přetížení reciveru

//error 150 - dynamické errory
static char err200[] PROGMEM = "\n             Data odeslana.";
static char err201[] PROGMEM = "\nPamet zaznamu temer zaplnena.\nZbyva misto na mene nez 10 zaznamu.";
/// @}

//static int8_t prev_scr = 0;
static uint8_t last_err = 255;
static char * err_str = NULL;
static char new_err = 0; ///<- proměnná indikuje, zda má být při příštím volání err_control() vyhlášen chybový stav.


char * error_text(uint8_t err_num);
void print_err(void);

/**
 * Funkce, která zajišťuje asynchroní vypisování errorů. Dříve totiž mohlo dojít k
 * přerušení běhu programu uprostřed vykonávání některé funkce a to vedlo k potížím s
 * korektním navracením systémových prostředků.
 */
void err_control(void){
  /*
  if (new_err == 1){
    scr_ch(ERR_SCR);
    new_err = 0;
  }*/
}

void err_scr_open(void){
//   prev_scr = akt_scr;
}

/// Funkce vytváří novou chybu. Dojde k přepnutí na
/// "error screen" a k výpisu textu popisujícího tuto chybu.
/// @param err_num Identifikuje číslem chybovou hlášku.
void new_error(uint8_t err_num){
  char * msg;

  last_err = err_num;
  new_err = 1;
  
  si_print_error(err_num);



    msg =(void*) mmalloc (DATASIZE);
    msg[0] = MSGN_ERR;
    msg[1] = err_num;
    new_msg(msg);


  //prev_scr = akt_scr;
  //scr_ch(ERR_SCR);
 
}


/**
 * Funkce vytvoří nový error. Jeho text je zadán v parametru s.
 */
void new_str_error(uint8_t err_num, char * s){
  assert(err_num >= 150);
  last_err = err_num;
  err_str = s;
  //scr_ch(ERR_SCR);

}

void print_err(void){
  
  //error_scr_draw();
  switch(last_err){
    case 1: 
      printxyd_P(0,0,(void *)err1);
      break;
    case 4:
      printxyd_P(0,0,(void *)err4);
      break;
    case 6:
      printxyd_P(0,0,(void *)err6);
      break;
    case 7:
      printxyd_P(0,0,(void *)err7);
      break;
    case 10:
      printxyd_P(0,0,(void *)err10);
      break;
    case 11:
      printxyd_P(0,0,(void *)err11);
      break;
    case 80: // lcd.c chyba LCD - překročení rozsahu displeje po odřádkování
      printxyd_P(0,0,(void *)err80);
      break;
    case 81: // lcd.c
      printxyd_P(0,0,(void *)err81);
      break;
    case 82: // lcd.c
      printxyd_P(0,0,(void *)err82);
      break;
    case 83: // irc.c
      printxyd_P(0,0,(void *)err83);
      break;
    case 84: // measure.c
      printxyd_P(0,0,(void *)err84);
      break;
    case 85:
      printxyd_P(0,0,(void *)err85);
      break;
    case 86:
      printxyd_P(0,0,(void *)err86);
      break;
    case 87:
      printxyd_P(0,0,(void *)err87);
      break;
    case 88:
      printxyd_P(0,0,(void *)err88);
      break;
    case 89:
      printxyd_P(0,0,(void *)err89);
      break;
    case 90:
      printxyd_P(0,0,(void *)err90);
      break;
    case 91:
      printxyd_P(0,0,(void *)err91);
      break;
    case 92:
      printxyd_P(0,0,(void *)err92);
      break;
    case 93:
      printxyd_P(0,0,(void *)err93);
      break;
    case 94:
      printxyd_P(0,0,(void *)err94);
      break;
    case 95:
      printxyd_P(0,0,(void *)err95);
      break;
    case 96:
      printxyd_P(0,0,(void *)err96);
      break;
    case 99:
      printxyd_P(0,0,(void *)err99);
      break;
    case 100:
      printxyd_P(0,0,(void *)err100);
      break;
    case 101:
      printxyd_P(0,0,(void *)err101);
      break;
    case 102:
      printxyd_P(0,0,(void *)err102);
      break;
    case 200:
      printxyd_P(0,0,(void *)err200);
      break;
    case 201:
      printxyd_P(0,0,(void *)err201);
      break;
    default:
      break;
  }
}


void error_scr_draw(void){
  /*if (prev_scr == ERR_SCR){
    prev_scr = MSR_SCR;
  }*/

  clear_dscreen();
  //akt_scr = ERR_SCR;
  if ((last_err >= 150)&&(last_err < 200)){
    printxyd(0,0,err_str);
    mfree((void*)err_str);
    err_str = NULL;
  }
  else{
    print_err();
  }
}

void error_scr_control(void){
//  char c;
  //static uint32_t pocitadlo_1 = 0;

  /*
  pocitadlo_1++;
  if (pocitadlo_1 > ERR_TIME){
    pocitadlo_1 = 0;
    scr_ch(prev_scr);
  }
  */
/*
  if ((c = get_spec()) != 0){
    switch (c){
    case 4: //Enter
    case 5: //Esc
      scr_ch(prev_scr);
      //measure_scr_draw();
      break;
    case 21: // F11 - Strorno měření
      kf_zrusit();
      break;
    case 22: //F12
      // Odeslání dat.
      send_data();
      break;
    default:
      break;
    }
  }
  */
}


/**
 * Funkce je volána v případě že makro assert() nalezlo nějakou chybu.
 * Funkce vyvtoří řeťězec, v němˇž je popsáno jméno funkce, jméno
 * souboru a řádek, z něhož je makro spouštěno.
 */
void err_assert(char * soubor, int radek){
  char * str = (char *)mmalloc(strlen(soubor) + 20 + 4 + 3);
  
  sprintf(str,"Error 150: assert()\n%s\n%i",soubor,radek);
  new_str_error(150,str);
}


/* end of error.c */
