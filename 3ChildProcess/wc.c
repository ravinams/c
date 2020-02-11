#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {

    FILE * filp;
    int count = 1;
    char c;
    filp = fopen(argv[1], "r");
    if(filp == NULL)
        printf("file not found\n");
    while((c = fgetc(filp)) != EOF) {
        if(c == ' ')
            count++;
    }
    printf("%d\n", count);
    return 0;
}