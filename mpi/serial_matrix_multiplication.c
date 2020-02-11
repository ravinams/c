#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timing.h"
#include "timing.c"

int matrixSize, noOfThreads;

double **matrix_a, **matrix_b;
double **matrix_output;

const double min = 1;
const double max = 100;

double getRand();
void populateMatrix(double **matrix);
//void printMatrixToFIle(double **matrix, const char *filename);
void printMatrixToConsole(double **matrix);
void *multiplyMatrix(void *arg);
double matrixDotProduct(int row, int column);

pthread_t worker_tid[64];
pthread_mutex_t mtx;

int main(int argc, char** argv) {


    if (argc < 3) {
        perror("\n Usage ./HomeWork2_SerialPrallelmatrix.o <squareMatrixSize> <threads> \n");
        exit(-1);
    }


    printf("\n ----- Welcome to Square Matrix Multiplication using threads. --------\n\n");

    matrixSize = atoi(argv[1]);
    noOfThreads = atoi(argv[2]);

    timing_start();

    printf("\nSize of the matrix (%d * %d)= %d", matrixSize, matrixSize, matrixSize * matrixSize);
    printf("\nNumber of worker threads = %d\n", noOfThreads);

    // The below section initialize and generate matrix_a
    int a = 0;
    matrix_a = (double **) malloc(matrixSize * sizeof (double *));
    for (a = 0; a < matrixSize; a++)
        matrix_a[a] = (double *) malloc(matrixSize * sizeof (double));

    populateMatrix(matrix_a);
    printf("\nMatrix A \n");
    printMatrixToConsole(matrix_a);

    // This section initializes and generates matrix_b
    int b = 0;
    matrix_b = (double **) malloc(matrixSize * sizeof (double *));
    for (b = 0; b < matrixSize; b++)
        matrix_b[b] = (double *) malloc(matrixSize * sizeof (double));

    populateMatrix(matrix_b);
    printf("\nMatrix B \n");
    printMatrixToConsole(matrix_b);

    // This section initializes and generates matrix_output
    int c = 0;
    matrix_output = (double **) malloc(matrixSize * sizeof ( double *));
    for (c = 0; c < matrixSize; c++)
        matrix_output[c] = (double *) malloc(matrixSize * sizeof ( double));



    int previous_end_element_index = -1;
    int t = 0;
    for (t = 0; t < noOfThreads; t++) {

        int *stripe = (int*) malloc(2 * sizeof (int));
        stripe[0] = previous_end_element_index + 1; // start index
        stripe[1] = stripe[0]+(matrixSize * matrixSize) / noOfThreads - 1; // last index

        if (t == noOfThreads - 1) // last thread will process all the elements till end
            stripe[1] = matrixSize * matrixSize - 1;



        pthread_create(&worker_tid[t], NULL, multiplyMatrix, (void*) stripe);
        previous_end_element_index = stripe[1];
    }

    //  wait for the threads to finish

    for (t = 0; t < noOfThreads; t++) {
        pthread_join(worker_tid[t], NULL);
    }

    printf("\nMatrix Output \n");
    printMatrixToConsole(matrix_output);


     //printMatrixToFIle(matrix_a,"matrix_a.csv") ;
     //printMatrixToFIle(matrix_b,"matrix_b.csv") ;
     //printMatrixToFIle(matrix_output,"matrix_output.csv") ;


    timing_stop();
    printf("\n\nExec Timing:\n");
    print_timing();
    return (EXIT_SUCCESS);
}

double getRand() {
    double d = (double) rand() / RAND_MAX;
    return min + d * (max - min);
}

void populateMatrix(double **matrix) {
    int i = 0, j = 0;
    for (i = 0; i < matrixSize; i++)
        for (j = 0; j < matrixSize; j++)
            matrix[i][j] = getRand(min, max);

}

void printMatrixToConsole(double **matrix) {

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

void *multiplyMatrix(void *arg) {
    int *stripe = (int*) arg;
    // pthread_self()


    int start_element_index = stripe[0], last_element_index = stripe[1];

    pthread_mutex_lock(&mtx);

    int p = 0, rowNum = 0, colNum = 0;
    for (p = start_element_index; p <= last_element_index; p++) {
        rowNum = p / matrixSize;
        colNum = p % matrixSize;
        matrix_output[rowNum][colNum] = matrixDotProduct(rowNum, colNum);
    }
    pthread_mutex_unlock(&mtx);

    pthread_exit(NULL);

    return NULL;

}

double matrixDotProduct(int row, int column) {

    double res = 0;
    int j = 0;

    for (j = 0; j < matrixSize; j++) {
        res = res + (matrix_a[row][j] * matrix_b[j][column]);
    }
    return res;

}

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

