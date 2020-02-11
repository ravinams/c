#include <stdio.h>
#include <stdlib.h>
#include "timing.h"
#include "timing.c"
#include "mpi.h"

int matrixSize ;


const double min = 1;
const double max = 100;

double getRand();
void populateMatrix(int size , double matrix[][size]);
//void printMatrixToFIle(double **matrix, const char *filename);
void printMatrixToConsole(int size , double matrix[][size]);
void *multiplyMatrix(void *arg);
double matrixDotProduct(int row, int column);

#define MASTER 0               /* taskid of first process */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

int main(int argc, char** argv) {

   int numtasks, rank , len , rc  ;
   char hostname[MPI_MAX_PROCESSOR_NAME] ;

   int arraymsg = 1;       /* setting a message type */
   int indexmsg = 2;       /* setting a message type */

   int source , /* origin task id of message */
       dest ;   /* destination task id to send message */

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
 
 
 if ( rank ==0 && argc < 2) {
        perror("\n Usage ./mpi_matrix_multiply.o <squareMatrixSize> \n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(-1);
    }

    int numworkers = numtasks-1 ;   
     matrixSize = atoi(argv[1]);

    double matrix_a[matrixSize][matrixSize], matrix_b[matrixSize][matrixSize] , matrix_output[matrixSize][matrixSize];
    
    /**************************** master task ************************************/
    if (rank == MASTER) 
    {           
       //printf("Master process \n") ;

          printf(" ****************************************************\n");
          printf("   Matrix Multiplication MPI   \n");
          printf(" ****************************************************\n");

          timing_start();

          printf("\nSize of the matrix (%d * %d)= %d", matrixSize, matrixSize, matrixSize * matrixSize);
          printf("\nNumber of worker processes = %d\n", numworkers+1);

   // The below section initialize and generate matrix_a
    // int a = 0;
    // matrix_a = (double **) malloc(matrixSize * sizeof (double *));
    // for (a = 0; a < matrixSize; a++)
    //     matrix_a[a] = (double *) malloc(matrixSize * sizeof (double));

    populateMatrix(matrixSize,matrix_a);
    printf("\nMatrix A \n");
    printMatrixToConsole(matrixSize,matrix_a);

    // This section initializes and generates matrix_b
    // int b = 0;
    // matrix_b = (double **) malloc(matrixSize * sizeof (double *));
    // for (b = 0; b < matrixSize; b++)
    //     matrix_b[b] = (double *) malloc(matrixSize * sizeof (double));

    populateMatrix(matrixSize,matrix_b);
    printf("\nMatrix B \n");
    printMatrixToConsole(matrixSize,matrix_b);

    // This section initializes and generates matrix_output
    // int c = 0;
    // matrix_output = (double **) malloc(matrixSize * sizeof ( double *));
    // for (c = 0; c < matrixSize; c++)
    //     matrix_output[c] = (double *) malloc(matrixSize * sizeof ( double));



    int previous_end_element_index = -1;
    int t = 0;
    for (t = 1; t <= numworkers; t++) {

        dest = t  ;
        int start = previous_end_element_index + 1; // start index
        int end = start+(matrixSize * matrixSize) / numworkers - 1; // last index

        
        if (t == numworkers) // last thread will process all the elements till end
            end = matrixSize * matrixSize - 1;

        MPI_Send(&start, 1, MPI_INT , dest, FROM_MASTER, MPI_COMM_WORLD); 
        MPI_Send(&end, 1, MPI_INT, dest,FROM_MASTER , MPI_COMM_WORLD); 
        MPI_Send(&matrix_a, matrixSize*matrixSize, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
        MPI_Send(&matrix_b, matrixSize*matrixSize, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
        MPI_Send(&matrix_output, matrixSize*matrixSize, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
        

       // pthread_create(&worker_tid[t], NULL, multiplyMatrix, (void*) stripe);
        previous_end_element_index = end ;

    }

    //  wait for the threads to finish

    for (t = 1; t <= numworkers; t++) {
        int s , e  ;
        source = t ;
        MPI_Recv(&s, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
        MPI_Recv(&e, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
        printf("%d <=> %d <=> %d \n",t , s, e  );
        MPI_Recv(&matrix_output, matrixSize*matrixSize, MPI_DOUBLE, source, FROM_WORKER, MPI_COMM_WORLD, &status);
    }

    printf("\nMatrix Output \n");
    printMatrixToConsole(matrixSize,matrix_output);


     //printMatrixToFIle(matrix_a,"matrix_a.csv") ;
     //printMatrixToFIle(matrix_b,"matrix_b.csv") ;
     //printMatrixToFIle(matrix_output,"matrix_output.csv") ;


    timing_stop();
    printf("\n\nExec Timing:\n");
    print_timing();
  }
  else  /**************************** worker task ************************************/
  {
      int start_element_index ,last_element_index ;
      MPI_Recv(&start_element_index, 1, MPI_INT, MASTER, FROM_MASTER,MPI_COMM_WORLD, &status);
      MPI_Recv(&last_element_index, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_a, matrixSize*matrixSize, MPI_DOUBLE, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_b, matrixSize*matrixSize, MPI_DOUBLE, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_output, matrixSize*matrixSize, MPI_DOUBLE, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
      //printf("%d -  %d -  %d \n", rank, start_element_index, last_element_index);

       int p = 0, rowNum = 0, colNum = 0;
       for (p = start_element_index; p <= last_element_index; p++) {
        rowNum = p / matrixSize;
        colNum = p % matrixSize;

        //printf("     %d - %d -  %d - %d\n", rank ,rowNum, colNum , matrix_output[rowNum][colNum]);
       //  if (rowNum < matrixSize && colNum < matrixSize)
         double res = 0;
         int j = 0;

         for (j = 0; j < matrixSize; j++) {
           res = res + (matrix_a[rowNum][j] * matrix_b[j][colNum]);
         }

        matrix_output[rowNum][colNum] = res ;
       }

        MPI_Send(&rowNum, 1, MPI_INT , MASTER, FROM_WORKER, MPI_COMM_WORLD); 
        MPI_Send(&colNum, 1, MPI_INT, MASTER,FROM_WORKER , MPI_COMM_WORLD); 
        MPI_Send(&matrix_output, matrixSize*matrixSize, MPI_DOUBLE, MASTER, FROM_WORKER, MPI_COMM_WORLD);

  }

    MPI_Finalize() ;
    //return (EXIT_SUCCESS);
}

double getRand() {
    double d = (double) rand() / RAND_MAX;
    return min + d * (max - min);
}

void populateMatrix(int size , double matrix[][size]){
    int i = 0, j = 0;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            matrix[i][j] = getRand(min, max);

}

void printMatrixToConsole(int size , double matrix[][size]) {

    // print original matrix
    int i = 0, j = 0;
    printf("\n");
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

// double matrixDotProduct(int row, int column) {

//     double res = 0;
//     int j = 0;

//     for (j = 0; j < matrixSize; j++) {
//         res = res + (matrix_a[row][j] * matrix_b[j][column]);
//     }
//     return res;

// }

//void printMatrixToFIle(double **matrix, const char *filename) {
//    FILE *fp;
//    char comma[2];
//    int i, j;
//    fp = fopen(filename, "w");
//    srand(time(NULL));
//    fprintf(fp, "%d\n", matrixSize);
//    for (i = 0; i < matrixSize; i++) {
//        sprintf(comma, "%s", "");
//        for (j = 0; j < matrixSize; j++) {
//            fprintf(fp, "%s%f", comma, matrix[i][j]);
//            sprintf(comma, "%s", ",");
//            ;
//        }
//        fprintf(fp, "\n");
//    }
//
//    fclose(fp);
//}

