// usart.c
void USART_Init( unsigned int baud );
void new_msg (char *);
void new_ack_msg(char * msg, unsigned char repeats, void (*p_fcn)(char*s),void (*p_fcn_nresponse)(void));
void msg_ack_fcn(char * msg);
void serial_msg_ack_timer(void);
void new_msg_P(PGM_P s_p);
char usart_get_recv_buff(int i);


/** Velikost dat - sta�� zm�nit zde */
#define DATASIZE 10
/** D�lka zpr�vy */
#define LenOfMsg DATASIZE + 1
/** D�lka p�ij�mac�ho bufferu */
#define LenOfRecvBuff 2 + DATASIZE + 1 + (DATASIZE / 2)  
/** D�lka bufferu pro odes�l�n� */
#define LenOfTransBuff 2 + DATASIZE + 1 + (DATASIZE / 2)
