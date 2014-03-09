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

extern AKT_SCR akt_scr;
extern char * menu_pridat;

extern void gui_control(void);
extern void gui_init(void);
extern void gui_prev_scr(char last_scr);
extern void ok_scr_init(char *str);
extern int ok_scr(void);
extern void scr_draw(char scr);
extern void scr_ch(char scr);
/* end of gui.h */
