#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INTERVAL 100000

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

   printf("Number of tasks = %d My rank = %d  Running on %s\n\n",numtasks,rank,hostname);

    int numworkers = numtasks-1 ;   
    if (rank == 0)
    {           
       //printf("Master process \n") ;
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
            
    }
   else
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