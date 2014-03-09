void wtch_main(void);
void wtch_init(void);

/// Makro pou��van� k resetov�n� watchodogu.
#define wtch_reset() wtch_counter = (wtch_counter < 0)?wtch_counter:0;

/// Makro pou��van� k resetu obou watchdog�.
#define wtch_reset2() wtch_counter = (wtch_counter < 0)? wtch_counter:0; wdt_reset();

/// Vypnut� watchdogu.
#define wtch_disable2() wtch_counter = -1; wdt_disable();

#define wtch_enable2() wtch_counter = 0; wdt_enable(WDTO_2S);

uint16_t wtch_counter;

