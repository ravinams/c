#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timing.c"

#define INTERVAL 100000
#define MASTER 0 

int main(int argc, char *argv[])
{
   int numtasks, rank , len , rc  ;
   char hostname[MPI_MAX_PROCESSOR_NAME] ;
   MPI_Status status;
   rc = MPI_Init(&argc ,&argv) ;
   if (rc != MPI_SUCCESS)
   {
   	 printf("Error starting the MPI program. Terminating. \n");
   	 MPI_Abort(MPI_COMM_WORLD,rc) ;
   }

   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
   MPI_Get_processor_name(hostname,&len);  

   if (numtasks < 2 ) {
        printf("Need at least two MPI tasks. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    } 
   int numworkers = numtasks-1 ;   
   // printf("Number of tasks = %d My rank = %d  Running on %s\n\n",numtasks,rank,hostname);

   
    /**************************** master task ************************************/
     if (rank == MASTER) 
    {           
       //printf("Master process \n") ;

    	  printf(" ****************************************************\n");
          printf("   Monte carlo simulations (PI calculation) MPI   \n");
          printf(" ****************************************************\n");

       timing_start();
       double piApproximation = 0;
       int i =0 , totalInCircle = 0;;
       for ( i = 1 ; i <= numworkers ; i++)
       {                
         int inCircle = 0 ;
         MPI_Recv(&inCircle, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status); 
         //printf("recieved Child process %d - %d \n",i,inCircle) ;

        totalInCircle = totalInCircle + inCircle ;
       }

       piApproximation =  4.0 * ((double)totalInCircle / (double)INTERVAL); 
       printf("Approximated Pi = %f\n",piApproximation);
       timing_stop();
       print_timing();
            
    }   
   else  /**************************** worker task ************************************/
   {

   	   // Initializing rand() 
       srand(time(NULL)); 
       int inCircle = 0 , i =0 ;
       for ( i = 1 ; i <= INTERVAL/numworkers ; i++)
       {
          double x = (double)random()/RAND_MAX;
          double y = (double)random()/RAND_MAX;   

          if (sqrt((x*x)+(y*y)) <= 1.0)
                    inCircle++;
       }
  

       //printf("Child process \n") ;
   MPI_Send(&inCircle, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
   
   //printf("send Child process %d - %d \n",rank,inCircle) ;

   }
  

   MPI_Finalize() ;
}