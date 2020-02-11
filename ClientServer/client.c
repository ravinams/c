// Please note this is a C program
// It compiles without warnings with gcc
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256], hostName[256], major[256];
    char *notFound = " -1 -1 ", *response = NULL;

    char *early = NULL, *mid = NULL, *ptr = NULL;

    printf("Enter server host name: ");
    scanf("%s", hostName);

    server = gethostbyname(hostName);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    printf("Enter server port number: ");
    scanf("%d", &portno);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
            (char *) &serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
        error("ERROR connecting");

    fgets(major, sizeof major, stdin); // this is dummy line .. DONT delete it .

    while (true) {

        bzero(major, 256);
        printf("\nEnter a college major : ");
        fgets(major, sizeof major, stdin);

        if (strlen(major) == 1) // if empty string , then terminate program 
        {
            error("ERROR .. empty major .. EXITING program..");
        }

        n = write(sockfd, major, strlen(major));

        if (n < 0)
            error("ERROR writing to socket");
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");

        // printf("socket reponse %s\n", buffer);
        
        response = (char *) malloc(strlen(buffer) + 1);
        strcpy(response, buffer);

        if (strcmp(response, notFound) == 0) {
            printf("\nThat major is not in the table");
        } else {
           // printf("\nThat corresponding pay is %s", response);

            ptr = strtok(response, " ");
            early = malloc(strlen(ptr) + 1);
            strcpy(early, ptr);

            ptr = strtok(NULL, " ");
            mid = malloc(strlen(ptr) + 1);
            strcpy(mid, ptr);

            printf("\nThe average early-career pay for a %s major is $%s", major,early);
            printf("\nThe corresponding mid-career pay is $%s", mid);
        }
       printf("\n");
    }

    close(sockfd);
    return 0;
}
