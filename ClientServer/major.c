#include <stdio.h>                                                              
#include <stdlib.h>                                                             
#include <string.h>                                                             
#define SIZE 255    
#define MAJOR_COUNT 512

struct collegeMajor {
    char *name;
    char *earlyCareer;
    char *midCareer;
};

int populateTable(char *fileName, struct collegeMajor *majors) {
    FILE *fp = NULL;
    char line[SIZE] = {0}, *ptr = NULL;
    int cnt = 0;

    // Open file for Reading 
    if (NULL == (fp = fopen(fileName, "r"))) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }


    while (fgets(line, sizeof (line) + 1, fp) != NULL) {

        line[strlen(line) - 1] = '\0';

        majors[cnt].name = malloc(256);
        majors[cnt].earlyCareer = malloc(256);
        majors[cnt].midCareer = malloc(256);

        /* 1. Read each line from the file */
        // printf("\n Read line:  %s\n", line);

        /* 2. Tokenise the read line, using "\" delimiter*/
        ptr = strtok(line, "\t");
        majors[cnt].name = malloc(strlen(ptr) + 1);
        strcpy(majors[cnt].name, ptr);

        ptr = strtok(NULL, "\t");
        majors[cnt].earlyCareer = malloc(strlen(ptr) + 1);
        strcpy(majors[cnt].earlyCareer, ptr);

        ptr = strtok(NULL, "\t");
        majors[cnt].midCareer = malloc(strlen(ptr) + 1);
        strcpy(majors[cnt].midCareer, ptr);

       // printf("(%d) After: major:[%s] early-career:[%s] mid-career:[%s]\n", cnt, majors[cnt].name, majors[cnt].earlyCareer, majors[cnt].midCareer);

        cnt++;
    }

    return cnt;
}

char * searchMajorInTable(char *searchMajor, struct collegeMajor *majors, int majorCnt) {

    char *response = " -1 -1 ";
    int i = 0;

    for (i = 0; i < majorCnt; i++) {
        if (strcmp(majors[i].name, searchMajor) == 0) {

            char *early = NULL, *mid = NULL;
            early = majors[i].earlyCareer;
            mid = majors[i].midCareer;

            response = malloc(strlen(early) + strlen(mid) + 5);

            strcat(response, early);
            strcat(response, " ");
            strcat(response, mid);

            // printf("found ;  early=%s , mid=%s \n", early, mid);

            break;
        }

    }

    // printf("response = %s", response);
    return response;
}

int main() {

    struct collegeMajor majors[MAJOR_COUNT];
    char *fileName = "salaries.txt";
    char *major = "Naval Architecture & Marine Engineering";
    char * response;
    int major_cnt = 0;


    major_cnt = populateTable(fileName, majors);
    
     printf("major_cnt = %d", major_cnt);
     
     
    response = searchMajorInTable(major, majors, major_cnt);

    printf(" ---   \n ");

    printf("response = %s", response);


    //    for (cnt = 0; cnt < major_cnt; cnt++) {
    //        printf("(%d) major = [%s] - early-career = [%s] - mid-career=[%s]\n", cnt, subject[cnt].name, subject[cnt].earlyCareer, subject[cnt].midCareer);
    //    }
    //    

    return 0;
}

