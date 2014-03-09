/* 
 * File:   memory.h
 * Author: mjirik
 *
 * Created on 25. září 2009, 20:35
 */

#ifndef _MEMORY_H
#define	_MEMORY_H

#ifdef	__cplusplus
extern "C" {
#endif

extern void * mmalloc (unsigned int size);
extern void mfree(void * pointer);
extern uint16_t used_mem(void);


#ifdef	__cplusplus
}
#endif

#endif	/* _MEMORY_H */

