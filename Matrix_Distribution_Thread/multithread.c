#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


// global variables can be accessed from child threads
int N, P;
int MatrixDistribution[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int **matrix;

pthread_t worker_tid[64];
pthread_mutex_t mtx;

void accumulateDistribution(int ele) {
    switch (ele) {
        case 1: MatrixDistribution[0]++;
            break;
        case 2: MatrixDistribution[1]++;
            break;
        case 3: MatrixDistribution[2]++;
            break;
        case 4: MatrixDistribution[3]++;
            break;
        case 5: MatrixDistribution[4]++;
            break;
        case 6: MatrixDistribution[5]++;
            break;
        case 7: MatrixDistribution[6]++;
            break;
        case 8: MatrixDistribution[7]++;
            break;
        case 9: MatrixDistribution[8]++;
            break;
        case 10: MatrixDistribution[9]++;
            break;

    }
}

void *calculateDistribution(void *arg) {

    int *stripe = (int*) arg;
    // pthread_self()
    
    
   // printf("\nThread inputs : %d , %d ,%d",pthread_self(),stripe[0],stripe[1]);

    int arrayX  = stripe[0], arrayY =stripe[1] ;
    
            
    int arrayXstart = 0,arrayXend = 0,arrayYstart = 0,arrayYend = 0 ;
    
    int totalElements = stripe[1] - stripe[0] + 1 ;    
    int processedElements = 0 ;
     
    arrayXstart = ( arrayX ) / N  ;
    arrayXend   = ( arrayX + totalElements ) / N  ;
    
    arrayYstart =  arrayX % N  ;
    arrayYend   =  ( arrayYstart + totalElements -1 ) > N ? N : totalElements -1 ;
    
    pthread_mutex_lock(&mtx);
//
//    for (int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//
//            accumulateDistribution(matrix[i][j]);
//
//        }
//    }

   
    for (int i = arrayXstart; i <= arrayXend; i++) {
        
        for (int j = arrayYstart; j <=arrayYend ; j++) {
            accumulateDistribution(matrix[i][j]);
            processedElements ++ ;
        }
        
        arrayYstart = 0 ;
        arrayYend   =  ( totalElements - processedElements  ) > N ? N : totalElements - processedElements ;
        
    }
    
    
    
    pthread_mutex_unlock(&mtx);
    
    pthread_exit(NULL); 
    
    return NULL; 

}

int main(int argc, char** argv) {

    printf("\n ----- Welcome to matrix distribution computation multi-threaded program. --------\n");

    if (argc < 3) {
        printf("\n You must enter N, P values as command line arguments");
        return EXIT_FAILURE;
    }


    N = atoi(argv[1]); // this will convert char to int
    P = atoi(argv[2]); // this will convert char to int

    printf("\nSize of the matrix (%d * %d)= %d", N, N, N * N);
    printf("\nNumber of worker threads = %d", P);

    matrix = (int **) malloc(N * sizeof (int *));
    for (int i = 0; i < N; i++)
        matrix[i] = (int *) malloc(N * sizeof (int));

    // Initialize matrix with numbers from 1-10 inclusive
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10 + 1; // rand() generates random numbers
        }
    }
    // print original matrix
    printf("\n\nMATRIX\n\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }



    int previous_end_element_index = 0 ;
    for (int i = 0; i < P; i++) {
        
        int *stripe = (int*) malloc(2 * sizeof (int));
        stripe[0] = previous_end_element_index+1;    // start index
        stripe[1] = stripe[0]+(N*N)/P-1; // last index 

        if (i == P-1)  // last thread will process all the elements till end
             stripe[1] = N*N ;
            
        pthread_create(&worker_tid[i], NULL, calculateDistribution, (void*) stripe);
        previous_end_element_index = stripe[1] ;
    }

    //  wait for the threads to finish 
    for (int k = 0; k < P; k++) {
        pthread_join(worker_tid[k], NULL);
    }

    printf("\nMatrix Distribution Of Values \n\n");
    for (int i = 0; i < 10; i++) {
        printf("Number of %d's : %d\n", i + 1, MatrixDistribution[i]);
    }


    return (EXIT_SUCCESS);
}

