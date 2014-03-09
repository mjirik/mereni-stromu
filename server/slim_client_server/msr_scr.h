/* $Id: msr_scr.h 117 2007-09-18 21:53:25Z Mira $
 */
#define MAX_ZAKAZNIK 16


extern void measure_scr_draw(void);
extern void measure_scr_control(void);
extern void measure_scr_init(void);
extern void measure_scr_close(void);
extern void measure_scr_open(void);
extern void set_menu_jmena_i(unsigned char i);
extern unsigned char get_stored_menu_jmena_i(void);
extern unsigned char get_menu_jmena_i(void);
extern void uloz_posledni_data(void);

//extern char * m_jmena[];
//char * menu_jmena_i;
/* end of msr_scr.h */
//extern unsigned char width_src;
