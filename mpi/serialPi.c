
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timing.c"

#define INTERVAL 100000

int main()
{

           printf(" ****************************************************\n");
           printf("   Monte carlo simulations (PI calculation) serial   \n");
           printf(" ****************************************************\n");
    
       timing_start(); 
       srand(time(NULL)); 
       int inCircle = 0 , total = 0; 
       double piApproximation = 0;
       while( total <  INTERVAL)
       {
          double x = (double)random()/RAND_MAX;
          double y = (double)random()/RAND_MAX;   

          if (sqrt((x*x)+(y*y)) <= 1.0)
                    inCircle++;

          total++ ;

          piApproximation =  4.0 * ((double)inCircle / (double)total); 
       }

      
      printf("Approximated Pi = %f\n",piApproximation);
      timing_stop();
      print_timing();
 }