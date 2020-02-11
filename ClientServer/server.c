#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

#define SIZE 255    
#define MAJOR_COUNT 512

struct majorPay {
    char *major;
    char *earlyCareerPay;
    char *midCareerPay;
};

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {

    int sockfd, portno;
    char input[256], output[256], fileName[256] ;

   

    struct sockaddr_in serv_addr;
    int n;

    struct majorPay majors[MAJOR_COUNT];
    int major_cnt = 0;
    
    FILE *fp = NULL;
    char line[SIZE] = {0}, *ptr = NULL;
    int cnt = 0;
    
    char *response = " -1 -1 ";
    int i = 0;
    

    printf("Enter file name : ");
    scanf("%s", fileName);
    //  START -- populate the table
    
     // Open file for Reading 
    if (NULL == (fp = fopen(fileName, "r"))) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof (line) + 1, fp) != NULL) {

        line[strlen(line) - 1] = '\0';

        majors[cnt].major = malloc(256);
        majors[cnt].earlyCareerPay = malloc(256);
        majors[cnt].midCareerPay = malloc(256);

        ptr = strtok(line, "\t");
        majors[cnt].major = malloc(strlen(ptr) + 1);
        strcpy(majors[cnt].major, ptr);

        ptr = strtok(NULL, "\t");
        majors[cnt].earlyCareerPay = malloc(strlen(ptr) + 1);
        strcpy(majors[cnt].earlyCareerPay, ptr);

        ptr = strtok(NULL, "\t");
        majors[cnt].midCareerPay = malloc(strlen(ptr) + 1);
        strcpy(majors[cnt].midCareerPay, ptr);


        cnt++;
    }
   
    //  END -- populate the table 
    
    major_cnt = cnt ;

    printf("Enter server port number: ");
    scanf("%d", &portno);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof (serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof (serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);

    while (true) { // multiple client connections

        printf("Waiting for incoming connections...\n");

        struct sockaddr_in cli_addr;
        int clilen = sizeof (cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0)
            error("ERROR on accept");

        printf("connection accepted ...\n");

        while (true) { // multiple client communications 
            
            printf("\nwaiting for new incoming messages  ...\n");
          
            bzero(input, 256);
            bzero(output, 256);

            n = read(newsockfd, input, 255);

            if (n < 0)
                error("ERROR reading from socket");

            else if (n == 0) {
                printf("Connection closed\n");
                //So I can now wait for another client
                break;
            }

            input[strlen(input) - 1] = '\0';
            
            // START - search table for Major
            
     response = " -1 -1 ";
     i = 0;

    for (i = 0; i < major_cnt; i++) {

        if (strcmp(majors[i].major, input) == 0) {

            char *early = NULL, *mid = NULL;
            early = majors[i].earlyCareerPay;
            mid = majors[i].midCareerPay;

            response = malloc(strlen(early) + strlen(mid) + 5);

            strcat(response, early);
            strcat(response, " ");
            strcat(response, mid);

            // printf("found ;  early=%s , mid=%s \n", early, mid);

            break;
        }

    }
            
            // END
            
            printf("\nHere is the message: %s", input);
            printf("\nHere is the response: %s \n", response);

            strcpy(output, response);
            n = write(newsockfd, output, 255);

            if (n < 0) error("ERROR writing to socket");
            
            fflush(stdout);
        }
        close(newsockfd);

    }

    close(sockfd);

    return 0;
}
