#include <stdio.h>
#include "msgn.h"
#include "serial.h"
#include "lcd.h"
#include "inout.h"

// global
void io_lcd2vozik(void);
void io_control_zmereno(int q);
void io_all_lcd2vozik(void);
int io_get_ind(void);
// global
static int io_int = 0;


#define LCD_NBYTES 10


///
void io_all_lcd2vozik(){
  zmena_na_lcd = 0xFFFF;
}

int io_get_ind(void){
  return io_int;
}

/// Funkce odesílá obsah obrazovky po sériové lince na LCD. Odesílány jsou jen
/// části, u kterých došlo ke změně.
void io_lcd2vozik(void) {
  if (zmena_na_lcd != 0) {
    int i, j, k;
    char msg[DATASIZE + 2];

    msg[DATASIZE + 1] = '\0';
    //printf("vypis po seriové lince\n");
    for (j = 0; j < 4; j++) {

      for (i = 0; i < 4; i++) {
        // zjištění zda došlo ke změně v této části LCD
        if ( ((zmena_na_lcd >> ((j * 4) + i)) & 1)  == 1) {
          // identifikátor zprávy, očekává se, že identifikátory jdou "po sobě"
          msg[0] = MSGN_LCD00 + (j * 4) + i;

          for (k = 0; k < DATASIZE; k++) {
            msg[k + 1] = default_screen[j][i * LCD_NBYTES + k];
          }
          write_port(msg);
          //printf("|%s|", msg);
        }
      }
      //printf("\n");
    }
  }
}


/**
 *Funckce rozsviti nebo shasne indikaci změření
 */
void io_control_zmereno(int q) {
  char msg[12] = "ahoj";
  static int stav = -1;
  if (q != stav) {
    if (q == 1) {
      //char *msg00 = "            ";
      //msg00[DATASIZE + 1] = '\0';
      io_int = 1;
      msg[0] = MSGN_MSR_LED_ON;
      write_port(msg);
      //INDIKACE_PORT |= _BV(INDIKACE_BIT);
    }
    else {
      //printf("x");
      //char msg[DATASIZE + 2];
      //char *msg00 = "            ";
      io_int = 0;
      msg[0] = MSGN_MSR_LED_OFF;
      //write_port(msg00);
      //INDIKACE_PORT &= ~_BV(INDIKACE_BIT);
      write_port(msg);
    }
    stav = q;
  }
}
