/* $Id: dat_scr.c 120 2007-09-21 05:43:42Z Mira $
 */

/**
 * \file dat_scr.c
 * \brief
 * Modul zajišťuje uživatelské rozhraní pro prohlížení a editaci
 * naměřených záznamů.
 */

#include <string.h>
//#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
//#include <avr/wdt.h>

#include "dat_scr.h"
#include "hlp_scr.h"
#include "msr_scr.h"
//#include "lst_scr.h"
#include "gui.h"
#include "gui_toolbox.h"
#include "lcd.h"
#include "main.h"
#include "keyboard.h"
#include "convert.h"
#include "data.h"
#include "error.h"
#include "errn.h"
#include "measure.h"
#include "data_serial.h"
#include "keyfcn.h"
#include "memory.h"
//#include "watchdog.h"

// global
void data_scr_init(void);
void data_scr_control(void);
void data_scr_draw(void);
void data_scr_open(void);
void data_scr_close(void);
//global

/// Definuje znak kurzoru
#define CURSOR 255
//#define CURSOR '\xDB'

#define MAX_STR_DELKA 4
#define MAX_STR_PRUMER 3

static void menu_open(void);
static void menu_close(void);
static void vypis_seznam(void);
static void control1(void);
static void control_edit(void);
static void control_prumer(void);
static void control_delka(void);
void control_ok(void);
static void menu_str_delka(void);
static void menu_str_prumer(void);

#define VELIKOST_MENU 7
static MENU_ITEM *menu[VELIKOST_MENU];


static char s_smazat_mereni[]  = " \n  Opravdu chcete odstranit mereni?";

/// pomocné proměnné, používané během zadávání
/// je-li záporná, nebyla zadána
///@{
static int zadana_delka = -1;
static int zadany_prumer = -1;
///@}

/* Jména druhů dřev měnit i v msr_scr.c */ // Pozor toto jeste nefunguje
 char* smrk  = "Smrk";
 char* boro_k  = "BorK";
 char* boro_b  = "BorB";
 char* buk  = "Buk ";
 char* dub  = "Dub ";
 char* n1  = "1";
 char* n2  = "2";
 char* nic  = "";
 char* ctyr  = "samo";
 char* paso  = "paso";
 char* man  = "man";

//static char* pole[2] PROGMEM = {"totooooo" ,"tamto"};

//static char s_prazdne[]  = "Prazdne";

 char * m_drevo[5];// = {"Smrk", "BorK","BorB","Buk ", "Dub "};
 char * m_katrem[2];//= {"1","2"};
 char * m_dalsi[3]= {""};// "ctyr", "paso"};
 char * m_manipulace[2]= {""}; //, "man"};
 unsigned char prohlizeny = 0;
 unsigned char menu_i = 0;


/** Uvádí, je-li, či nění-li aktivováno nastavování parametrů. */
static char dat_scr_status = 0;


static void vykresli_radek(char zaznam,char i);

/// Funkce zajišťuje vykreslení obrazovky s daty.
void data_scr_draw(void){
  //char s[15];
  //strcpy_P(s,pole[0]);
  //m_drevo[0] = (char *)mmalloc(5);
  //m_drevo[0]
  if (akt_scr != DAT_SCR){
    prohlizeny = (unsigned char) data_top - 1;
    akt_scr = DAT_SCR;
  }
  vypis_seznam();
}

/// Posune kurzor směrem vzhůru.
void data_up(void){
	//  assert(-1);
  if ((data_top > 0)&&(prohlizeny < ((unsigned char)(data_top - 0)))){ // -0 proto�e na vrchu je je�t� p�idat
    prohlizeny++;
  }
}


/// Posune kurzor dolu.
void data_down(void){
	//  assert(1==0);
  if ((data_top > 0)&&(prohlizeny > 0)){
    prohlizeny--;
  }
}


/**
 * Funkce zajišťuje funkci kláves(a nejen jich) v této obrazovce. Je
 * volána při každé otočce nekonečné smyčky v mainu
 */
void data_scr_control(void){
  switch (dat_scr_status){
  case 1:
    control_edit();
    break;
  case 2:
    control_ok();
    break;
  case 3:
    control_delka();
    break;
  case 4:
    control_prumer();
    break;
  default:
    control1();
    break;
  }
}

void control_delka(void){
  char *s;
  unsigned char radek = 0;

  if (prohlizeny < 4){
    radek = 3 - (unsigned char)prohlizeny;
  }

  if ((s = read_line()) != NULL){
    if (s[0] != '\0'){
      //int cislo;
      //int * p_cislo = (int *)mmalloc(sizeof(int));
      char s2[10]="     ";
      if (sscanf(s,"%i",&zadana_delka) == 0){
        zadana_delka = -1;
        new_error(100); 
        //printxyd(1,30,"blbe");
        //printxyd(2,30,s);
      }
      else{
        sprintf(s2,"%2i.%02i",zadana_delka/100,zadana_delka%100);
        printxyd(radek,30,s2);
        //*p_cislo = cislo;
        //menu[5] -> polozky = (char **) p_cislo;
      }
      mfree((void *) s);
    }
    else { // pokud Esc,  nebo je retezec nulove delky
      //printxyd(1,35,"esc_null");
      zadana_delka = -1;
      mfree((void *) s);
      new_error(100); // nespravne zadane cislo
      //menu[5] -> polozky = NULL; // ale asi to tu b�t nemus�.
      //assert(1);
    }
    dat_scr_status = 1; 
  }
}

void control_prumer(void){
  char *s;
  unsigned char radek = 0;

  if (prohlizeny < 4){
    radek = 3 - (unsigned char)prohlizeny;
  }

  if ((s = read_line()) != NULL){
    if (s[0] != '\0'){
      //int cislo;
      //int * p_cislo = (int *)mmalloc(sizeof(int));
      char s2[10]="      ";
      
      if (sscanf(s,"%i",&zadany_prumer) == 0){
        zadany_prumer=-1;
	new_error(100);
	//printxyd(1,30,"blbe");
	//printxyd(2,30,s);
      }
      else{
	sprintf(s2,"%2i.%01i",(int)(zadany_prumer/10),(int)(zadany_prumer%10));
	printxyd(radek,36,s2);
	//stp_nastav(menu_i,(float)cislo);
	//*p_cislo = cislo;
	//menu[6] -> polozky = (char **) p_cislo;
      }
      mfree((void *) s);
    }
    else { // pokud Esc,  nebo je retezec nulove delky
      zadany_prumer = -1;
      mfree((void *) s);
      new_error(100); // nespravne zadane cislo
              //[6] -> polozky = NULL; // ale asi to tu b�t nemus�
    }
    dat_scr_status = 1; 
  }
}

/// Funkce přebírá kontrolu nad klávesnicí v okamžiku, kdy je třeba potvrdit
/// smazání souboru.
void control_ok(void){
    // Potvrzen� p�ed smaz�n�m jm�na
		//  clear_dscreen();
		//  ok_scr_init((char *) s_smazat);
  
  switch (ok_scr()){
  case -1:
    dat_scr_status = 0;
    data_scr_draw();
    break;
  case 1:
    dat_scr_status = 0;
    dat_del_record(prohlizeny);
    data_down();
    data_scr_draw();
    break;
  default:
    break;
  }   
}


/// Zajišťuje pohyb kurzoru, přepínání do editačnéého režimu
void control1(void){
  char c;
  
  if ((c = get_char()) != 0){
    switch (c){
    case ' ':
      data_down();
      data_scr_draw();        
      break;
    default:
      break;
    }
  }else if ((c = get_spec()) != 0){
    switch(c){
    case 2: // Tab
    case 8:
    case 10:
      data_down();
      data_scr_draw();
      break;
    case 4://Enter
      // podmínka, aby nedošlo k otevření menu jestliže není zaveden žádný zákazník
      if (dat_prvni_zakaznik() != -1){
      //m_jmena
        menu_open();
      }
      else{
        new_error(ERRN_NO_CUSTOMER);
      }
      break;
    case 6: //Del
      if (prohlizeny != data_top){
        clear_dscreen();
        ok_scr_init((char *)s_smazat_mereni);
        dat_scr_status = 2;
      }
      break;
    case 7:
    case 9:
      data_up();
      data_scr_draw();        
      break;
    case 12://F2
    case 5: //Esc
      scr_ch(MSR_SCR);
      //measure_scr_draw();
      break;
    case 11: //F1
      scr_ch(HLP_SCR);
      //help_scr_draw();
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
 *  Přebírá kontrolu pokud editujeme data.
 *  @todo v případě, že není zadán žádný zákazník, po kliknutí na přidat přesun do menu se seznamem zákazníků, nebo error
 */
void control_edit(void){
  char c;
  if ((c = get_char()) != 0){
    switch (c){
    case ' ':
      break;
    default:
      break;
    }
  }else if ((c = get_spec()) != 0){
    switch(c){
    case 2: // Tab
      menu_close();
      break;
    case 8:
    case 10:
      menu_i = menu_next_item(menu, menu_i, VELIKOST_MENU);
      break;
    case 4://Enter
      if (menu_i == 5){
        menu_str_delka();
      }
      else if (menu_i == 6){
        menu_str_prumer();
      }
      else {
        menu_next_value(menu, menu_i);
      }
      break;
    case 7:
    case 9:
      menu_i = menu_prev_item(menu, menu_i, VELIKOST_MENU);
      break;
    case 5: //esc
      menu_close();
      data_scr_draw();
      break;
    case 11:
      scr_ch(HLP_SCR);
      //help_scr_draw();
      break;
    case 21: // F11 - Strorno m��en�
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
}

/// Inicializační funkce je prázdná. Žádné inicializace nejsou potřebné.
void data_scr_init(void){
}

/**
 * Funkce alokuje prostředky pro menu, dále pak načítá do paměti texty ("Smrk", "man" ...)
 * z programové paměti.
 */
void data_scr_open(void){
  int i = 0;

  m_drevo[0] = smrk;
  m_drevo[1] = (boro_k);
  m_drevo[2] = (boro_b);
  m_drevo[3] = (buk);
  m_drevo[4] = (dub);
  m_katrem[0] = (n1);
  m_katrem[1] = (n2);
  m_dalsi[1] = (ctyr);
  m_dalsi[2] = (paso);
  m_manipulace[1] = (man);

  for (i = 0; i < VELIKOST_MENU; i++){
    menu[i] = new_menu_item();
    menu[i] -> y = 0;
    menu[i] -> val = 0;
  }

	//  menu[0] -> x = 1;
	//  menu[0] -> polozky = NULL;
	//  menu[0] -> len = 0;

  menu[0] -> x = 5;
  menu[0] -> polozky = m_jmena;
  menu[0] -> len = MAX_ZAKAZNIK;

  menu[1] -> x = 14;
  menu[1] -> polozky = m_drevo;
  menu[1] -> len = 5;

  menu[2] -> x = 19;
  menu[2] -> polozky = m_katrem;
  menu[2] -> len = 2;

  menu[3] -> x = 21;
  menu[3] -> polozky = m_dalsi;
  menu[3] -> len = 3;

  menu[4] -> x = 26;
  menu[4] -> polozky = m_manipulace;
  menu[4] -> len = 2;

  menu[5] -> x = 30;
  menu[5] -> polozky = NULL;
  menu[5] -> len = 0;

  menu[6] -> x = 36;
  menu[6] -> polozky = NULL;
  menu[6] -> len = 0;

  if (data_top > 0){
    prohlizeny = data_top - 1;
  }
  else{
    prohlizeny = 0;
  }

}


/// Funkce provádí uvolnění naalokovaných prostředků.
void data_scr_close(void){
  int i = 0;
  /*
  mfree((void *)m_drevo[0]);
  mfree((void *)m_drevo[1]);
  mfree((void *)m_drevo[2]);
  mfree((void *)m_drevo[3]);
  mfree((void *)m_drevo[4]);
  mfree((void *)m_katrem[0]);
  mfree((void *)m_katrem[1]);
  mfree((void *)m_dalsi[1]);
  mfree((void *)m_dalsi[2]);
  mfree((void *)m_manipulace[1]);
*/
 

	// úklid v případě, že dojde k přerušení například vyvoláním erroru
  switch(dat_scr_status){
  case 1: //upravy
    //wtch_reset2();
    menu_close();
    dat_scr_status = 0;
    break;
  case 3:
  case 4:
    //wtch_reset2();
    read_line_kill();
    //menu_close();
    dat_scr_status = 0;
    break;
  default:
    break;
  }


   for (i = 0; i < VELIKOST_MENU; i++){
    mfree((void *)menu[i]);
    menu[i] = NULL;
  }
}


/// Vypisuje data. Funkce je volána funkcí data_scr_draw() . Zajišťuje výpis
/// dat a dokreslení kurzoru na patřičné místo.
void vypis_seznam(void){
  int i;
  int zaznam;
  unsigned char prvni_vykreslovany = 0;
  
  clear_dscreen();
  /*  if (data_top == 0){
  printxyd(0,0, strcpy_P(str_buf,s_prazdne));
  //    strcpy_P(str_buf, s_prazdne);
  //    printcxyd(0,1,str_buf);
  }
  else{*/
  if (prohlizeny < 4){
    prvni_vykreslovany = 3;
    printcxyd(3 - (int)prohlizeny,0,CURSOR);
  }
  else{
    prvni_vykreslovany = prohlizeny;
    printcxyd(0,0,CURSOR);
  }
  
  i = ((data_top + 1) < LCD_Y_MAX)? LCD_Y_MAX - (data_top + 1):0;
  for(; (i < LCD_Y_MAX) ; i++){
    zaznam = (int)(prvni_vykreslovany - i);
    if (zaznam != data_top){
      r_record(zaznam);
      
      //strcpy(str_buf, "   ");
      vykresli_radek(zaznam,i);
    }
    else{
      printxyd(i,5,menu_pridat);
    }
  }
  //}
}

/**
 * Funkce vykresli radek
 * @param zaznam číslo záznamu, který je vykreslován
 * @param i řádek, na který je záznam vykreslován
 */
static void vykresli_radek(char zaznam,char i){
  str_buf[0] = ' ';
  printxyd(i,1,int2strn(str_buf,3,(uint16_t)zaznam));
  //printcxyd(i,4,(data_zakaznik % 10)+ '0'); /// @todo odstranit
  printxyd(i,5,m_jmena[(int)data_zakaznik]);
  printxyd(i,14,m_drevo[(int)data_druh_dreva]);
  printxyd(i,19,m_katrem[(int)((data_operace >> 3)&1)]);
  printxyd(i,21,m_dalsi[(int)((data_operace >> 1)&3)]);
  printxyd(i,26,m_manipulace[(int)((data_operace)&1)]);

  sprintf(str_buf,"%2i.%02i",(int)(data_delka/100),(int)(data_delka%100));
  printxyd(i,30,str_buf);
  //printcxyd(i,32,'.');
  //str_buf[0] = '0';

  //printxyd(i,33,int2strn(str_buf,2,data_delka % 100));

  sprintf(str_buf,"%2i.%01i",(int)(data_prumer/10),(int)(data_prumer%10));
  printxyd(i,36,str_buf);
  //printxyd(i,36,int2strn(str_buf,2,data_prumer / 10));
  //printcxyd(i,38,'.');
  //printcxyd(i,39,(data_prumer % 10) + '0');
}


/**
 * Funkce zakládá menu pro editaci údajů u naměřených kmenů.
 */
void menu_open(void){
  unsigned char radek =0;
  
  menu_i = 0;
  dat_scr_status = 1;

	/* Nastavení řádku */
  if (prohlizeny < 4){
    radek = 3 - (unsigned char)prohlizeny;
  }

	// o�et�en� položky přidat
  if (prohlizeny == data_top){ //pridat
    //printxyd(1,5,"pridavame");
    
    
    if (prohlizeny == (unsigned char) 0){
      //printxyd(1,20,"prvni");
      //new_error(94);
      
      
      if (m_jmena[get_stored_menu_jmena_i()] != NULL){ // existuje n�jak� z�kazn�k
        data_zakaznik = get_stored_menu_jmena_i();
        data_druh_dreva = (unsigned char)0;
        data_operace = (unsigned char) 0;
        data_delka = (unsigned char) 0;
        data_prumer = (unsigned char) 0;
        
        new_record(data_zakaznik, data_druh_dreva, data_operace, data_delka, data_prumer);
        printxyd(radek, 4,mezery(str_buf,8));
        vykresli_radek(data_top - 1, radek);
      }
      else{
        new_error(94);
      } 
    }
    else{
      r_record((unsigned int)(data_top - 1));
      new_record(data_zakaznik, data_druh_dreva, data_operace, data_delka, data_prumer);
      printxyd(radek, 4,mezery(str_buf,8));
      vykresli_radek(data_top - 1, radek);
    }

  }
  else{ //jinak
    r_record((unsigned int)prohlizeny);
  }



  menu[0] -> y = radek;  
  menu[1] -> y = radek;  
  menu[2] -> y = radek;
  menu[3] -> y = radek;  
  menu[4] -> y = radek;
  menu[5] -> y = radek;
  menu[6] -> y = radek;

  menu[0] -> val = data_zakaznik;
  menu[1] -> val = data_druh_dreva;
  menu[2] -> val = ((data_operace >> 3) &1);  //1,2
  menu[3] -> val = ((data_operace >> 1) &3);  // dalsi_operace
  menu[4] -> val = ((data_operace) & 1);      // manipulace

  printcxyd(radek,4,255); // vykreslen� spr�vn�ho kurzoru
  printcxyd(radek,0,' ');
}

/** Funkce uzavírá menu */
void menu_close(void){
  char scr_zakaznik = menu[0] -> val;
  char scr_drevo = menu[1] -> val;
  char scr_operace = 0 | (((menu[2] -> val) & 1) << 3);
//  int * p_cislo = NULL;

  scr_operace |= ((menu[3] -> val) & 3) << 1;
  scr_operace |= (menu[4] -> val) & 1;

  
  r_record((unsigned int)prohlizeny);
  
	// ukl�d�n� �e�ezc�

  if (zadana_delka != -1){
    data_delka = (uint16_t) zadana_delka;
  }
  if (zadany_prumer != -1){
    data_prumer = (uint16_t) zadany_prumer;
  }
  /*
   * //číslo je uloženo do menu
  if ((menu[5] -> polozky) != NULL){
    p_cislo = (int *)menu[5] -> polozky;
    data_delka = (uint16_t) *p_cislo;
    mfree((void *) menu[5]);
    menu[5] -> polozky = NULL;
    // @todo free menu[5] -> polozky
  }

  if ((menu[6] -> polozky) != NULL){
    p_cislo = (int *)menu[6] -> polozky;
    data_prumer = (uint16_t) *p_cislo;
    mfree((void *) menu[6]);
    menu[6] -> polozky = NULL;
    // @todo free menu[6] -> polozky 
  }
*/
  w_record((unsigned int)prohlizeny ,scr_zakaznik, scr_drevo, scr_operace, data_delka, data_prumer);

  dat_scr_status = 0;

   // vykreslení správného kurzoru
  printcxyd(menu[0]->y,0,255);
  printcxyd(menu[0]->y,menu[menu_i]->x - 1,' ');
}

/**
 * Funkce zahájí zápis délky.
 */
void menu_str_delka(void){
  char * s = (char *)mmalloc(MAX_STR_DELKA + 1); // TODO �lo by to pak p�ealokovat na men��
  if (s != NULL){
    mezery(s,MAX_STR_DELKA);
    s[MAX_STR_DELKA] = '\0';
    //m_jmena[MAX_STR_DELKA] = s;
    read_line_init (menu[(int)menu_i] -> y, menu[(int)menu_i] -> x, s);
    dat_scr_status = 3;
    printcxyd(menu[(int)menu_i] -> y,34,' ');
  }
}

/**
 * Funkce zahajuje zadávání průměru.
 */
void menu_str_prumer(void){
  char * s = (char *)mmalloc(MAX_STR_PRUMER + 1); // TODO �lo by to pak p�ealokovat na men��
  if (s != NULL){
    mezery(s,MAX_STR_PRUMER);
    s[MAX_STR_PRUMER] = '\0';
    //m_jmena[MAX_STR_DELKA] = s;
    read_line_init (menu[(int)menu_i] -> y, menu[(int)menu_i] -> x, s);
    dat_scr_status = 4;
    printcxyd(menu[(int)menu_i] -> y,39,' ');
  }
}


/* end of dat_scr.c */
