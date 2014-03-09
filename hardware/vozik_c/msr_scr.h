/* $Id: msr_scr.h 117 2007-09-18 21:53:25Z Mira $
 */


void measure_scr_draw(void);
void measure_scr_control(void);
void measure_scr_init(void);
void measure_scr_close(void);
void measure_scr_open(void);
void set_menu_jmena_i(unsigned char i);
unsigned char get_stored_menu_jmena_i(void);
unsigned char get_menu_jmena_i(void);
void uloz_posledni_data(void);

#define MAX_ZAKAZNIK 16
extern char * m_jmena[];
//char * menu_jmena_i;
/* end of msr_scr.h */
//extern unsigned char width_src;
