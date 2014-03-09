/* 
 * File:   errn.h
 * Author: mjirik
 *
 * Created on 28. září 2009, 20:24
 */

#ifndef _ERRN_H
#define	_ERRN_H

#ifdef	__cplusplus
extern "C" {
#endif


    #define ERRN_RECIVER_OVERLOADED 103
    #define ERRN_RECIVE_BUFFER_FULL 104
    /// Toto je jen informační error, situace je ošetřena. Chyba by neměla
    /// (téměř) nikdy nastat.
    #define ERRN_RECIVE_END_OF_BUFFER_BUG 105
    #define ERRN_NO_CUSTOMER 106
	

#ifdef	__cplusplus
}
#endif

#endif	/* _ERRN_H */

