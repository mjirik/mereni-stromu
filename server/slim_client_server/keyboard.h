/*
 * File name: keyboard.h
 * Date:      2006/07/31 22:50
 * Author:    Mira
 *
 * keyboard.c
 */

extern void keyboard_init(void);
extern char get_char(void);
extern void read_key_ncurses(void);
extern char get_spec(void);
extern void keyboard_recv_watchdog(void);
extern void keyb_in_buff(char flag, char kod);
extern void read_key();
/* end of keyboard.h */

