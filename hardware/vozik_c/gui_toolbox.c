/*
 * $Id: gui_toolbox.c 117 2007-09-18 21:53:25Z Mira $
 */
/**
 * @file gui_toolbox.c
 * @brief
 * Modul zav�d� n�kter� jednoduch� elementy gui.
 *
 * Element 'menu' je seznam ve kter�m ka�d� polo�ka m��e nab�vat v�ce hodnot.
 * Zm�nu hodnoty zaji��uje funkce menu_next_value(). P�esun kursoru k dal��
 * polo�ce je realizov�n prost�ednictv�m funkce menu_next_item(), nebo funkce
 * menu_prev_item().
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "lcd.h"
#include "gui_toolbox.h"
#include "gui.h"
#include "convert.h"
#include "keyboard.h"
#include "error.h"
#include "main.h"
#include "msr_scr.h"
#include "measure.h"
#include "data_serial.h"
#include "keyfcn.h"

/// @todo Zde by bylo mo�n� �et�it pam�t�.
static int char_counter;
static char max_len;
static char sour_x;
static char sour_y;
//static char (*screen)[LCD_X_MAX];
static char *str;
static char *def_str = NULL; ///<- pou��v�no p�i defaultn�m �et�zci po enteru
static char s_prazdne[] PROGMEM = "prazdne";

/**
 * Funkce inicializuje na��t�n� �et�zce z ��dky.
 * @param y Sou�adnice po��tku.
 * @param x Sou�adnice po��tku.
 * @param s Ukazatel na string, kam bude �et�zec ukl�d�n.
 */
void read_line_init(int y, int x, char *s){ 
  int i;
  
  char_counter = 0;
  max_len = (char) strlen(s);
  sour_x = x;
  sour_y = y;
  str = s;
  for (i = 0; i < max_len ; i++){
    printcxyd(y,x + i,' ');
  }
  if (char_counter + sour_x < (LCD_X_MAX - 1)){
    printxyd(sour_y,char_counter + sour_x,"_");
  }

}

/**
 * Funkce zah�j� zad�v�n� �et�zce stejn� jako read_line_init.
 * Rozd�lem v�ak je, �e v p��pad�, �e je zad�v�n� ukon�eno enterem p�i nulov� d�lce,
 * je nastavena defaultn�� hodnota �e��zce, kter� je z�sk�na z po��te�n�ho stavu.
 */
void read_line_init_edefault(int y, int x, char *s,char len){ 
  int i;
  
  char_counter = 0;
  max_len = len; //(char) strlen(s);
  def_str = mmalloc(strlen(s) + 1);
  strcpy(def_str, s);
  sour_x = x;
  sour_y = y;
  str = s;
  for (i = 0; i < max_len ; i++){
    printcxyd(y,x + i,' ');
  }
  if (char_counter + sour_x < (LCD_X_MAX - 1)){
    printxyd(sour_y,char_counter + sour_x,"_");
  }

}


/**
 * Funkce na��t� znaky z ��dky, vykresluje p�itom kurzor. Umo��uje maz�n�
 * znak� Backspacem. Nen� implementov�n pohyb �ipkami.
 *
 * @returns
 * N�vratov� hodnota je NULL. Pokud je stisknut enter, m�n� se na ukazatel na �et�zec
 * kter� byl vytvo�en.
 * 
 * @retval NULL
 * Nebylo-li dokon�eno zad�v�n� �et�zce.
 *
 * @retval char*str
 * Pokud bylo ukon�eno zad�v�n� �et�zce, n�vratov� hodnota je ukazatel na dan�
 * �et�zec.
 */
 char *read_line (void){
   char xpchc = sour_x + char_counter;
   char c[3] = {' ','_','\0'};
   if ((c[0] = get_spec()) != 0){
     switch(c[0]){
     case 5: //esc
       return "";
       break;
     case 1:
       if (char_counter > 0){
         char_counter--;
         if (char_counter == (max_len)){
           printxyd(sour_y,sour_x + char_counter,"_");
           str[char_counter - 1] = ' ';
         }
         else{
           printxyd(sour_y,sour_x + char_counter,"_ ");
           str[char_counter] = ' ';
         }
       }
       break;
     case 4: //enter
       if ((char_counter == 0)&&(def_str != NULL)){
         strcpy(str,def_str);
         free((void *)def_str);
         def_str = NULL;
       }
       else {
         str[char_counter] = '\0';
         if (def_str == NULL){
           free((void *)def_str);
           def_str = NULL;
         }
       }
       return str;
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
   else if ((c[0] = get_char()) != 0){
     if (char_counter >= (max_len - 1)){
       c[1] = '\0';
       if (char_counter == max_len)
         char_counter--;
     }
     else {
       c[1] = '_';
     }
     printxyd(sour_y,sour_x + char_counter,c);
     str[char_counter] = c[0];
     if ((xpchc < (LCD_X_MAX - 1))&&(char_counter < max_len)){
       char_counter++;
     }
   }
   return NULL;
 }

/**
 * Funkce m� za �kol ukon�en� zad�v�n� �et�zce z venku. Zajist� uvoln�n� zdroj�.
 */
void read_line_kill(void){
  free((void *)str);
  max_len = 0;
  char_counter = 0;
  if (def_str != NULL){
    free((void *)def_str);
    def_str = NULL;
  }
}



// --------------menu------------
///
/// Menu se zakl�d� t�m, �e se nejprve v pam�ti vytvo�� jednotliv� polo�ky menu funkc� new_menu_item.
/// V t�ch je nadefinov�n ukazatel na pole popisk� hodnot.
/// x,y jsou sou�adnice po��tku n�pisu
/// val je aktu�ln� hodnota menu
/// len je po�et mo�n�ch hodnot menu. Je li v poli popisk� m�sto 
/// ukazatele na �et�zec ukazatel na NULL, doch�z� k p�eskakov�n� t�to hodnoty.
MENU_ITEM *new_menu_item(void){
  MENU_ITEM *p_pom;
  p_pom = (MENU_ITEM *) malloc(sizeof(MENU_ITEM));
  if(p_pom == NULL){
    new_error(88);
  }
  return p_pom;
}

/**
 * Funkce vykresl� menu.
 * Vykresleny jsou jeho polo�ky a kurzor. Je-li hodnota n�jak� polo�ky
 * NULL, sk��e se na dal��. Jedinou v�jimkou je hodnota 255. Pro ty je vyps�na
 * hl�ka "prazdne".
 */
void menu_draw(MENU_ITEM *menu[], uint8_t len, char menu_i){
  int i;

  for (i =0; i <len ; i++){
    if (menu[i] -> polozky == NULL){
      
    }
    else{
      // pokud je hodnota polo�ky NULL a nen� 255 pak nev�me co vypsat. Zkus�me tedy sko�it
      // na dal�� polo�ku.
      if ((menu[i] -> polozky[menu[i] -> val] == NULL)&&(menu[i] -> val != 255))
	menu_next_value(menu, menu_i);
      
      if ((menu[i] -> polozky[menu[i] -> val] == NULL)&&(menu[i] -> val != 255)){
      }
      //printxyd(menu[i] -> y, menu[i] -> x,"prazdne");
      else{
	// printxyd(menu[i] -> y, menu[i] -> x, menu[i] -> polozky[menu[i] -> val]);
	if (menu[i] -> val == 255)
	  printxyd(menu[i] ->y, (menu[i] -> x), strcpy_P(str_buf,s_prazdne));
	else
	  printxyd(menu[i] ->y, (menu[i] -> x), menu[i] -> polozky[menu[i] -> val] );
    }
    }
  }

  i = (int) menu_i;
  printcxyd(menu[i] -> y, ((menu[i] -> x) - 1), 255 );
}

/**
 * Funkce posune kurzor na p�edchoz� polo�ku menu.
 */
char menu_next_item(MENU_ITEM *menu[], char i, char max_i){
  int ii = (int) i;
  int max_ii = (int) max_i;

  printcxyd(menu[ii] -> y, ((menu[ii] -> x) - 1), ' ' );

  if (ii < (max_ii - 1)){
    ii++;
  }
  else{
    ii = 0;
  }
  printcxyd(menu[ii] ->y, ((menu[ii] -> x) - 1), 255 );

  return (char) ii;
}

/**
 * Funkce posune kurzor na dal�� polo�ku menu.
 */
char menu_prev_item(MENU_ITEM *menu[], char i, char max_i){
  int ii = (int) i;
  int max_ii = (int) max_i;

  printcxyd(menu[ii] -> y, ((menu[ii] -> x) - 1), ' ' );

  if (ii > 0){
    ii--;
  }
  else{
    ii = max_ii - 1;
  }
  printcxyd(menu[ii] ->y, ((menu[ii] -> x) - 1), 255 );

  return (char) ii;
}

/**
 * Funkce nastav� zvolenou hodnotu na dal��.
 */
void menu_next_value(MENU_ITEM *menu[], char ic){
  int i = (int) ic;
  char len_a;
  char len_b;
  char counter = 0;
  
  if (menu[i] -> polozky[menu[i] -> val] != NULL)
    len_a = strlen(menu[i] -> polozky[menu[i] -> val]);
  else
    len_a = 8;

  do{ // p�eskakov�n� NULL hodnot
    // posouv�n� hodnot vp�ed
    if (menu[i] -> val < (menu[i] -> len - 1)){
      menu[i] -> val = menu[i] -> val + 1;
    }
    else 
      menu[i] -> val = 0;

    counter++;

    if (counter > menu[i] -> len){
      menu[i] -> val = 255;
      break;
    }
  }while((menu[i] -> polozky[menu[i] -> val]) == NULL);
  
  if (menu[i] -> val != 255)
    len_b = strlen(menu[i] -> polozky[menu[i] -> val]);
  else
    len_b = 0;
  
  // domaz�n� konce minl�ho �et�zce
  if (len_a > len_b){
    int j;
    len_a = len_a - len_b;
    for (j=0; j < len_a; j++){
      printcxyd(menu[i] ->y, menu[i] ->x + len_b + j,' ');
    }
  }
  
  if (menu[i] -> val == 255)
    printxyd(menu[i] ->y, (menu[i] -> x), strcpy_P(str_buf,s_prazdne));
  else
    printxyd(menu[i] ->y, (menu[i] -> x), menu[i] -> polozky[menu[i] -> val] );
}


/* end of gui_toolbox.c */
