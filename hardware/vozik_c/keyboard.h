/*
 * File name: keyboard.h
 * Date:      2006/07/31 22:50
 * Author:    Mira
 *
 * keyboard.c
 */

void keyboard_init(void);
char get_char(void);
char get_spec(void);
void keyboard_recv_watchdog(void);
void keyb_in_buff(char flag, char kod);
/* end of keyboard.h */

