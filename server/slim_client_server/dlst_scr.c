/* $Id: dlst_scr.c 117 2007-09-18 21:53:25Z Mira $
 */
/**
 * @file dlst_scr.c
 * @brief
 * Modul Zajištuje vykreslování a obsluhování dospleje se jmény zákazníků.
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <avr/io.h>
//#include <avr/pgmspace.h>
#include "dlst_scr.h"
//#include "lst_scr.h"
#include "msr_scr.h"
#include "gui.h"
#include "gui_toolbox.h"
#include "keyboard.h"
#include "error.h"
#include "convert.h"
#include "main.h"
#include "data.h"
#include "measure.h"
#include "data_serial.h"
#include "keyfcn.h"
#include "memory.h"

//global
void dlst_scr_init(void);
void dlst_scr_draw(void);
void dlst_scr_open(void);
void dlst_scr_close(void);

void dlst_scr_control(void);

void set_dlst_menu_i(char menu_jmena_i);
//global


static char menu_len;
static char menu_i;
static MENU_ITEM *menu[MAX_ZAKAZNIK];
static char s_smazat[]  = " \n  Opravdu odstranit jmeno ze seznamu?";
static char run = 0;


void dlst_scr_init(void){

}

void dlst_scr_draw(void){
  akt_scr = DLST_SCR;
  clear_dscreen();
  menu_draw(menu,menu_len,menu_i);
}

/**
 * Funkce zajišťuje vytvoření datových struktur pro vykreslení menu.
 */
void dlst_scr_open(void){
  int i;
  int j = 0;
  unsigned char menu_jmena_i = 0;

  
  if (menu_len < MAX_ZAKAZNIK) // pokud už je seznam plný, tak se nevypisuje "Pridat"
    m_jmena[MAX_ZAKAZNIK] = menu_pridat; //tenhle řádek je zde asi zbytečně

  for (i = 0; i < (MAX_ZAKAZNIK ); i++) {
    menu[i] = new_menu_item();
    menu[i] -> x =(char) 1 + ((i / 4) * 9);
    menu[i] -> y =(char) (i % 4);
    menu[i] -> polozky = m_jmena;
    menu[i] -> len = (char) MAX_ZAKAZNIK + 1;
  }

  i = 0;

//menu_jmena_i = 0
  menu_jmena_i = get_stored_menu_jmena_i();  




    // Tohle přiřazuje každé položce menu hodnotu o jedna vyžší, ale přeskakuje to NULL
  while (j < (MAX_ZAKAZNIK + 1)){ 
    //< ta plus jednička je tam protože pole je o jedna větší. Poslední položka pole je "přidat".

    if ((m_jmena[j] != NULL) && (i < MAX_ZAKAZNIK)){
      //< ta druhá část se uplatňuje pouze v případě, že je seznam zákazníků plný
      // tak aby se do velikosti menu nepočítalo i "přidat", které je jinak
      // na pozici MAX_ZAKAZNIK+1
      menu[i] -> val = j;
       if (menu_jmena_i == j) 
         menu_i = i; 
      i++;
    }
    j++;
  }
  menu_len =(char) (i < MAX_ZAKAZNIK + 1)? i :(MAX_ZAKAZNIK);
  if (menu_len <= menu_jmena_i){
    menu_jmena_i = 0;
  }
}

/**
 * Funkce nastaví pozici kurzoru tak, aby jméno které je v msr_scr bylo vybráno
 * i zde.
 */
void set_dlst_menu_i(char menu_jmena_i){
  int i =0;

  for (i = 0; i < (MAX_ZAKAZNIK + 1); i++){
    if (menu[i] -> val == menu_jmena_i ){
      menu_i = i;
      break;
    }
  }
  menu_i = 0;
}

void dlst_scr_close(void){
  int i;

  for ( i = 0; i < MAX_ZAKAZNIK; i++){
    mfree((void *)menu[i]);
  }


  switch (run){
  case 1:
    read_line_kill();
    run = 0;
    break;
  default:
    break;
  }

}

/**
 * Funkce je cyklicky volána pokud je obrazovka aktivní. Stará se především o čtení
 * z klávesnice a o provádění příkazů zadaných jejím prostřednictvím.
 * proměnná 'run' sefinuje stav, v jakém se systém obrazovky nalézá.
 * ve stavu 0 je obrazovka v normálním režimu
 * ve stavu jedna uživatel zdává jméno a všechny znaky z klávesnice jsou proto
 * posílány do jména.
 * stav 2 je stav, v němž je obrazovka při potvrzování vymazání jména
 */
void dlst_scr_control(void){

  char c;
  if (run == 1){
    // probíhá načítání řetězce
    char *s;
    if ((s = read_line()) != NULL){
      if (s[0] != '\0'){
        // délka zadaného řetězce není nulová
	char m;
        //tady by to šlo přealokovat
        /// @TODO Zde je možné ušetřit paměť.
        m_jmena[MAX_ZAKAZNIK] = menu_pridat;
        m = dat_add_zakaznik(s);

	run = 0; // je to zde proto, aby nedocházelo k násilnému
		 // ukončení funkcí read_line_kill během
		 // následujícího uzavření a otevření dlst_scr
        
	// tyto řádky jsou zde (asi :-) ) proto, aby při zadání nového jména došlo k posunu k
	// překrreslen displeje a posusu tlačítka "pridat"
	dlst_scr_close();
        dlst_scr_open();
        menu_i = m;
        
      }
      else { // pokud Esc,  nebo je retezec nulove delky
        mfree((void *) m_jmena[MAX_ZAKAZNIK]);
        m_jmena[MAX_ZAKAZNIK] = menu_pridat;
      }
      dlst_scr_draw(); 
      run = 0;
    }
  }else if (run == 2){ //potvrzení před smazáním jména
    switch (ok_scr()){
    case -1:
      dlst_scr_draw();
//        clear_dscreen();
      //menu_draw(menu,menu_len, menu_i);
      run = 0;
      break;
    case 1:
      run = 0;
      //mfree((void *)jmena[(int)menu_i]);
      //odstran_jmeno(menu_i);
      dat_odstran_zakaznika(menu[(int)menu_i] -> val);
      dlst_scr_close();
      dlst_scr_open();
      dlst_scr_draw();
      //clear_dscreen();
      //menu_draw(menu,menu_len, menu_i);
      break;
    default:
      break;
    }    
  }else if ((c = get_spec()) != 0){
    // normálka
    switch(c){
    case 2: //Tab
    case 8:
    case 10:
      menu_i = menu_next_item(menu, menu_i, menu_len);
      break;
    case 7:
    case 9:
      menu_i = menu_prev_item(menu, menu_i, menu_len);
      break;
    case 4: //Enter
      // přepočet z indexu menu v dlst_scr na index seznamu jmen na msr_scr
      if (m_jmena[menu[(int)menu_i] ->val] == menu_pridat){ //nove jmeno
	char * s = (char *)mmalloc(9); // TODO �šlo by to pak přealokovat na menší
	if (s == NULL)
	  new_error(88);
	else{
	  mezery(s,8);
	  s[8] = '\0';
	  s[0] = 'z';
	  s[1] = 'a';
	  s[2] = 'k';
	  s[3] = ((menu_i + 1) / 10) + '0';
	  s[4] = ((menu_i + 1) % 10) + '0';
	  s[5] = '\0';
	  m_jmena[MAX_ZAKAZNIK] = s;
	  read_line_init_edefault (menu[(int)menu_i] -> y, menu[(int)menu_i] -> x, s,8);
	  run = 1;
	}
        
      }
      else{
	set_menu_jmena_i((unsigned char)menu[(int)menu_i] -> val);
	scr_ch(MSR_SCR);
      }
      break;
    case 5: //Esc  
      scr_ch(MSR_SCR);
      break;
    case 6: //Del
      if (menu[(int)menu_i] -> val != MAX_ZAKAZNIK){
	if (find_last_record_of_zakaznik(menu[(int)menu_i] -> val) == -1){
	  ok_scr_init((char *)s_smazat);
	  run = 2;
	}
	else{
	  new_error(95);
	}
      }
      break;
      
      
    case 11: //F11
      scr_ch(HLP_SCR);
      //help_scr_draw();
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
}

/* end of dlst_scr.c */
