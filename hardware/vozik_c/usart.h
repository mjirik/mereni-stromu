// usart.c
void USART_Init( unsigned int baud );
void new_msg (char *);
void new_ack_msg(char * msg, unsigned char repeats, void (*p_fcn)(char*s),void (*p_fcn_nresponse)(void));
void msg_ack_fcn(char * msg);
void serial_msg_ack_timer(void);
void new_msg_P(PGM_P s_p);
char usart_get_recv_buff(int i);


/** Velikost dat - staèí zmìnit zde */
#define DATASIZE 10
/** Délka zprávy */
#define LenOfMsg DATASIZE + 1
/** Délka pøijímacího bufferu */
#define LenOfRecvBuff 2 + DATASIZE + 1 + (DATASIZE / 2)  
/** Délka bufferu pro odesílání */
#define LenOfTransBuff 2 + DATASIZE + 1 + (DATASIZE / 2)
