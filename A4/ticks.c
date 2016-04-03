#include "cycle.h" 
#include "stdio.h"
#include "stdlib.h"
#define LONG 2814585000
int main () {

   int i;
   ticks t1;
   ticks t2;
   double mytime1;
   double mytime2;

   for ( i=0; i<10; i++ ) {
      t1 = getticks();
      sleep(1);
      t2 = getticks();
      mytime1 = elapsed(t2,t1);
      mytime2 = elapsed(t2,t1) / LONG;
      printf ("%f	%f\n",mytime1,mytime2);
   }

}

