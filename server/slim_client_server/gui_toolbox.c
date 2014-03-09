/*
 * $Id: gui_toolbox.c 117 2007-09-18 21:53:25Z Mira $
 */
/**
 * @file gui_toolbox.c
 * @brief
 * Modul zavádí některé jednoduché elementy gui.
 *
 * Element 'menu' je seznam ve kterém každá položka může nabývat více hodnot.
 * Změnu hodnoty zajišťuje funkce menu_next_value(). Přesun kursoru k další
 * položce je realizován prostřednictvím funkce menu_next_item(), nebo funkce
 * menu_prev_item().
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <avr/pgmspace.h>
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
#include "memory.h"

// global
void read_line_init(int y, int x, char *s);
void read_line_init_edefault(int y, int x, char *s,char len);

char *read_line (void);
void read_line_kill(void);


MENU_ITEM *new_menu_item(void);
void menu_draw(MENU_ITEM *menu[], unsigned int len, char i);
//void menu_draw(MENU_ITEM menu, unsigned int len);

char menu_next_item(MENU_ITEM *menu[], char i, char max_i);
char menu_prev_item(MENU_ITEM *menu[], char i, char max_i);
void menu_next_value(MENU_ITEM *[], char i);
// global


/// @todo Zde by bylo možné šetřit pamětí.
static int char_counter;
static char max_len;
static char sour_x;
static char sour_y;
//static char (*screen)[LCD_X_MAX];
static char *str;
static char *def_str = NULL; ///<- pou��v�no p�i defaultn�m �et�zci po enteru
static char s_prazdne[]  = "prazdne";

/**
 * Funkce inicializuje načítání řetězce z řádky.
 * @param y Soužadnice počátku.
 * @param x Souřadnice počátku.
 * @param s Ukazatel na string, kam bude řetězec ukládán.
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
 * Funkce zahájí zadávání řetězce stejně jako read_line_init.
 * Rozdílem však je, že v případě, že je zadávání ukončeno enterem při nulové délce,
 * je nastavena defaultníí hodnota řeťězce, která je získána z počátečního stavu.
 */
void read_line_init_edefault(int y, int x, char *s,char len){ 
  int i;
  
  char_counter = 0;
  max_len = len; //(char) strlen(s);
  def_str = (char *) mmalloc(strlen(s) + 1);
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
 * Funkce načítá znaky z řádky, vykresluje přitom kurzor. Umožňuje mazání
 * znaků Backspacem. Není implementován pohyb šipkami.
 *
 * @returns
 * Návratová hodnota je NULL. Pokud je stisknut enter, mění se na ukazatel na řetězec
 * který byl vytvořen.
 * 
 * @retval NULL
 * Nebylo-li dokončeno zadávání řetězce.
 *
 * @retval char*str
 * Pokud bylo ukončeno zadávání řetězce, návratová hodnota je ukazatel na daný
 * řetězec.
 */
 char *read_line (void){
   char xpchc = sour_x + char_counter;
   char c[3] = {' ','_','\0'};
   if ((c[0] = get_spec()) != 0){
     switch (c[0]) {
      case 5: //esc
        str[0] = '\0';
        if (def_str != NULL) {
          mfree((void *) def_str);
          def_str = NULL;
        }
        return str;
        //return "";
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
         mfree((void *)def_str);
         def_str = NULL;
       }
       else {
         str[char_counter] = '\0';
         if (def_str != NULL){
           mfree((void *)def_str);
           def_str = NULL;
         }
       }
       return str;
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
 * Funkce má za úkol ukončení zadávání řetězce z venku. Zajistí uvolnění zdrojů.
 */
void read_line_kill(void){
  mfree((void *)str);
  str=NULL;
  max_len = 0;
  char_counter = 0;
  if (def_str != NULL){
    mfree((void *)def_str);
    def_str = NULL;
  }
}



// --------------menu------------
///
/// Menu se zakládá tím, že se nejprve v paměti vytvoří jednotlivé položky menu funkcí new_menu_item.
/// V těch je nadefinován ukazatel na pole popisků hodnot.
/// x,y jsou souřadnice počátku nápisu
/// val je aktuální hodnota menu
/// len je počet možných hodnot menu. Je li v poli popisků místo 
/// ukazatele na řetězec ukazatel na NULL, dochází k přeskakování této hodnoty.
MENU_ITEM *new_menu_item(void){
  MENU_ITEM *p_pom;
  p_pom = (MENU_ITEM *) mmalloc(sizeof(MENU_ITEM));
  if(p_pom == NULL){
    new_error(88);
  }
  return p_pom;
}

/**
 * Funkce vykreslí menu.
 * Vykresleny jsou jeho položky a kurzor. Je-li hodnota nějaké položky
 * NULL, skáče se na další. Jedinou výjimkou je hodnota 255. Pro ty je vypsána
 * hláška "prazdne".
 */
void menu_draw(MENU_ITEM *menu[], unsigned int len, char menu_i){
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
	  printxyd(menu[i] ->y, (menu[i] -> x), strcpy(str_buf,s_prazdne));
	else
	  printxyd(menu[i] ->y, (menu[i] -> x), menu[i] -> polozky[menu[i] -> val] );
    }
    }
  }

  i = (int) menu_i;
  printcxyd(menu[i] -> y, ((menu[i] -> x) - 1), symbol_kurzoru );
}

/**
 * Funkce posune kurzor na předchozí položku menu.
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
 * Funkce posune kurzor na další položku menu.
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
  printcxyd(menu[ii] ->y, ((menu[ii] -> x) - 1), symbol_kurzoru );

  return (char) ii;
}

/**
 * Funkce nastaví zvolenou hodnotu na další.
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

  do{ // přeskakování NULL hodnot
    // posouvání hodnot vpřed
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
  
  // domazání konce minlého řetězce
  if (len_a > len_b){
    int j;
    len_a = len_a - len_b;
    for (j=0; j < len_a; j++){
      printcxyd(menu[i] ->y, menu[i] ->x + len_b + j,' ');
    }
  }
  
  if (menu[i] -> val == 255)
    printxyd(menu[i] ->y, (menu[i] -> x), strcpy(str_buf,s_prazdne));
  else
    printxyd(menu[i] ->y, (menu[i] -> x), menu[i] -> polozky[menu[i] -> val] );
}


/* end of gui_toolbox.c */
