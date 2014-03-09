/* $Id: dlst_scr.c 117 2007-09-18 21:53:25Z Mira $
 */
/**
 * @file dlst_scr.c
 * @brief
 * Modul Zaji�tuje vykreslov�n� a obsluhov�n� dospleje se jm�ny z�kazn�k�.
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
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

static char menu_len;
static char menu_i;
static MENU_ITEM *menu[MAX_ZAKAZNIK];
static char s_smazat[] PROGMEM = " \n  Opravdu odstranit jmeno ze seznamu?";
static char run = 0;


void dlst_scr_init(void){

}

void dlst_scr_draw(void){
  akt_scr = DLST_SCR;
  clear_dscreen();
  menu_draw(menu,menu_len,menu_i);
}

void dlst_scr_open(void){
  int i;
  int j = 0;
  unsigned char menu_jmena_i =0;

  
  //if (menu_len < MAX_ZAKAZNIK) // pokud u� je seznam pln�, tak se nevypisuje "Pridat"
    //m_jmena[MAX_ZAKAZNIK - 1] = menu_pridat;

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

  // Tohle p�i�azuje ka�d� polo�ce menu hodnotu o jedna vy���, ale p�eskakuje to NULL
  while (j < (MAX_ZAKAZNIK + 1)){ // ta plus jedni�ka je tam proto�e pole je o jedna v�t��. Posledn� polo�ka pole je "p�idat".
    if (m_jmena[j] != NULL){
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
 * Funkce nastav� pozici kurzoru tak, aby jm�no kter� je v msr_scr bylo vybr�no
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
    free((void *)menu[i]);
  }


  switch (run){
  case 1:
    read_line_kill();
    break;
  default:
    break;
  }

}

/**
 * Funkce je cyklicky vol�na pokud je obrazovka aktivn�. Star� se p�edev��m o �ten�
 * z kl�vesnice a o prov�d�n� p��kaz� zadan�ch jej�m prost�ednictv�m.
 * prom�nn� 'run' sefinuje stav, v jak�m se syst�m obrazovky nal�z�.
 * ve stavu 0 je obrazovka v norm�ln�m re�imu
 * ve stavu jedna u�ivatel zd�v� jm�no a v�echny znaky z kl�vesnice jsou proto
 * pos�l�ny do jm�na.
 * stav 2 je stav, v n�m� je obrazovka p�i potvrzov�n� vymaz�n� jm�na
 */
void dlst_scr_control(void){

  char c;
  if (run == 1){
    char *s;
    if ((s = read_line()) != NULL){
      if (s[0] != '\0'){
	char m;
        //tady by to �lo p�ealokovat
        /// @TODO Zde je mo�n� u�et�it pam�.
        m_jmena[MAX_ZAKAZNIK] = menu_pridat;
        m = dat_add_zakaznik(s);

	run = 0; // je to zde proto, aby nedoch�zelo k n�siln�mu
		 // ukon�en� funkc� read_line_kill b�hem
		 // n�sleduj�c�ho uzav�en� a otev�en� dlst_scr
        
	// tyto ��dky jsou zde (asi :-) ) proto, aby p�i zad�n� nov�ho jm�na do�lo k posunu k
	// p�ekrreslen displeje a posusu tla��tka "pridat"
	dlst_scr_close();
        dlst_scr_open();
        menu_i = m;
        
      }
      else { // pokud Esc,  nebo je retezec nulove delky
        free((void *) m_jmena[MAX_ZAKAZNIK]);
        m_jmena[MAX_ZAKAZNIK] = menu_pridat;
      }
      dlst_scr_draw(); 
      run = 0;
    }
  }else if (run == 2){ //potvrzen� p�ed smaz�n�m jm�na
    switch (ok_scr()){
    case -1:
      dlst_scr_draw();
//        clear_dscreen();
      //menu_draw(menu,menu_len, menu_i);
      run = 0;
      break;
    case 1:
      run = 0;
      //free((void *)jmena[(int)menu_i]);
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
      // p�epo�et z indexu menu v dlst_scr na index seznamu jmen na msr_scr
      if (m_jmena[menu[(int)menu_i] ->val] == menu_pridat){ //nove jmeno
	char * s = (char *)malloc(9); // TODO �lo by to pak p�ealokovat na men��
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

/* end of dlst_scr.c */
