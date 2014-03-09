void eeprom_load(void);
void eeprom_save(void);
char eeprom_r8(void * uiAddress);
void eeprom_w8(void * uiAddress, uint8_t c);

void eeprom_w16(void * uiAddress, uint16_t word);
uint16_t eeprom_r16(void * uiAddress);
void eeprom_w32(void *uiAddress, uint32_t data);
uint32_t eeprom_r32(void *uiAddress);

extern unsigned char width_src;

// eeprom.c
// mapa eeprom

/*
#define ee_irc_counter 0 //size 4 Byte
#define ee_irc_k 4 //size 2 Byte
#define ee_irc_set_on_value 6//size 4 Byte
#define ee_width_src 10// 1 byte
#define ee_flag 11 // 1 byte
#define ee_lst 900 // 9*16
#define ee_data 156 // asi 105*7
*/

// píší se velikosti dat
#define ee_irc_counter_size 4 //size 4 Byte
#define ee_irc_k_size 2 //size 2 Byte
#define ee_irc_set_on_value_size 4//size 4 Byte
#define ee_width_src_size 1// 1 byte
#define ee_flag_size 1 // 1 byte
#define ee_lst_size 9*16 // 9*16
#define ee_data_size 105*7 // asi 105*7
//#define ee_width_src_size 1

#define ee_irc_counter 0 //size 4 Byte
#define ee_irc_k (ee_irc_counter + ee_irc_counter_size)
#define ee_irc_set_on_value (ee_irc_k + ee_irc_k_size)
#define ee_width_src (ee_irc_set_on_value + ee_irc_set_on_value_size)
#define ee_flag (ee_width_src + ee_width_src_size)
#define ee_lst (ee_flag + ee_flag_size)
#define ee_data (ee_lst + ee_lst_size)
//#define ee_width_src (ee_data + ee_data_size)


