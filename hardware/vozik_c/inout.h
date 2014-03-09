/* 
 * File:   inout.h
 * Author: mjirik
 *
 * Created on 20. září 2009, 10:38
 */

#ifndef _INOUT_H
#define	_INOUT_H

#ifdef	__cplusplus
extern "C" {
#endif

void io_msr_led_on(void);
void io_msr_led_off(void);

void io_irc1_options(uint16_t k1, uint32_t q1);
void io_irc2_options(uint16_t k2, uint32_t q2);


#ifdef	__cplusplus
}
#endif

#endif	/* _INOUT_H */

