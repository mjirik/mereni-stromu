/* $Id: convert.c 117 2007-09-18 21:53:25Z Mira $
 * File name: convert.c
 * Date:      2006/07/21 17:05
 * Author:    Mira
 */
/**
 * \file convert.c
 * \brief
 * Zde jsou prov�d�ny konverze ��seln�ch typ� na string.
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
 * Funkce vypln� dan� �et�zec mezerami. Po�et mezer je ur�en argumentem len. Za mezery je um�st�n
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


/// Funkce vrac� d�lku �et�zce.
int strlen(char * s){
  int i = 0;
  while (s[i] != '\0'){
    i++;
  }
  return i;
}



/**
 * Funkce p�ev�d� ��slo na �et�zec. Vstupem je �et�zec. Zm��� se jeho d�lka a na takovou d�lku se p�evede 
 * ��slo. Pokud je �et�zec p��li� kr�tk�, dojde k vyps�n� chybov� zpr�vy. Zarovn�v� se vpravo.
 * Pokud je prvn� znak vstupn�ho �et�zce roven '0', vypisuj� se i po��te�n� nuly
 * nap� 0056
 */
char * int2str(char *s, uint16_t cislo){
  
  return int2strn(s,strlen(s), cislo);
}

/**
 * Funkce p�ev�d� ��slo na �et�zec
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

  if ((cislo/zaklady[i]) >= 10){ //tohle testuje jestli neni ��slo na v�c m�st ne� je mo�n� zobrazit
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
