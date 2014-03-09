/**
 * @file camera.c
 * Modul řeší zpracování signálu z kamery.
 */
// překad
// copiler, přidat cestu do opencv1.2/include/opencv
// linker, přidat všechny knihovny opencv1.2/lib/*.lib


#ifdef MODUL_CAMERA
#include <stdio.h>
#include <stdint.h>
#include <cv.h>
#include <highgui.h>
#include "camera.h"
#include "memory.h"

/* globální proměnné */
void camera_init(char * fimg_url, char * fcopyURL_path);
void camera(void);
void camera_close(void);
/* globální proměnné */

static char * img_url;
static char * copyURL_path;


/**
 * Funkce vyšle požadavek na získání souboru obrazu z kamery. Získávání se
 * provádí v novém vlákně - nezávisle na běhu tohoto programu, nečeká se na dokončení.
 */
void get_img(void){
  char * str ;

  str = (char *) mmalloc(200);

//  sprintf(str, "java -jar \"copyURL.jar\" %s snapshot.jpg",img_url);
  #ifdef WIN32
  sprintf(str, "cmd /C java -jar \"copyURL.jar\" %s snapshot.jpg",img_url);
  #else
  sprintf(str, "java -jar \"copyURL.jar\" %s snapshot.jpg &",img_url);
  #endif
  printf ("%s\n",str);
  system(str);
  mfree((void *)str);

}




void camera_init(char * fimg_url, char * fcopyURL_path){
  img_url = fimg_url;
  copyURL_path = fcopyURL_path;
  // vyšleme požadavek na první obrázek
  get_img();
  cvNamedWindow("Mereni",CV_WINDOW_AUTOSIZE);
}



void camera(void){
  //system("java -jar \"copyURL.jar\" http://www.spindleruv-mlyn.com/_kamera/1024x768-cam0.jpg snapshot.jpg");
  //http://www.spindleruv-mlyn.com/_kamera/1024x768-cam0.jpg
  IplImage* img = cvLoadImage( "snapshot.jpg"/*"d:/tmp/rychlost/1.jpg"*/ ,CV_LOAD_IMAGE_COLOR);

  // jestli se obrázek teprve stahuje, není dostupný k otevření
  if (img != NULL){

    // zpracování obrázku
    cvShowImage("Mereni", img );
    cvWaitKey(1);
    cvReleaseImage( &img );

    // požadavek na další obrázek
    get_img();
  }
  else{
    printf("obrázek neni\n");
  }

}


void camera_close(void){
  cvDestroyWindow( "Mereni" );
}

#endif
