void eeprom_load(void);
void eeprom_save(void);
char eeprom_r8(void * uiAddress);
void eeprom_w8(void * uiAddress, uint8_t c);

void eeprom_w16(void * uiAddress, uint16_t word);
uint16_t eeprom_r16(void * uiAddress);
void eeprom_w32(void *uiAddress, uint32_t data);
uint32_t eeprom_r32(void *uiAddress);

// eeprom.c
// mapa eeprom

#define ee_irc_counter 0 //size 4 Byte
#define ee_irc_k 4 //size 2 Byte
#define ee_irc_set_on_value 6//size 4 Byte

