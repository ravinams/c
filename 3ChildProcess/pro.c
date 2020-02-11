
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
    char temp[] = "temp.txt";

    // Our find and replace arguments
    char *Find;
    char *Replace;


    //printf("fileName = %s\n", fileName);

    /* open input file in read mode */
    Input = fopen(fileName, "r");
    /* open temporary file in write mode */
    Output = fopen(temp, "w");

    char *header = "This is the updated version.\n";
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

int main(int argc, char** argv) {

    // declare the thread variable which are pointers to the actual threads
    pid_t t_child1, t_child2, t_child3;

    if (argc != 3) {
        printf("Incorrect usage of the program .\n");
        printf("You must supply two files as arguments to the program.");
        return (EXIT_FAILURE);
    }

    char *fileName1 = argv[1];
    char *fileName2 = argv[2];

    // printf("%s %s %s",argv[0],argv[1],argv[2]) ;
    printf("+++++Parent start+++++\n");

    t_child1 = fork(); // child-1 process
    if (t_child1 == 0) {

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

        exit(0);


    } else {
        t_child2 = fork(); // child-2 process
        if (t_child2 == 0) {
            char * ls_args[3] = {"./wc", fileName1, NULL};
            printf("\nHere is output from wc: \n");
            execvp(ls_args[0], ls_args);
            exit(0);
        } else {

            t_child3 = fork(); // child-3 process
            if (t_child3 == 0) {
                fileReplaceWords(fileName2);
                printf("\n%s has been updated\n", fileName2);
                exit(0);
            }

        }

    }

    int status = 0;
    pid_t pid;
    // try to wait for any children while there exists at least one
    while ((pid = waitpid(-1, &status, 0)) != -1) {
        printf("Child %d is done , the exit status is 0\n\n", pid, status);
    }

    // all child threads finished , so exit the parent thread

    printf("+++++Parent done+++++\n");

    return (EXIT_SUCCESS);
}

