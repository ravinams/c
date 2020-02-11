#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#include "timing.h"
#include "timing.c"


int M , N , I  ;
void printGeneration(int M, int N,int grid[M][N]) ;
void defaultGeneration(int M, int N,int grid[M][N]) ;
int getAliveNeighborCount(int row , int col , int M , int N , int currentGrid[M][N]);
int computeNextGeneration(int row, int col, int M , int N , int futureGrid[M][N]) ;

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
        perror("\n Usage Invalid usage. ./mpi_game_parallel <M> <N> <I> \n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(-1);
    }
    
        M = atoi(argv[1]); // Height size of the matrix
        N = atoi(argv[2]); // Wieght size of the matrix
        I = atoi(argv[3]); // I number of iterations .
    

       int numworkers = numtasks-1 ;   
       int chunkSize  = (M*N)/numworkers ;

       int grid[M][N] ;
    
    /**************************** master task ************************************/
    if (rank == MASTER) 
    {                 
          printf(" ****************************************************\n");
          printf("   Game of Life simulation MPI   \n");
          printf(" ****************************************************\n");

          timing_start();
        
          printf("\nSize of the matrix(%d,%d), no of iterations = %d ", M, N, I);
          printf("\nNumber of worker processes = %d\n", numworkers+1);

           int grid[M][N] ;
 
           defaultGeneration(M,N,grid);
           printf("Original Generation.\n");
           printGeneration(M,N,grid);

    int previous_end_element_index = -1;
    int t = 0;
    for (t = 1; t <= numworkers; t++) {

        dest = t  ;
        int start = previous_end_element_index + 1; // start index
        int end = start+chunkSize - 1; // last index
        
        if (t == numworkers) // last thread will process all the elements till end
            end = M * N - 1;

        MPI_Send(&start, 1, MPI_INT , dest, FROM_MASTER, MPI_COMM_WORLD); 
        MPI_Send(&end, 1, MPI_INT, dest,FROM_MASTER , MPI_COMM_WORLD); 
        MPI_Send(&grid, M*N, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
      
            
        previous_end_element_index = end ;

    }

    //  wait for the threads to finish

    for (t = 1; t <= numworkers; t++) {
        int s , e  ;
        source = t ;
        MPI_Recv(&s, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
        MPI_Recv(&e, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
      
        int resultArray[e-s+1];
        MPI_Recv(&resultArray, e-s+1, MPI_DOUBLE, source, FROM_WORKER, MPI_COMM_WORLD, &status);
        int idx = 0 ;        
        for (idx = s; idx <= e ; idx++) {
              int rowNum = (idx) / ( M );
              int colNum = (idx) % ( N ) ;            
              grid[rowNum][colNum] = resultArray[idx-s] ;            
        }      
    }

    printf("Next Generation.\n");
    printGeneration(M,N,grid);

    timing_stop();
    printf("\n\nExec Timing:\n");
    print_timing();
  }
  else  /**************************** worker task ************************************/
  {
      int start_element_index ,last_element_index ;
      MPI_Recv(&start_element_index, 1, MPI_INT, MASTER, FROM_MASTER,MPI_COMM_WORLD, &status);
      MPI_Recv(&last_element_index, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
      MPI_Recv(&grid, M*N, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
     
      
      //printf("%d -  %d -  %d \n", rank, start_element_index, last_element_index);

       int p = 0, rowNum = 0, colNum = 0 , resultArray[last_element_index-start_element_index+1];
       for (p = start_element_index; p <= last_element_index; p++) {
        rowNum = p / ( M ) ;
        colNum = p % ( N );
        
        resultArray[p-start_element_index]=computeNextGeneration(rowNum,colNum,M,N,grid)  ;
       }

        MPI_Send(&start_element_index, 1, MPI_INT , MASTER, FROM_WORKER, MPI_COMM_WORLD); 
        MPI_Send(&last_element_index, 1, MPI_INT, MASTER,FROM_WORKER , MPI_COMM_WORLD); 
        MPI_Send(&resultArray, last_element_index-start_element_index+1, MPI_DOUBLE, MASTER, FROM_WORKER, MPI_COMM_WORLD);

  }

    MPI_Finalize() ;
    return (EXIT_SUCCESS);
}

void defaultGeneration(int M, int N,int grid[M][N]) 
{
   int i , j;
  
  srand(time(NULL));
   
  

    for ( i = 0; i < M; i++) 
        { 
            for ( j = 0; j < N; j++) 
            { 
                grid[i][j] = rand()%2;
               
            } 
           
        }   
}

void printGeneration(int M, int N,int grid[M][N]) 
{
    int i , j;

   for ( i = 0; i < M; i++) 
        { 
            for ( j = 0; j < N; j++) 
            {                
                printf("%d ",grid[i][j]) ;
            } 
            printf("\n");
        } 
        printf("\n");
}

int getAliveNeighborCount(int row , int col , int M , int N , int currentGrid[M][N]) 
{
    int aliveLiveCnt = 0;
    
                // this section calculate neighbors for cells on the grid boundaries
            if (row == 0 || row == M - 1 || col == 0 || col == N - 1) {
              
                    if ((row == 0) && (col == 0)) //top left corner
                    {
                            // adjacent  cells 
                            if (currentGrid[row][col+1] == 1) // same row , next column
                                 aliveLiveCnt++;
                            if (currentGrid[row+1][col+1] == 1) // diagonal
                                aliveLiveCnt++;
                            if (currentGrid[row+1][col] == 1) // next row , same column
                                aliveLiveCnt++;
                           
                            // horizontally wrapped
                             if (currentGrid[M -1][col] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[M -1][col+1] == 1)
                                aliveLiveCnt++;
                            
                             // vertically wrapped
                            
                            if (currentGrid[row][N-1] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[row+1][N-1] == 1)
                                aliveLiveCnt++;
                            
                            // diagonally wrapped
                              
                             if (currentGrid[M -1][N-1] == 1)
                                 aliveLiveCnt++; 
                      
                    }
                    if ((row == 0) && ((col > 0) && (col < N - 1)))// top middle section
                    {
                            // same row
                            if (currentGrid[row][col-1] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[row][col+1] == 1)
                                 aliveLiveCnt++;
                            
                            //next row
                            if (currentGrid[row+1][col-1] == 1)
                                aliveLiveCnt++;
                            if (currentGrid[row+1][col] == 1)
                                aliveLiveCnt++;
                             if (currentGrid[row+1][col+1] == 1)
                                aliveLiveCnt++;
                            
                              // horizontally wrapped
                             if (currentGrid[M -1][col-1] == 1)
                                 aliveLiveCnt++;
                             if (currentGrid[M -1][col] == 1)
                                 aliveLiveCnt++;
                             if (currentGrid[M -1][col+1] == 1)
                                 aliveLiveCnt++;
                    }

                    if ((row == 0) && (col == N - 1)) // top right corner
                    {

                              // adjucent  cells 
                            if (currentGrid[row][col-1] == 1) // same row , previous column
                                 aliveLiveCnt++;
                            if (currentGrid[row+1][col-1] == 1) // diagonal
                                aliveLiveCnt++;
                            if (currentGrid[row+1][col] == 1) // next row , same column
                                aliveLiveCnt++;
                           
                            // horizontally wrapped
                             if (currentGrid[M -1][col] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[M -1][col-1] == 1)
                                aliveLiveCnt++;
                            
                             // vertically wrapped
                            
                            if (currentGrid[row][0] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[row+1][0] == 1)
                                aliveLiveCnt++;
                            
                            // diagonal wrapped
                              
                             if (currentGrid[0][N-1] == 1)
                                aliveLiveCnt++;
                    }

                    if (((row > 0) && (row < M - 1)) && (col == N - 1)) // right side middle
                    {
                             // same column
                            if (currentGrid[row-1][N - 1] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[row+1][N - 1] == 1)
                                 aliveLiveCnt++;
                            
                            //previuos column 
                            if (currentGrid[row-1][N - 2] == 1)
                                aliveLiveCnt++;
                            if (currentGrid[row][N - 2] == 1)
                                aliveLiveCnt++;
                             if (currentGrid[row+1][N - 2] == 1)
                                aliveLiveCnt++;
                            
                              // vertical wrapped
                             if (currentGrid[row-1][0] == 1)
                                 aliveLiveCnt++;
                             if (currentGrid[row][0] == 1)
                                 aliveLiveCnt++;
                             if (currentGrid[row +1][0] == 1)
                                 aliveLiveCnt++;
                    }

                    if ((row == M - 1) && (col == N - 1)) // bottom right corner
                    {
                             // adjucent  cells 
                            if (currentGrid[row][col-1] == 1) // same row , previous column
                                 aliveLiveCnt++;
                            if (currentGrid[row-1][col-1] == 1) // diagonal
                                aliveLiveCnt++;
                            if (currentGrid[row-1][col] == 1) // previous row , same column
                                aliveLiveCnt++;
                           
                            // horizontally wrapped  -- 0 row 
                             if (currentGrid[0][col] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[0][col-1] == 1)
                                aliveLiveCnt++;
                            
                             // vertically wrapped -- last i.e M-1 row
                            
                            if (currentGrid[row][col] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[row][col-1] == 1)
                                aliveLiveCnt++;
                            
                            // diagonal wrapped
                              
                             if (currentGrid[0][0] == 1)
                                aliveLiveCnt++;
                    }

                    if ((row == M - 1) && ((col < N - 1) && (col > 0))) // bottom portion
                    {
                              // same row
                            if (currentGrid[row][col-1] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[row][col+1] == 1)
                                 aliveLiveCnt++;
                            
                            //previous row
                            if (currentGrid[row-1][col-1] == 1)
                                aliveLiveCnt++;
                            if (currentGrid[row-1][col] == 1)
                                aliveLiveCnt++;
                             if (currentGrid[row-1][col+1] == 1)
                                aliveLiveCnt++;
                            
                              // horizontally wrapped --first i.e 0 row 
                             if (currentGrid[0][col-1] == 1)
                                 aliveLiveCnt++;
                             if (currentGrid[0][col] == 1)
                                 aliveLiveCnt++;
                             if (currentGrid[0][col+1] == 1)
                                 aliveLiveCnt++;
                    }
                    if ((row == M - 1) && (col == 0)) // bottom left corner
                    {
                             // adjacent  cells 
                            if (currentGrid[row][col+1] == 1) // same row , next column
                                 aliveLiveCnt++;
                            if (currentGrid[row-1][col-1] == 1) // diagonal
                                aliveLiveCnt++;
                            if (currentGrid[row-1][col+1] == 1) // previous row , same column
                                aliveLiveCnt++;
                           
                            // horizontally wrapped
                             if (currentGrid[0][col] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[0][col+1] == 1)
                                aliveLiveCnt++;
                            
                             // vertically wrapped
                            
                            if (currentGrid[row][N-1] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[row-1][N-1] == 1)
                                aliveLiveCnt++;
                            
                            // diagonally wrapped
                              
                             if (currentGrid[0][N-1] == 1)
                                aliveLiveCnt++;
                    }

                    if (((row < M - 1) && (row > 0)) && (col == 0)) // left side middle
                    {
                              // same column
                            if (currentGrid[row-1][col] == 1)
                                 aliveLiveCnt++;
                            if (currentGrid[row+1][col] == 1)
                                 aliveLiveCnt++;
                            
                            //next column 
                            if (currentGrid[row-1][col+1] == 1)
                                aliveLiveCnt++;
                            if (currentGrid[row][col+1] == 1)
                                aliveLiveCnt++;
                             if (currentGrid[row+1][col+1] == 1)
                                aliveLiveCnt++;
                            
                              // vertical wrapped -- last column
                             if (currentGrid[row-1][N-1] == 1)
                                 aliveLiveCnt++;
                             if (currentGrid[row][N-1] == 1)
                                 aliveLiveCnt++;
                             if (currentGrid[row +1][N-1] == 1)
                                 aliveLiveCnt++;
                        }

            }  // boundary cells section end here 
              // this section calculate neighbors for cells inside the grid boundaries
            else if ((row > 0 && row < M - 1) && (col > 0 || col < M - 1)) {
              

                if (currentGrid[row - 1][col - 1] == 1) {
                    aliveLiveCnt++;
                }

                if (currentGrid[row - 1][col] == 1) {
                    aliveLiveCnt++;
                }
                if (currentGrid[row - 1][col + 1] == 1) {
                    aliveLiveCnt++;
                }

                if (currentGrid[row][col - 1] == 1) {
                    aliveLiveCnt++;
                }

                if (currentGrid[row][col + 1] == 1) {
                    aliveLiveCnt++;
                }

                if (currentGrid[row + 1][col - 1] == 1) {
                    aliveLiveCnt++;
                }

                if (currentGrid[row + 1][col + 1] == 1) {
                    aliveLiveCnt++;
                }
            }  // non boundary section end here 

     
    return aliveLiveCnt ;
}

int computeNextGeneration(int row, int col, int M , int N , int currentGrid[M][N]) 
{
  int aliveLiveCnt = 0;
  int value = 0 ;

              aliveLiveCnt = getAliveNeighborCount(row,col,M,N,currentGrid) ;
            // the below section populates new world as per rules .
            
            value = '0'; // initially fill it with dash 

            if (currentGrid[row][col] == 1 && aliveLiveCnt < 1) //rule 1
            {
                value = 0;
            } 
            else if (currentGrid[row][col] == 1 && (aliveLiveCnt == 2))//rule 2
            {
                value = 1;
            } else if ((currentGrid[row][col] == 1 || currentGrid[row][col] == 0) && (aliveLiveCnt == 3)) //rule 3
            {
                value = 1;

            } else if (currentGrid[row][col] == 1 && aliveLiveCnt >= 4) //rule 4
            {
                value = 0;
            } else {
                value = currentGrid[row][col];
            }

            return value ;
}
