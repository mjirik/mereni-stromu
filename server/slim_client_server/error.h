/*
 * File name: error.h
 * Date:      2006/07/27 19:44
 * Author:    Mira
 *
 * error.c
 */

extern void new_str_error(unsigned int err_num, char * s);
extern void new_error(unsigned int err_num);
extern void error_scr_draw(void);
extern void error_scr_control(void);
extern void err_assert(char * soubor, int radek);
extern void err_scr_open(void);
extern void err_control(void);

//#define assert(x) if (x){}
#  if defined(NDEBUG)
#    define assert(e) ((void)0)
#  else /* !NDEBUG */
#    define assert(e) ((e) ? (void)0 :\
		       err_assert(__FILE__, __LINE__))
#  endif /* NDEBUG */


//char err98[];

/* end of error.h */
