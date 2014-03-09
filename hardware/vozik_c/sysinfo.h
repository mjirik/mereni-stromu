/* 
 * File:   sysinfo.h
 * Author: mjirik
 *
 * Created on 29. září 2009, 19:30
 */

#ifndef _SYSINFO_H
#define	_SYSINFO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common/sysinfo_flag.h"

extern void sysinfo(void);
extern void si_set_flag(char flag);
extern void si_print_recv_buffer(void);
extern void si_print_error(char err_num);
//extern void si_print_dbgirc1(char c);
//extern void si_print_dbgirc2(char c);
extern char sysinfo_flag;


#define si_print_run(c) if(((sysinfo_flag >> SI_SHOW_RUN)&1) == 1) printcxyd(0,0,(c))
//(if (((sysinfo_flag >> SI_SHOW_RUN)&1) == 1) {printcxyd(0,0,(c));})
#define si_print_dbgirc1(c) if (((sysinfo_flag >> SI_SHOW_DBGIRC1)&1) == 1) printcxyd(0,1,(c) + '0')
#define si_print_dbgirc2(c) if (((sysinfo_flag >> SI_SHOW_DBGIRC1)&1) == 1) printcxyd(0,2,(c) + '0')




#ifdef	__cplusplus
}
#endif

#endif	/* _SYSINFO_H */

