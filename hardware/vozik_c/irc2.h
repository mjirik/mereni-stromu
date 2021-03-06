/* 
 * File:   irc2.h
 * Author: mjirik
 *
 * Created on 21. září 2009, 21:49
 */

#ifndef _IRC2_H
#define	_IRC2_H

#ifdef	__cplusplus
extern "C" {
#endif


// irc.c
extern void irc2_watch(void);
extern void set_irc2_k_mm(uint16_t);
extern uint16_t irc2_get_l(void);
extern void set_irc2_set_on_value_mm(int32_t q);
extern uint16_t irc2_get_k(void);
extern void irc2_init(void);

extern int32_t get_irc2_set_on_value(void);

extern void set_irc2_counter (int32_t a);
extern int32_t get_irc2_counter(void);
extern void set_irc2_k(uint16_t a);
extern void set_irc2_set_on_value(int32_t a );
//int32_t irc_counter;
//int32_t irc_set_on_value;

//uint16_t irc_k;

// irc.c



#ifdef	__cplusplus
}
#endif

#endif	/* _IRC2_H */

