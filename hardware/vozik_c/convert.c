/* $Id: convert.c 125 2009-09-13 09:16:22Z mjirik $
 * File name: convert.c
 * Date:      2006/07/21 17:05
 * Author:    Mira
 */
/**
 * \file convert.c
 * \brief
 * Zde jsou prováděny konverze číselných typů na string.
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "convert.h"
#include "lcd.h"
#include "main.h"
#include "error.h"

//static char err[] = "Err - chyba prevodu";

/**
 * Funkce vyplní daný řetězec mezerami. Počet mezer je určen argumentem len. Za mezery je umístěn
 * znak '\0'.
 */
char * mezery(char *s,char len){
  int i;
  for (i = 0; i < len; i++){
    s[i] = ' ';
  }
  s[(int)len]='\0';
  return s;
}


/// Funkce vrací délku řetězce.
int strlen(char * s){
  int i = 0;
  while (s[i] != '\0'){
    i++;
  }
  return i;
}



/**
 * Funkce převádí číslo na řetězec. Vstupem je řetězec. Změří se jeho délka a na takovou délku se převede 
 * číslo. Pokud je řetězec příliš krátký, dojde k vypsání chybové zprávy. Zarovnává se vpravo.
 * Pokud je první znak vstupního řetězce roven '0', vypisují se i počáteční nuly
 * např 0056
 */
char * int2str(char *s, uint16_t cislo){
  
  return int2strn(s,strlen(s), cislo);
}

/**
 * Funkce převádí číslo na řetězec
 */
char * int2strn(char * s, int len, uint16_t cislo){
//  char s[6]= "     \0";
  //uint16_t zaklad;
  int nenula = 0;
  int i;
  uint16_t zaklady [5] = {1,10,100,1000,10000};
  
  s[len] = '\0';
  if (s[0] == '0'){
    nenula =  1;

  }

/*
  for (int i=0; i < (len - 1);i++){
    zaklad = zaklad * 10;
  }
*/
  if (len < 5){
    i = len - 1;
  }
  else {
    i = 4;
  }
  
  //zaklad = zaklady[i];

  if ((cislo/zaklady[i]) >= 10){ //tohle testuje jestli neni číslo na víc míst než je možný zobrazit
    //new_error(85);
  }
  else{
    if (i < 4)
      cislo = cislo % (zaklady[i+1] );
    else
      cislo = cislo % (zaklady[4]);
  }
  while (i >= 0){ //zaklad >= 1){//i < len){
    int mezi;
    mezi = (cislo / zaklady[i]);
    if ((mezi == 0)&&(nenula == 0)&&(zaklady[i] > 1)){
      s[len - i - 1] = ' '; 
    }
    else{
      s[len - i - 1] = 48 + mezi;
      if (nenula == 0){
        nenula = 1;
      }
    }
    cislo = cislo - ((cislo/zaklady[i]) * zaklady[i]);
    //cislo = cislo % zaklady[i];
    i--;
  }
  
  return s;
}

/* end of convert.c */
