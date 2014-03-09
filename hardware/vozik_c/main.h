#define SizeOfMsg 11
//char data_msg[SizeOfMsg];
extern char str_buf[41];
extern char * getstr_P( PGM_P s_p);

extern void my_watchdog(void);

#//define _TICTOC

#ifdef _TICTOC
extern void tic(void);
extern uint16_t toc(void);
#endif