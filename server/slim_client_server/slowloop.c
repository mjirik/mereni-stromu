#include <stdio.h>
#include <time.h>

//global
int time4slowloop(void);
//global

/// čas provádění dlouhé smyčky v sekundách
#define SLOWLOOP_TIME 200

static time_t cas0 = 0;



int time4slowloop(){
  time_t cas1;
  double rozdil;
  long q;
  cas1 = time(&cas1);

  q = cas1;
  rozdil  = difftime(cas1,cas0);
  //printf("cashhh : %f\n",rozdil);
  if (rozdil > SLOWLOOP_TIME){
    cas0 = cas1;
    //printf("prekresleni\ndalsi text");
    return 1;
  }
  else{
    return -1;
  }
}
