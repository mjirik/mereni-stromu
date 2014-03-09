/* 
 * File:   inout.h
 * Author: mjirik
 *
 * Created on 20. září 2009, 15:44
 */

#ifndef _INOUT_H
#define	_INOUT_H

#ifdef	__cplusplus
extern "C" {
#endif

extern void io_lcd2vozik(void);
extern void io_control_zmereno(int q);
extern void io_all_lcd2vozik(void);
extern int io_get_ind(void);

#ifdef	__cplusplus
}
#endif

#endif	/* _INOUT_H */

