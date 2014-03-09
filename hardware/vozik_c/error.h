/*
 * File name: error.h
 * Date:      2006/07/27 19:44
 * Author:    Mira
 *
 * error.c
 */

#include "common/errn.h"

void new_str_error(uint8_t err_num, char * s);
void new_error(uint8_t err_num);
void error_scr_draw(void);
void error_scr_control(void);
void err_assert(char * soubor, int radek);
void err_scr_open(void);
void err_control(void);

//#define assert(x) if (x){}
#  if defined(NDEBUG)
#    define assert(e) ((void)0)
#  else /* !NDEBUG */
#    define assert(e) ((e) ? (void)0 :\
		       err_assert(__FILE__, __LINE__))
#  endif /* NDEBUG */


//char err98[];

/* end of error.h */
