/* 
 * File:   camera.h
 * Author: mjirik
 *
 * Created on 4. říjen 2009, 13:04
 */

#ifndef _CAMERA_H
#define	_CAMERA_H

#ifdef	__cplusplus
extern "C" {
#endif

extern void camera_init(char * fimg_url, char * fcopyURL_path);
extern void camera(void);
extern void camera_close(void);

#ifdef	__cplusplus
}
#endif

#endif	/* _CAMERA_H */

