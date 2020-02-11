#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MAX 256

// This method will find & replace words in a  given file
int fileReplaceWords(char *fileName) {
    FILE *Input, *Output;
    const int BufferSize = 0x1000;
    char Buffer[BufferSize];
    char temp[] = "temp.txt" ;

    // Our find and replace arguments
    char *Find;
    char *Replace;

    
    //printf("fileName = %s\n", fileName);

    /* open input file in read mode */
    Input = fopen(fileName, "r");
    /* open temporary file in write mode */
    Output = fopen(temp, "w");

     char *header ="This is the updated version.\n" ;     
     fwrite(header, 1, strlen(header), Output);
     
    // For each line...
    while (NULL != fgets(Buffer, BufferSize, Input)) {
        // For each incidence of "is"
        char *Stop = NULL; // Where to stop copying (at 'is')
        char *Start = Buffer; // Start at the beginning of the line, and after each match
                      
        //  printf("Line: %s\n", Buffer);
        Find = "run";
        Replace = "execute";
        while (1) {
            // Find next match
            Stop = strstr(Start, Find);

            if (NULL == Stop) {
                // Print the remaining text in the line
                fwrite(Start, 1, strlen(Start), Output);
                break;
            }

            // We have found a match!  Copy everything from [Start, Stop)
            fwrite(Start, 1, Stop - Start, Output);

            // Write our replacement text
            fwrite(Replace, 1, strlen(Replace), Output);

            // Next time, we want to start searching after our 'match'        
            Start = Stop + strlen(Find);

        };
    

    }

    // Close our files
    fclose(Input);
    fclose(Output);
    rename(temp, fileName);  
    
   /// ---
    
    /* open input file in read mode */
    Input = fopen(fileName, "r");
    /* open temporary file in write mode */
    Output = fopen(temp, "w");
    
    // For each line...
    while (NULL != fgets(Buffer, BufferSize, Input)) {
        // For each incidence of "is"
        char *Stop = NULL; // Where to stop copying (at 'is')
        char *Start = Buffer; // Start at the beginning of the line, and after each match
                      
        //  printf("Line: %s\n", Buffer);
        Find = "examine";
        Replace = "study";
        while (1) {
            // Find next match
            Stop = strstr(Start, Find);

            if (NULL == Stop) {
                // Print the remaining text in the line
                fwrite(Start, 1, strlen(Start), Output);
                break;
            }

            // We have found a match!  Copy everything from [Start, Stop)
            fwrite(Start, 1, Stop - Start, Output);

            // Write our replacement text
            fwrite(Replace, 1, strlen(Replace), Output);

            // Next time, we want to start searching after our 'match'        
            Start = Stop + strlen(Find);

        };
    

    }

    // Close our files
    fclose(Input);
    fclose(Output);
    rename(temp, fileName); 
    
    //
    
  
    return 0;
}

// this method will be invoked by the child thread 1

void *child1(void *arg) {
    int arr[20], cnt = 0, totalMarks = 0;
    printf("Please enter 20 marks :\n");

    char temp;
    do {
        scanf("%d%c", &arr[cnt], &temp);
        cnt++;
    } while (temp != '\n' || cnt < 20);

    int i;
    for (i = 0; i < 20; i++) {
        printf("(%d)", arr[i]);
        totalMarks = totalMarks + arr[i];
    }

    //  printf(" totalMarks : %d\n",totalMarks) ;
    printf(" Average : %f\n", totalMarks / 20.0);
    printf("child-1 %d done, the exit status is 0\n", pthread_self());
    pthread_exit((void*) 0);
}

// this method will be invoked by the child thread 2

void *child2(void *arg) {
   

     char *fileName1 = (char*) arg;

     char * ls_args[3] = { "./wc", fileName1, NULL} ;
     
    // printf("fileName1 = %s\n",fileName1) ;

   

    printf("Here is output from wc: ");
    
  pid_t c_pid, pid;
  int status;

  c_pid = fork();

  if (c_pid == 0){
      
       execvp( ls_args[0], ls_args);
  }
  
   
    printf("\nchild-2 %d done, the exit status is 0\n", pthread_self());
    pthread_exit((void*) 0);
}

// this method will be invoked by the child thread 3

void *child3(void *arg) {

    char *fileName2 = (char*) arg;

   
    fileReplaceWords(fileName2);
    printf("\n%s has been updated\n",fileName2) ;
    printf("child-3 %d done, the exit status is 0\n", pthread_self());
    pthread_exit((void*) 0);
}

int main(int argc, char** argv) {

    // declare the thread variable which are pointers to the actual threads
    pthread_t t_child1, t_child2, t_child3;
  
    if (argc !=3 )
    {
        printf("Incorrect usage of the program .\n") ;
        printf("You must supply two files as arguments to the program.") ;
        return (EXIT_FAILURE);
    }

    // printf("%s %s %s",argv[0],argv[1],argv[2]) ;
    printf("parent start\n");

    // create the child threads and invoke the thread functions

    pthread_create(&t_child1, NULL, child1, NULL);
    pthread_create(&t_child2, NULL, child2, argv[1]);
    pthread_create(&t_child3, NULL, child3, argv[2]);

    // please wait till the child threads finishes 

    pthread_join(t_child1, NULL);
    pthread_join(t_child2, NULL);
    pthread_join(t_child3, NULL);


    // all child threads finished , so exit the parent thread
    printf("parent done\n");

    return (EXIT_SUCCESS);
}

