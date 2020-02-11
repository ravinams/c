#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int convertIntegralToBinary(int);
float convertFractionToBinary(float);
int findDigitsCount(int);

int main(int argc, char** argv) {

    FILE *fptr;

    if (argc < 2) {
        printf("Please supply the file name as argument \n");
        exit(0);
    } else if (argc > 2) {
        printf("Too many arguments , only one argument is required  \n");
        exit(0);
    }

    fptr = fopen(argv[1], "r"); // Open file
    if (fptr == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }

    float fileContent;
    int indx = 0;
    float wholeNumber = 0.0, nuemerator = 0.0, denominator = 0.0, fvalue = 0.0;
    while (!feof(fptr)) {

        fscanf(fptr, "%f", &fileContent);

        if (indx % 3 == 0)
            wholeNumber = fileContent;
        if (indx % 3 == 1)
            nuemerator = fileContent;
        if (indx % 3 == 2)
            denominator = fileContent;

        if (indx % 3 == 2) {

            fvalue = (wholeNumber >= 0 ? wholeNumber : -wholeNumber) + nuemerator / denominator;
            fvalue = wholeNumber >= 0 ? fvalue : - fvalue;
            printf("Floating point format of %.15f: \n       ", fvalue);

            int binaryInt = convertIntegralToBinary((int) fvalue); //  a). convert integral part to binary    
            // printf("decimal : %d\n", binaryInt);

            float binaryFract = convertFractionToBinary((fvalue - (int) fvalue)); // b). convert fractional part to binary  
            // printf("fraction: %lf\n", binaryFract);

            //Combining both the integral and fractional binary value.
            float binaryTotal = binaryInt + binaryFract;
            // printf("binary equivalent = %lf\n", binaryTotal);

            int sign = 0;
            if (fvalue >= 0) //  c). Normalize the number
                sign = 0;
            else
                sign = 1;

            int exponent = findDigitsCount(binaryInt) - 1;
            int exponentBinary = 0;
            if (fvalue != 0)
                exponentBinary = convertIntegralToBinary(127 + exponent);

            float mantissa = binaryTotal / pow(10, exponent);
            float matissaFraction = (mantissa - (int) mantissa);

            printf("%d ", sign);
            printf("%08d ", exponentBinary);
            printf("%.0f00000000000000000", round(matissaFraction * pow(10, 6)));

            printf("\n");
            wholeNumber = 0.0;
            nuemerator = 0.0;
            denominator = 0.0;
        }

        indx++;
    }
    fclose(fptr);
    return 0;

}

int convertIntegralToBinary(int intValue) {
    int binary = 0, i = 1;
    while (intValue > 0) {
        binary = binary + intValue % 2 * i;
        i = i * 10;
        intValue = intValue / 2;
    }
    return binary;
}

float convertFractionToBinary(float fractional) {
    float binaryFract = 0, k = 0.1f, temp1;
    while (k > 0.00000001) {
        temp1 = fractional * 2;
        binaryFract = binaryFract + ((int) temp1) * k;
        fractional = temp1 - (int) temp1;
        k = k / 10;
    }
    return binaryFract;
}

int findDigitsCount(int n) {
    int count = 0;
    while (n != 0) {
        n /= 10;
        ++count;
    }
    return count;
}