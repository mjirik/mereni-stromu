// usart.c
#include "common/msgn.h"

extern void USART_Init(unsigned int baud);
extern void new_msg(char *);
extern void new_ack_msg(char * msg, unsigned char repeats, void (*p_fcn)(char*s), void (*p_fcn_nresponse)(void));
extern void msg_ack_fcn(char * msg);
extern void serial_msg_ack_timer(void);
extern void new_msg_P(PGM_P s_p);
extern char usart_get_recv_buff(int i);
extern void serial_read_buffer(void);


extern void send_uint32(char msgn, uint32_t number1, uint32_t number2);
extern void recive_uint32(char * msg, uint32_t * number1, uint32_t * number2);

extern void msg_time_0(void);
extern void msg_time_pp(void);
extern uint32_t get_msg_time(void);
extern char* get_recv_buffer(void);


extern uint32_t msg_time;

/** Velikost dat - sta�� zm�nit zde */
#define DATASIZE 10
/** Délka zprávy */
#define LenOfMsg DATASIZE + 1
/** Délka přijímacího bufferu */
#define LenOfRecvBuff 2 + DATASIZE + 1 + (DATASIZE / 2)  
/** Délka bufferu pro odeslání */
#define LenOfTransBuff 2 + DATASIZE + 1 + (DATASIZE / 2)

/// počet bytů LCD odesílaných po sériové lince. Musí být menší nebo roven
/// DATASIZE
#define LCD_NBYTES 10

