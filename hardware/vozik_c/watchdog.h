void wtch_main(void);
void wtch_init(void);

/// Makro pouøívané k resetování watchodogu.
#define wtch_reset() wtch_counter = (wtch_counter < 0)?wtch_counter:0;

/// Makro používané k resetu obou watchdogù.
#define wtch_reset2() wtch_counter = (wtch_counter < 0)? wtch_counter:0; wdt_reset();

/// Vypnutí watchdogu.
#define wtch_disable2() wtch_counter = -1; wdt_disable();

#define wtch_enable2() wtch_counter = 0; wdt_enable(WDTO_2S);

uint16_t wtch_counter;

