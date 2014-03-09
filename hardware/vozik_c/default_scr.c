

#include <avr/pgmspace.h>
#include <stdio.h>
#include "lcd.h"
#include "irc.h"
#include "irc2.h"
#include "default_scr.h"
#include "main.h"
#include "convert.h"

/// @name PROGMEM
/// Texty jsou uloženy v programové paměti. Dojde tak k úspoře místa
// @{
static char s_zakz[] PROGMEM = "Server nedosazen ";
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

void default_scr_draw(void){
  clear_dscreen();
//  akt_scr = MSR_SCR;
  printxyd(1,0, strcpy_P(str_buf,s_zakz));
  //printxyd(1,0, strcpy_P(str_buf,s_drev));
  //printxyd(2,0, strcpy_P(str_buf,s_oper));
  //printxyd_P(3,0, (void*) s_dat_scr);

  printxyd(1, 25, strcpy_P(str_buf,s_delka));
  printxyd(2, 25, strcpy_P(str_buf,s_prumer_v));


  printcxyd(1,37, '.');
  printcxyd(2,37, '.');



}

void default_scr_control(void){
  static uint16_t  tmp1=-1;
  static uint16_t  tmp2=-1;
  uint16_t tmp;
  /*sprintf(str_buf,"%d",toc());
  printxyd(0,20,str_buf); */
  //printxyd( 1,38, int2strn(str_buf,2,(irc1_get_l() % 100)));
  //str_buf[0] = ' ';

  tmp = irc1_get_l();
  if (tmp != tmp1){
    sprintf(str_buf,"%2d.%d",(tmp/100), tmp%100);// / 100.0));
    printxyd( 1,34, str_buf);
    tmp1 = tmp;
  }
  
  tmp = irc2_get_l();
  if (tmp != tmp2){
    sprintf(str_buf,"%2d.%d",tmp/10,tmp%10);//) / 10.0);
    printxyd( 2,34,str_buf);
  //printxyd( 2,34, int2strn(str_buf,3,(irc2_get_l() / 10)));
    tmp2=tmp;
  }

  //printcxyd( 2,38,(irc2_get_l()  % 10) + 48);
}