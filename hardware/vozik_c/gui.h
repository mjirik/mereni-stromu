/*
 * File name: gui.h
 * Date:      2006/07/27 11:30
 * Author:    Mira
 */
// gui.c
#include <inttypes.h>


typedef enum{
  MSR_SCR,
  LST_SCR, ///@todo odstranit LST_SCR
  HLP_SCR,
  ERR_SCR,
  DAT_SCR,
  DLST_SCR,
  OK_SCR,
  SER_SCR,
  STP_SCR,
} AKT_SCR;

AKT_SCR akt_scr;
char * menu_pridat;

void gui_control(void);
void gui_init(void);
void gui_prev_scr(char last_scr);
void ok_scr_init(char *str);
int ok_scr(void);
void scr_draw(char scr);
void scr_ch(char scr);
/* end of gui.h */
