#include <inttypes.h>
#include <avr/io.h>

#include "inout.h"
#include "irc.h"
#include "irc2.h"

/** @name Pin indikace stavu
 * Proměnné definují umístění pinu, jež zajišťuje indikaci stavu (msr_status)
 * @todo zjistit správný pin
 * @{
 */
#define MSR_LED_PORT PORTB
#define MSR_LED_DDR DDRB
#define MSR_LED_PIN PINB
#define MSR_LED_BIT 7
/** @} */

void inout_init(void){
  // msrled
  //io_msr_led_off();
}

void io_msr_led_on(void){
  MSR_LED_PORT |= _BV(MSR_LED_BIT);
}

void io_msr_led_off(void){
  MSR_LED_PORT &= ~_BV(MSR_LED_BIT);
}

///  Funkce nastavuje parametry irc1
/// @param k1 milimetry na otáčku
/// @param q1 kolik milimetrů od počátku je setpoint
void io_irc1_options(uint16_t k1, uint32_t q1){
  set_irc1_k_mm ( k1) ;
  set_irc1_set_on_value_mm( q1);
}

///  Funkce nastavuje parametry irc2
/// @param k2 milimetry na otáčku
/// @param q2 kolik milimetrů od počátku je setpoint
void io_irc2_options(uint16_t k2, uint32_t q2){
  set_irc2_k_mm ( k2) ;
  set_irc2_set_on_value_mm( q2);
}


