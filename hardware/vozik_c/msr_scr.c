/* $Id: msr_scr.c 117 2007-09-18 21:53:25Z Mira $
 */
/**
 * @file msr_scr.c
 * @brief
 * Hlavn� obrazovka m��en�. Menu je alokov�no a� p�ed zobrazen�m. Hodnoty jsou
 * mezit�m ulo�eny v poli values.
 *
 * Posledn� polo�kou menu je klik�tko pro odchod na datovou obrazovku
 * 
 */


#include <stdio.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "msr_scr.h"
#include "hlp_scr.h"
#include "dlst_scr.h"
#include "lcd.h"
#include "gui.h"
#include "gui_toolbox.h"
#include "convert.h"
#include "measure.h"
#include "irc.h"
#include "keyboard.h"
#include "data.h"
//#include "lst_scr.h"
#include "dat_scr.h"
#include "main.h"
#include "error.h"
#include "eeprom.h"
#include "usart.h"
#include "data_serial.h"
#include "keyfcn.h"

static void keyb(void);
void save_menu_values(void);
void load_menu_values(void);


//static MENU_ITEM *p_a= NULL;
//static MENU_ITEM *p_b =NULL;
//

/// Definice po�tu polo�ek menu
#define VELIKOST_MENU 6

/// Definice po�tu polo�ek menu, kter� mohou nab�vat n�jak�ch hodnot.
/// Ostatn� polo�ky jsou "nehodnotov�". Jsou to nap��klad klik�tka pro odchod
/// na jinou obrazovku.
#define VELIKOST_MENU_HODNOT 5


static MENU_ITEM *menu[VELIKOST_MENU];

/// @name PROGMEM
/// Texty jsou ulo�eny v programov� pam�ti. Dojde tak k �spo�e m�sta
// @{
static char s_zakz[] PROGMEM = "Zakz";
static char s_drev[] PROGMEM = "Drev";
static char s_oper[] PROGMEM = "Oper";
static char s_delka[] PROGMEM = "Delka";
static char s_prumer_v[]PROGMEM = "prum_v";
static char s_prumer_h[]PROGMEM = "prum_h";
static char s_dat_scr[]PROGMEM = "Data";
static char s_smazat[] PROGMEM = " \n  Opravdu odstranit jmeno ze seznamu?";
static char s_msr_zahajeno[] PROGMEM = "Mereni zahajeno   ";
static char s_msr_delka[] PROGMEM =    "Zmerena delka     ";
static char s_msr_domereno[] PROGMEM = "Mereni dokonceno  ";
static char s_msr_pause[] PROGMEM =    "Mereni pozastaveno";
// @}

//static char s_nelze_smazat[] PROGMEM = "Jmeno nelze odstranit pokud je\n na nej odkaz mezi namerenymi kmeny!";


//TODO p�ed spu�t�n�m je t�eba nastavit v�echno na null,
//aby se to p�i ka�d�m vypnut� a zapnut� nezapl�ovalo z EEpromky
//char * m_jmena[MAX_ZAKAZNIK];
//static char jmena_len = 3;



/// @name Hodnoty polo�ek menu.
/// Pole �e��zc�, ketr� jsou vypisov�ny pro p��slu�n� volby.
/// @todo P�esunout tyto texty do PROGMEM
//@{
static char * m_drevo[] = {"Smrk", "Bor-kura","Bor-borka","Buk","Dub"}; // menit i v dat_scr.c
static char * m_katrem[] = {"1", "2"};
static char * m_dalsi[] = {"", "samo", "paso"};
static char * m_manipulace[] = {"", "man"};
//@}
static char menu_i = 0;

static uint8_t menu_values[VELIKOST_MENU_HODNOT]={0,0,0,0,0};



void save_menu_values(void){
  int i = 0;
  
  for (i=0; i < VELIKOST_MENU_HODNOT; i++){
    menu_values[i] = menu[i] -> val;
  }
}

void load_menu_values(void){
  int i = 0;
  
  for (i=0; i < VELIKOST_MENU_HODNOT; i++){
    menu[i] -> val = menu_values[i];
  }
}

void measure_scr_init(void){
//  int i;

  //free_m_jmena();

//for (i = 0; i < MAX_ZAKAZNIK; i++){
//  m_jmena[i] = NULL;
//}

  //m_jmena[0] = "Fanda";
  //m_jmena[2] = "Jarda";
  //m_jmena[3] = "Tonda";

  // polo�ka p�idat je na posledn�m m�st� v seznamu polo�ky p�ed n� jsou
  // NULL a jsou tud� p�eskakov�ny. V p��pad� napln�n� je na posledn�
  // m�sto m�sto n�pisu "pridat" uveden NULL. T�m dojde k jeho nevyps�n�
//  m_jmena[MAX_ZAKAZNIK - 1] = menu_pridat;
//  m_jmena[MAX_ZAKAZNIK] = menu_pridat;


  //menu_jmena_i = &(menu[0] -> val);

}

void measure_scr_open(void){
  int i = 0;

  for (i = 0; i < VELIKOST_MENU; i++){
    menu[i] = new_menu_item();
  
    menu[i] -> val = 0;
  }

  menu[0] -> x = 7;
  menu[0] -> y = 0;
  menu[0] -> polozky = m_jmena;
  menu[0] -> len = MAX_ZAKAZNIK;

  menu[1] -> x = 7;
  menu[1] -> y = 1;
  menu[1] -> polozky = m_drevo;
  menu[1] -> len = 5;

  menu[2] -> x = 7;
  menu[2] -> y = 2; 
  menu[2] -> polozky = m_katrem;
  menu[2] -> len = 2;

  menu[3] -> x = 9;
  menu[3] -> y = 2; 
  menu[3] -> polozky = m_dalsi;
  menu[3] -> len = 3;

  menu[4] -> x = 14;
  menu[4] -> y = 2; 
  menu[4] -> polozky = m_manipulace;
  menu[4] -> len = 2;

  menu[5] -> x = 7;
  menu[5] -> y = 3;
  menu[5] -> polozky = NULL;
  menu[5] -> len = 0;
  menu[5] -> val = 255;

  load_menu_values();
}

void measure_scr_close(void){
  int i = 0;

  save_menu_values();

  for (i = 0; i < VELIKOST_MENU; i++){
    free((void *)menu[i]);
    menu[i] = NULL;
  }
  
}



void measure_scr_draw(void){
  clear_dscreen();
  akt_scr = MSR_SCR;
  printxyd(0,0, strcpy_P(str_buf,s_zakz));
  printxyd(1,0, strcpy_P(str_buf,s_drev));
  printxyd(2,0, strcpy_P(str_buf,s_oper));
  printxyd_P(3,0, (void*) s_dat_scr);

  printxyd(1, 25, strcpy_P(str_buf,s_delka));
  printxyd(2, 25, strcpy_P(str_buf,s_prumer_v));
  if (width_src > 0){
    printxyd(3, 25, strcpy_P(str_buf,s_prumer_h));
    printcxyd(3,37, '.');
  }

  printcxyd(1,37, '.');
  printcxyd(2,37, '.');

  menu_draw(menu,VELIKOST_MENU,menu_i);

}



/// Funkce udr�uje LCD aktu�ln�. Vykresluje nep�etr�it� spr�vn� �daje.
/// Na konci je vol�na funkce pro kontrolu kl�vesnice.
/// @todo Bylo by mo�n� zde z�skat rychlost. �ada �daj� se vykresluje
/// nep�etr�it�, p�itom by sta�ilo je aktualizovat poze p�i zm�n�
/// prom�nn� msr_status.
void measure_scr_control(void){
  //uint16_t l,d_v,d_h;

// vykreslov�n� stavu
  switch (msr_status){
  case L_NEZMERENO:
    printxyd_P(0,20,(void *)s_msr_zahajeno);
    break;
  case L_ZMERENO:
    printxyd_P(0,20,(void *)s_msr_delka);
    break;
  case ZMERENY_PRUMERY:
    printxyd_P(0,20,(void *)s_msr_domereno);
    break;
  case PAUSE_L_NEZMERENO:
  case PAUSE_L_ZMERENO:
    printxyd_P(0,20,(void *)s_msr_pause);
    break;
  default:
    printxyd(0,20,mezery(str_buf,20));
    break;
  }

  if (msr_status >= L_ZMERENO){
    //l = msr_delka;
    printxyd(1,19, int2strn(str_buf,2,(msr_delka / 100)));
    str_buf[0] = '0';
    printxyd(1,22, int2strn(str_buf,2,(msr_delka % 100)));
    printcxyd(1,21,'.');
    str_buf[0] = ' ';

  }
  else{
    printxyd(1,18,mezery(str_buf,7));
//    l = irc_get_l();
  }
  
  if (msr_status == ZMERENY_PRUMERY){

    printxyd( 2,18, int2strn(str_buf, 3,(uint16_t)(msr_prumer_v / 10)));
    
    printcxyd( 2,22,(msr_prumer_v % 10) + '0');
    printcxyd( 2,21,'.');

    if (width_src > 0){
      printxyd( 3,18, int2strn(str_buf,3,(msr_prumer_h / 10)));
      printcxyd(3,22, (msr_prumer_h % 10) + 48);
      printcxyd(3,21,'.');
    }


//    printxyd( 0,35, int2strn(str_buf,1,(msr_objem / 1000)));
//    printxyd( 0,37, int2strn(str_buf,3,(msr_objem % 1000)));
//    printcxyd(0,36,'.');
  }
  else{
    printxyd( 2,18, mezery(str_buf,7));
    printxyd( 3,18, mezery(str_buf,7));
    //printxyd( 0,35, mezery(str_buf,7));
  }
  
//  printcxyd( 3,10, msr_status + '0');
//  printxyd( 3,13, int2strn(str_buf,2,msr_pocet_mereni));    

  printxyd( 1,34, int2strn(str_buf,3,(irc_get_l() / 100)));
  str_buf[0] = '0';
  printxyd( 1,38, int2strn(str_buf,2,(irc_get_l() % 100)));
  str_buf[0] = ' ';

  printxyd( 2,34, int2strn(str_buf,3,(prumer_v / 10)));


  printcxyd( 2,38,(prumer_v % 10) + 48);
  
//  printxyd(,34,"00");
  if (width_src >  0){
    printxyd( 3,34, int2strn(str_buf,3,(prumer_h / 10)));
    printcxyd( 3,38, (prumer_h % 10) + 48);
  }
  
  
//  printxyd( 3,35, int2strn(str_buf,4,msr_prumer) );
 
  keyb();
}

static void keyb(void){
  char c;
  static char run = 2;
  if (run == 1){
    char * s;
    if ((s = read_line()) != NULL){
      if (s[0] != '\0'){
        //tady by to �lo p�ealokovat
        // TODO pam�
        m_jmena[MAX_ZAKAZNIK - 1] = menu_pridat;
        dat_add_zakaznik(s);
      }
      else { // pokud Esc,  nebo je retezec nulove delky
        free((void *) m_jmena[menu[0] -> len - 1]);
        m_jmena[menu[0] -> len - 1] = menu_pridat;
      }
      run = 2;
    }
  }
  else if (run == 3){  // Potvrzen� p�ed smaz�n�m jm�na
    clear_dscreen();
    ok_scr_init((char *) s_smazat);
    
    switch (ok_scr()){
    case -1:
      measure_scr_draw();
      run = 2;
      break;
    case 1:
      run = 2;
      dat_odstran_zakaznika(menu[0] -> val);
      menu_next_value(menu, menu_i);
      scr_draw(MSR_SCR);
      //measure_scr_draw();
      break;
    default:
      break;
    }
    //  }else if(run == 4){
    //    clear_dscreen();
    //    ok_scr_init((char *) s_nelze_smazat);
    //    if (ok_scr() != 0){
    //      scr_draw(MSR_SCR);
    //      //measure_scr_draw();
    //      run = 2;
    //    }
    
  }else if ((c = get_char()) != 0){
    switch(c){
    case ' ':
      menu_next_value(menu, menu_i);
      break;
    case '1':
      menu_next_value(menu, 0);
      break;
    case '2':
      menu_next_value(menu, 1);
      break;
    case '3':
      menu_next_value(menu, 2);
      break;
    case '4':
      menu_next_value(menu, 3);
      break;
    case '5':
      menu_next_value(menu, 4);
      break;
      
    default:
      break;
    }
  }
  else if ((c = get_spec()) != 0){
    switch (c){
    case 1:
      //menu_next_value(menu, menu_i);
      break;
    case 2:  //Tab
    case 8:  //DownArrow
    case 10: //RightArrow
      menu_i = menu_next_item(menu, menu_i, VELIKOST_MENU);
      break;
    case 7:
    case 9:
      menu_i = menu_prev_item(menu, menu_i, VELIKOST_MENU);
      break;
    case 4: //Enter
      if (menu_i == 0){
        
        if ((menu[0] -> val == (MAX_ZAKAZNIK - 1))&&
          (m_jmena[menu[0] -> val] == menu_pridat)){ // spou�t�n� za�v�n� nov�ho �lov�ka
        
        char * s = (char *)mmalloc(9); // �lo by to pak p�ealokovat na men��
        if (s == NULL)
          new_error(88);
        else{
          mezery(s,8);
          m_jmena[MAX_ZAKAZNIK - 1] = s;
          read_line_init (menu[0] -> y, menu[0] -> x, s);
          run = 1;
        }
          }
          else{
            // zde je �e�eno p�ed�v�n� parametr� jm�na z�kazn�ka do
            // obrazovky z�kazn�k�
            scr_ch(DLST_SCR);
          }
      }
      else if(menu_i == 5){
        scr_ch(DAT_SCR);
      }
      else{
        menu_next_value(menu, menu_i);
        //	scr_ch(LST_SCR);
        //list_scr_draw();
      }
      break;
    case 6: //Del
      if (menu[0] -> val != 255){ // nelze mazat "pr�zdn�"
        // nutno testovat, zda z�znam nen� p�i�azen k n�kter�mu stromu
        if (find_last_record_of_zakaznik(menu[0] -> val) != -1){
          new_error(95);
          //	  ok_scr_init((char *) s_nelze_smazat);
          //	  run = 4;
        }
        else{
          
          ok_scr_init((char *)s_smazat);
          run = 3;
        }
      }
      break;
    case 11:  /// - F1 : Help screen
      //lcd_init(); /// @todo odstanil jsem to, nevim co se stande
      scr_ch(HLP_SCR);
      //help_scr_draw();
      break;
    case 12:  /// - F2 : Data screen
      scr_ch(DAT_SCR);
      //data_scr_draw();
      break;
    case 13:  /// - F3 : St�l� z�kazn�ci
      //assert(1==5);
      //scr_ch(LST_SCR);
      //data_scr_draw();
      break;
    case 14:  /// - F4 : Error screen
      scr_ch(ERR_SCR);
      //error_scr_draw();
      //p_akt_screen = error_screen;
      break;
    case 15:  /// - F5 : Transmitt buffer screen
      /// @warning zobrazen� odes�lac�ho bufferu bude pro provoz
      /// odstaveno. Pro odlazov�n� by v�ak tento prost�edek mohl b�t
      /// u�ite��n�.
      //      scr_ch(SER_SCR);
      break;
    case 16: 
      scr_ch(STP_SCR);
      break;
      /*{ 
      int i=0;
      char * s;
      s = (char *)mmalloc (10);
      strcpy(s,"Nazdar !");
      new_msg(s);
      //new_error(11);
      for (i=0; i < 2000; i++){
      _delay_ms(1);
      }
      break;
      }*/
    case 19:  //F9
      kf_start();
      break;
    case 20:  //F10
      if (msr_status == ZMERENY_PRUMERY){
        if (menu[0] -> val == 255){
          new_error(94);
        }
        else{
          uloz_posledni_data();
          msr_status = VOLNO;
        }
      }
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
 * Funkce m� za �kol p�ipravit z prom�nn�ch, kter� jsou k dispozici data pro
 * ulo�en� a pak je i ulo�it.
 * Je vol�na po stisknut� tla��tek pro zah�jen� m��en�, nebo pro konec m��en�,
 * nebo pak z vn�j�ku v p��pad� v�padku nap�jen�, aby nez�stal posledn�
 * z�znam neulo�en�,
 * */
  void uloz_posledni_data(void){
    if ((msr_status == ZMERENY_PRUMERY)&&((menu[0] -> val != 255))){ ///@todo zauva�ovat jestli by se nem�l
								     ///ukl�dat i bezejmen�
    char scr_zakaznik = menu[0] -> val;
    char scr_drevo = menu[1] -> val;
    char scr_operace = 0 | (((menu[2] -> val) & 1) << 3);
    scr_operace |= ((menu[3] -> val) & 3) << 1;
    scr_operace |= (menu[4] -> val) & 1;
    
    new_record(scr_zakaznik, scr_drevo, scr_operace, msr_delka, msr_prumer);
  }
}


/**
 * Funkce slou�� pro z�sk�n� aktu�ln�ho z�kaz z vn�j�ku
 * Nap��klad ji vyu��v� modul pro vykreslov�n� seznamu
 * dne�n�ch z�kazn�k�. Implementace s testov�n�m prom�nn�
 * akt_scr se uk�zala jako nevhodn�, nebo� modul dlst_scr
 * vol� funkci pot� co je ji� msr_scr ve stavu closed, ale
 * akt scr na n�j st�le ukazuje, nebo� je�t� st�le nen�
 * otev�en dlst_scr. Proto byla implementace zm�n�na a funkce
 * v�dy vrac� hodnotu ze z�lo�n�ho pole menu_values,
 * kter� v p��pad�, �e je pr�v� vykreslov�n msr_scr nen�
 * aktu�ln�.
 * @return Hodnotu val z polo�ky menu, kter� zobrazuje jm�na
 * z�kazn�k�. Jednodu�eji - Kolik�t� z�kazn�k byl naposledy
 * vybr�n v msr_scr.
 */
unsigned char get_stored_menu_jmena_i(void){
/*  if (akt_scr == MSR_SCR){
    return menu[0] -> val;
  }
  else {
    return menu_values[0];
  }
*/
  return menu_values[0];
}


/** 
 * Funkce vrac� hodnotu menu_i.
 * @warning 
 * V n�kter�ch situac�ch je vhodn�j�� volat funkce get_stored_menu_jmena_i().
 * Jedn� se zejm�na o situace b�hem p�ep�n�n� obrazovek.
 */ 
unsigned char get_menu_jmena_i(void){
  if (akt_scr == MSR_SCR){
    return menu[0] -> val;
  }
  else {
    return menu_values[0];
  }
}

/**
 * Funkce slou�� k nastaven� aktu�ln�ho z�kazn�ka z vn�j�ku.
 * Nap��klad ji vyu��v� modul pro vykreslov�n� seznamu
 * dne�n�ch z�kazn�k�.
 */
void set_menu_jmena_i(unsigned char i){
  if (akt_scr == MSR_SCR){
    menu[0] -> val = i;
  }
  else{
    menu_values[0] = i;
  }
}

/* end of msr_scr.c */
