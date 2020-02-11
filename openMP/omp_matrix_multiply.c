#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//#include "timing.h"
//#include "timing.c"

const double min = 1;
const double max = 100;

double getRand();
void populateMatrix(double **matrix,int mSize);
void printMatrixToFIle(double **matrix, const char *filename,int mSize);
void printMatrixToConsole(double **matrix,int mSize);
void *multiplyMatrix(void *arg);
double matrixDotProduct(int row, int column,int mSize);

int main(int argc, char** argv) {


    if (argc < 3) {
        perror("\n Usage ./HomeWork2_SerialPrallelmatrix.o <squareMatrixSize> <threads> \n");
        exit(-1);
    }

    printf("\n\n");

    int matrixSize, noOfThreads;

    double **matrix_a, **matrix_b , **matrix_output;


    matrixSize = atoi(argv[1]);
    noOfThreads = atoi(argv[2]);
    //timing_start();

    printf("\nSize of the matrix (%d * %d)= %d", matrixSize, matrixSize, matrixSize * matrixSize);
    printf("\nNumber of worker threads = %d\n", noOfThreads);

    // The below section initialize and generate matrix_a
    int a = 0;
    matrix_a = (double **) malloc(matrixSize * sizeof (double *));
    for (a = 0; a < matrixSize; a++)
        matrix_a[a] = (double *) malloc(matrixSize * sizeof (double));

    populateMatrix(matrix_a,matrixSize);
    printf("\nMatrix A, .csv file has been generated as well. \n");
    printMatrixToConsole(matrix_a,matrixSize);

    // The below section initialize and generate matrix_b
    int b = 0;
    matrix_b = (double **) malloc(matrixSize * sizeof (double *));
    for (b = 0; b < matrixSize; b++)
        matrix_b[b] = (double *) malloc(matrixSize * sizeof (double));

    populateMatrix(matrix_b,matrixSize);
    printf("\nMatrix B .csv file has been generated as well.\n");
    printMatrixToConsole(matrix_b,matrixSize);

    // The below section initialize and generate matrix_output
    int c = 0;
    matrix_output = (double **) malloc(matrixSize * sizeof ( double *));
    for (c = 0; c < matrixSize; c++)
        matrix_output[c] = (double *) malloc(matrixSize * sizeof ( double));




  int i,j,k;  
  double wtime;
  
  
  wtime = omp_get_wtime();
  omp_set_num_threads(noOfThreads);
  # pragma omp parallel shared ( matrix_a, matrix_b, matrix_output, matrixSize) private ( i, j, k )
  {

  # pragma omp for
  for ( i = 0; i < matrixSize; i++ )
  {
    for ( j = 0; j < matrixSize; j++ )
    {
      matrix_output[i][j] = 0.0;
      for ( k = 0; k < matrixSize; k++ )
      {
        matrix_output[i][j] = matrix_output[i][j] + matrix_a[i][k] * matrix_b[k][j];
      }
    }    
  }

  }

  wtime = omp_get_wtime()-wtime;
 

    printf("\nMatrix Output, .csv has been generated as well. \n");
    printMatrixToConsole(matrix_output,matrixSize);


     printMatrixToFIle(matrix_a,"matrix_a.csv",matrixSize) ;
     printMatrixToFIle(matrix_b,"matrix_b.csv",matrixSize) ;
     printMatrixToFIle(matrix_output,"matrix_output.csv",matrixSize) ;

    printf ("\n\nElapsed seconds to multiply matrix = %g\n", wtime );

    //timing_stop();
    //printf("\n\nExec Timing:\n");
    //print_timing();
    return (EXIT_SUCCESS);
}

double getRand() {
    double d = (double) rand() / RAND_MAX;
    return min + d * (max - min);
}

void populateMatrix(double **matrix, int matrixSize) {
    int i = 0, j = 0;
    for (i = 0; i < matrixSize; i++)
        for (j = 0; j < matrixSize; j++)
            matrix[i][j] = getRand(min, max);

}

void printMatrixToConsole(double **matrix, int matrixSize ) {

    // print original matrix
    int i = 0, j = 0;
    printf("\n");
    for (i = 0; i < matrixSize; i++) {
        for (j = 0; j < matrixSize; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

void printMatrixToFIle(double **matrix, const char *filename, int matrixSize) {
    FILE *fp;
    char comma[2];
    int i, j;
    fp = fopen(filename, "w");
    srand(time(NULL));
    fprintf(fp, "%d\n", matrixSize);
    for (i = 0; i < matrixSize; i++) {
        sprintf(comma, "%s", "");
        for (j = 0; j < matrixSize; j++) {
            fprintf(fp, "%s%f", comma, matrix[i][j]);
            sprintf(comma, "%s", ",");
            ;
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

