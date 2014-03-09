/**
 * @file sysinfo.c
 * @brief
 * Modul se stará o výpis systémových informací na LCD.
 *
 * Funkce typu print jednorázově vytisknou informaci na LCD,
 * funkce show vypisují informace neustále
 */

#include <stdint.h>
#include <stdio.h>
#include <avr/pgmspace.h> // kvuli inkludovanému main.h  :-(
#include "memory.h"
#include "lcd.h"
#include "main.h"
#include "usart.h"
#include "sysinfo.h"
#include "watchdog.h"

//static char show_used_mem = 0;
char sysinfo_flag = 0 ; //(1<<SI_SHOW_RUN)|(1<<SI_SHOW_DBGIRC1)|(1<<SI_SHOW_DBGIRC2);


/**
 * Funkce nastaví vlajkové bity, podle kterých jsou či nejsou zobrazovány
 * systémové informace
 */
void si_set_flag(char flag){
  sysinfo_flag = flag;
}



/**
 * Funkce vypíše obsah přijímacího bufferu.
 */
void si_print_recv_buffer(void){
  if (((sysinfo_flag >> SI_SHOW_RECV_BUFF)&1) == 1) {
    char * rb;
    rb = get_recv_buffer();
    sprintf(str_buf, "%2x,%2x,%2x,%2x,%2x,%2x,%2x,%2x,%2x,%2x",
            (unsigned int) rb[0], (unsigned int) rb[1], (unsigned int) rb[2], (unsigned int) rb[3],
            (unsigned int) rb[4], (unsigned int) rb[5], (unsigned int) rb[6], (unsigned int) rb[7],
            (unsigned int) rb[8], (unsigned int) rb[9]
            );
    printxyd(2, 0, str_buf);

    sprintf(str_buf, "%2x,%2x,%2x,%2x,%2x,%2x,%2x,%2x",
            (unsigned int) rb[10], (unsigned int) rb[11], (unsigned int) rb[12], (unsigned int) rb[13],
            (unsigned int) rb[14], (unsigned int) rb[15], (unsigned int) rb[16], (unsigned int) rb[17]
            );
    printxyd(3, 0, str_buf);
  }
}


/**
 * Funkce vypisuje číslo erroru.
 */
void si_print_error(char err_num) {
  if (((sysinfo_flag >> SI_SHOW_ERR)&1) == 1) {
    sprintf(str_buf, "E%3d", err_num);
    printxyd(0, 32, str_buf);
  }
}

/*  funkce jsou nahrazeny v .h inline definicemi
void si_print_dbgirc1(char c){
  if (((sysinfo_flag >> SI_SHOW_DBGIRC1)&1) == 1) {
    printcxyd(0,0,c + '0');
  }
}
void si_print_dbgirc2(char c){
  if (((sysinfo_flag >> SI_SHOW_DBGIRC2)&1) == 1) {
    printcxyd(0,0,c + '0');
  }
}
*/

void si_print_mem(void){
  if (((sysinfo_flag >> SI_SHOW_MEM)&1) == 1) {
      uint16_t mem;
      mem = used_mem();
      sprintf(str_buf, "M%3d", mem);
      printxyd(0, 36, str_buf);
    }
}

/**
 * Funkce vypisuje systémové údaje na LCD. Vypisovány jsou jen údaje dle
 * nastavení. Výpis probíhá jedmnou za deset volání.
 */
void sysinfo(void) {
  static char counter = 0;

  wtch_dbg_info();
  if (counter > 50) {
    si_print_mem();
    //si_print_recv_buffer();
    counter = 0;
  }
  counter++;
}