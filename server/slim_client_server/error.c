/*
 * $Id: error.c 131 2009-09-25 23:25:40Z mjirik $
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
//#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "lcd.h"
#include "gui.h"
#include "main.h"
#include "keyboard.h"
#include "keyfcn.h"
#include "msr_scr.h"
#include "measure.h"
#include "data_serial.h"
#include "memory.h"
#include "errn.h"

// global
void new_str_error(unsigned int err_num, char * s);
void new_error(unsigned int err_num);
void error_scr_draw(void);
void error_scr_control(void);
void err_assert(char * soubor, int radek);
void err_scr_open(void);
void err_control(void);
// global

#define ERR_TIME 300

/// @name Chybové výpisy
/// Jsou uloženy v programové paměti. Je to kvůli úspoře
/// paměti SRAM.
/// @{
static char err1[] = "Error 1: Doslo k chybe v kontrolnim\nsouctu pri prijmu zpravy na katru. \nNemusi to znamenat zadne potize.";
static char err4[]  = "Error 4: Chyba IRC\nRychle otoceni IRC na katru";
static char err6[]  = "Error 6: Chyba IRC\nRychle otoceni IRC na voziku";
static char err7[]  = "Error 7: Plna odesilaci fronta na katru.\nNemusi to znamenat zadny problem.";
static char err10[]  = "Error 10: \n DEBUG";
static char err11[]  = "Error 11: \n DEBUG 2";
static char err80[]  = "Error 80: Chyba LCD\nPrekroceni rozsahu\nLCD po odradkovani";
static char err81[]  = "Error 81: Chyba LCD\nText presahuje\nokraj LCD";
static char err82[]  = "Error 82: Chyba LCD\nPocatek textu\nmimo displej";
static char err83[]  = "Error 83: Chyba IRC\nOprava na referen.\nbode o vice nez\n 10 cm";
static char err84[]  = "Error 84: \nNelze zahajit mereni\ndokud neni\nukonceno predchozi.";
static char err85[]  = "Error 85: Chyba konverze int2string.\nPrevadene cislo je prilis dlouhe.\nDochazi k pretekani ve vypisu."; //convert
static char err86[]  = "Error 86: Doslo k chybe v kontrolnim\nsouctu pri prijmu zpravy\nna voziku.\nNemusi to znamenat zadne potize.";
static char err87[]  = "Error 87: Pri prijmu zpravy na\nvoziku byla komunikace zahajena\nnesprvnym znakem";
static char err88[]  = "Error 88: Nedostatek pameti";
static char err89[]  = "Error 89: \nPrekrocen rozsah ulozitelne delky.\nMaximalni ulozitelna delka je 25.5 m";
static char err90[]  = "Error 90: \nPrekrocen rozsah ulozitelneho prumeru.\nMaximalni ulozitelny prumer je 63.5 cm";
static char err91[]  = "Error 91: \nPrekrocena maximalni doba odezvy na \nzpravu.";
static char err92[]  = "Error 92: \nV dusledku odjezdu voziku od katru \nse nepodarilo provest dostatek mereni.\nNamerena data mohou byt nepresna.";
static char err93[]  = "Error 93: \nV dusledku odjezdu voziku od katru, \nnebylo zajisteno dostatek dat.\nMereni je zruseno.";
static char err94[]  = "Error 94: \nJe nutne nejdrive zadat jmeno zakaznika.";
static char err95[]  = "Error 95: Zakaznika nelze odstranit. \nBylo pro nej jiz namereno nekolik kmenu.\nPred odstranenim je nutne tato mereni\nsmazat.";
static char err96[]  = "Error 96: \nDoslo k preplneni odesilaciho bufferu \nserioveho kanalu na katru. Nemusi to \nznamenat zadne potize.";
//error 97 - kritická chyba - reset - implementováno ve watchdog.c
/// error 98 - výpadek napájení - reset - implementováno v main.c
static char err99[]  = "Error 99: \nPokus o mereni prilis kratkeho kmenu.\nNejkratsi meritelny kmen je 2m.";
static char err100[]  = "Error 100: \nNespravne zadane cislo.";
static char err101[]  = "Error 101: Pamet zaznamu zaplnena.\nNelze ukladat dalsi zaznamy. Odeslete\ndata na server.";
static char err102[]  = "Error 102: \nChyba pri komunikaci se serverem.";

//error 150 - dynamické errory
static char err200[]  = "\n             Data odeslana.";
static char err201[]  = "\nPamet zaznamu temer zaplnena.\nZbyva misto na mene nez 10 zaznamu.";
/// @}

int8_t prev_scr = 0;
unsigned int last_err = 255;
char * err_str = NULL;
char new_err = 0;  ///<- proměnná indikuje, zda má být při příštím volání err_control() vyhlášen chybový stav.

char * error_text(unsigned int err_num);
void print_err(void);

/**
 * Funkce, která zajišťuje asynchroní vypisování errorů. Dříve totiž mohlo dojít k
 * přerušení běhu programu uprostřed vykonávání některé funkce a to vedlo k potížím s
 * korektním navracením systémových prostředků.
 */void err_control(void){
  if (new_err == 1){
    scr_ch(ERR_SCR);
    new_err = 0;
  }
}

void err_scr_open(void){
   prev_scr = akt_scr;
}

/// Funkce vytváří novou chybu. Dojde k přepnutí na
/// "error screen" a k výpisu textu popisujícího tuto chybu.
/// @param err_num Identifikuje číslem chybovou hlášku.
void new_error(unsigned int err_num){
  last_err = err_num;
  new_err = 1;
  //prev_scr = akt_scr;
  //scr_ch(ERR_SCR);
 
}


/**
 * Funkce vytvoří nový error. Jeho text je zadán v parametru s.
 */
void new_str_error(unsigned int err_num, char * s){
  assert(err_num >= 150);
  last_err = err_num;
  err_str = s;
  scr_ch(ERR_SCR);

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
    case ERRN_NO_CUSTOMER:
      printxyd(0,0, "ERR 106: \nNeni zadan zadny zakaznik. \nJe nutno jej vytvorit.");
      break;
    default:
      break;
  }
}


void error_scr_draw(void){
  if (prev_scr == ERR_SCR){
    prev_scr = MSR_SCR;
  }

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
  char c;
  //static uint32_t pocitadlo_1 = 0;

  /*
  pocitadlo_1++;
  if (pocitadlo_1 > ERR_TIME){
    pocitadlo_1 = 0;
    scr_ch(prev_scr);
  }
  */

  if ((c = get_spec()) != 0){
    switch (c){
    case 4: //Enter
    case 5: //Esc
      scr_ch(prev_scr);
      //measure_scr_draw();
      break;
    case 21: // F11 - Strorno m��en�
      kf_zrusit();
      break;
    case 22: //F12
      // Odesl�n� dat.
      send_data();
      break;
    default:
      break;
    }
  }
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
