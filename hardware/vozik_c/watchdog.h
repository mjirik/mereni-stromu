#include "lcd.h"

extern void wtch_main(void);
extern void wtch_init(void);

extern int16_t wtch_counter;
extern char * wtch_sfile;
extern int wtch_sline;
//extern char * wtch_sfunction;


/// Makro pouřívané k resetování watchodogu.
#define wtch_reset() wtch_counter = (wtch_counter < 0)?wtch_counter:0;

/// Makro používané k resetu obou watchdogů.
#define wtch_reset2() wtch_counter = (wtch_counter < 0)? wtch_counter:0; wdt_reset();

/// Vypnutí watchdogu.
#define wtch_disable2() wtch_counter = -1; wdt_disable();

#define wtch_enable2() wtch_counter = 0; wdt_enable(WDTO_2S);

#define wtch_dbg_info() wtch_sfile = __FILE__; wtch_sline = __LINE__; //{char str[10]; lcd_w_inst(0x01); printnt(wtch_sfile); sprintf(str," %i",wtch_sline); printnt(str);}


