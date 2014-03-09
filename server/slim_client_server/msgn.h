/* 
 * File:   msgn.h
 * Author: mjirik
 *
 * Created on 15. září 2009, 22:57
 */

#ifndef _MSGN_H
#define	_MSGN_H

#ifdef	__cplusplus
extern "C" {
#endif

// odesílá kód stlačené klávesy
#define MSGN_KEY '\x60'

#define MSGN_IRC '\x61'

#define MSGN_ERR '\x62'

#define MSGN_LCD00 '\x70'
#define MSGN_LCD01 '\x71'
#define MSGN_LCD02 '\x72'
#define MSGN_LCD03 '\x73'
#define MSGN_LCD04 '\x74'
#define MSGN_LCD05 '\x75'
#define MSGN_LCD06 '\x76'
#define MSGN_LCD07 '\x77'
#define MSGN_LCD08 '\x78'
#define MSGN_LCD09 '\x79'
#define MSGN_LCD10 '\x7A'
#define MSGN_LCD11 '\x7B'
#define MSGN_LCD12 '\x7C'
#define MSGN_LCD13 '\x7D'
#define MSGN_LCD14 '\x7E'
#define MSGN_LCD15 '\x7F'


#define MSGN_MSR_LED_ON  '\x90'
#define MSGN_MSR_LED_OFF '\x91'

#define MSGN_IRC1_OPTIONS '\x92'
#define MSGN_IRC1_VAL '\x93'
#define MSGN_IRC2_OPTIONS '\x94'
#define MSGN_IRC2_VAL '\x95'

#ifdef	__cplusplus
}
#endif

#endif	/* _MSGN_H */

