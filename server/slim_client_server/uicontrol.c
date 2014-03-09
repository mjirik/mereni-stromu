#include <stdio.h>
#include "serial.h"
#include "msgn.h"
#include "main.h"

/**
 *Funckce rozsviti nebo shasne indikaci změření
 */
void uicontrol_zmereno(int q) {
  char msgqq[12] = "ahoj";
  static int stav = -1;
  if (q != stav) {
    if (q == 1) {
      //char *msg00 = "            ";
      //msg00[DATASIZE + 1] = '\0';
      msgqq[0] = MSGN_MSR_LED_ON;
      write_port(msgqq);
      //INDIKACE_PORT |= _BV(INDIKACE_BIT);
    }
    else {
      printf("x");
      //char msg[DATASIZE + 2];
      //char *msg00 = "            ";
      msgqq[0] =10;// MSGN_MSR_LED_OFF;
      //write_port(msg00);
      //INDIKACE_PORT &= ~_BV(INDIKACE_BIT);
      printf("y");
      write_port(msgqq);
      printf("z");
    }
    stav = q;
  }
}
