#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Palindrome {



int begin;

int end;

};



void setStringLength(char *str, int length, int begin, int end, struct Palindrome *netString, int *pal_count)

{


while (begin >= 0 && end < length && str[begin] == str[end])

{



netString[*pal_count].begin = begin;



netString[*pal_count].end = end;



(*pal_count)++;



begin--;



end++;

}

}



void GeneratePalindrome( int count, int begin,char *str, int length, struct Palindrome **array, int size,char ***netString, int *temp1, int *pal_count, struct Palindrome *pal_set)

{

int p;

if (size > 0 && array[size - 1]->end == length - 1)

{

temp1[*pal_count] = size;

netString[*pal_count] = malloc(size * sizeof(char *));

for (p = 0; p < size; p++)

{

int begin = array[p]->begin;

int end = array[p]->end;

netString[*pal_count][p] = malloc(end - begin + 2);

memcpy(netString[*pal_count][p], str + begin, end - begin + 1);

netString[*pal_count][p][end - begin + 1] = '\0';

}

(*pal_count)++;

}



else

{



for (p = begin; p < count; p++)

{


if ((size == 0 && pal_set[p].begin == 0) ||(size > 0 && array[size - 1]->end + 1 == pal_set[p].begin))

{



array[size] = &pal_set[p];



GeneratePalindrome(count, p + 1, str, length, array, size + 1, netString, temp1, pal_count, pal_set);

}

}

}


}


char ***ReturnPalindrome(char* str, int**sizes, int* sizeCount)

{



int len = strlen(str);


if (len == 0)

{

*sizeCount = 0;

return NULL;

}



int p, capacity =100, pal_count = 0;



struct Palindrome *pal_set = malloc(capacity * sizeof(*pal_set));

for (p = 0; p < len; p++) {



setStringLength(str, len, p, p, pal_set, &pal_count);

setStringLength(str, len, p, p + 1, pal_set, &pal_count);

}



char ***netString = malloc(capacity * sizeof(char **));



struct Palindrome **array = malloc(pal_count * sizeof(*array));

*sizes = malloc(capacity * sizeof(int));



GeneratePalindrome(pal_count, 0, str, len, array, 0, netString, *sizes, sizeCount, pal_set);


return netString;

/*int b;
for(b=0;b<capacity;b++)
{
    free(netString[b]);
}
free (netString);
for(b=0;b<pal_count;b++)
{
    free(array[b]);

}
free (array);

//for(b=0;b<capacity;b++)
//{
    //free(pal_set[b]);

//}
//free(pal_set);
*/
}




int main()
{
   int i;
   int k;
   int j;
   int MAX_SIZE=100;
   int array[MAX_SIZE];
    int size;
    size=0;
    printf("enter size of array\n");
    scanf("%d",&size);


   while(size!=-1)
   {
       printf("\nEnter values for array\n");
       for(i=0;i<size;i++)
       {
           scanf("%d",&array[i]);
       }

       int res= increasing(array,size);
       printf("result is %d\n",res);
    printf("enter size of array\n");
    scanf("%d",&size);

   }





int A[5] = {20,25,30,2,1};
int rec_ct=0;
int base_ct=0;
int max_depth=0;

printf("Using min_rec_tail\nMinimum of array: %d\n",min_rec_tail(A,5,&rec_ct,&base_ct,&max_depth));
printf("number of recursive calls: %d\nnumber of base cases: %d\nMax_depth: %d\n",rec_ct,base_ct,max_depth);
rec_ct=base_ct=0;
max_depth=0;
printf("\nUsing min_rec_1\nMinimum of array: %d\n",min_rec_1(A,5,&rec_ct,&base_ct,&max_depth));
printf("number of recursive calls: %d\nnumber of base cases: %d\nMax_depth: %d\n",rec_ct,base_ct,max_depth);

rec_ct=0;
base_ct=0;
max_depth=0;

printf("\nUsing min_rec_2\nMinimum of array: %d\n",min_rec_2(A,5,&rec_ct,&base_ct,&max_depth));
printf("number of recursive calls: %d\nnumber of base cases: %d\nMax_depth: %d\n",rec_ct,base_ct,max_depth);

int pal_count = 0;

int *temp1;

int choice = 0;

char string[100];

char ***substring;



do

{


printf("Enter the string: ");

scanf("%s", &string);




substring = ReturnPalindrome(string, &temp1, &pal_count);



printf("The string %s has %d palindromic decompositions: \n", string, pal_count);



for (j = 0; j < pal_count; j++)

{

char **list = substring[j];

for (k = 0; k < temp1[j]; k++)

{

printf("%s, ", list[k]);

}

printf("\n");

}

printf("\nPress 0: To Continue\nPress -1: To Exit\n");

scanf("%d", &choice);

pal_count = 0;

} while (choice != -1);
return 0;


}




/*int increasing(int *A,int N,int k)

{

    if(N==0)
    {
        return 1;
    }
    if(N==1)
    {
        return 1;
    }
    if(N==2)
    {
        if(A[k]==A[k+1])
        {
            return 1;
        }
    }
    if(k==N)
    {
        return 1;
    }
    if(A[k]>A[k+1])
    {
        return 0;
    }

    else
    {

        return increasing(A,N,k+1);
    }
}

*/
int increasing(int * A, int N) {

if (N == 1 || N == 0)
{
    return 1;
}


if (A[N-1] < A[N-2])
{
return 0;

}

return increasing(A,N-1);

}
int min_rec_tail(int* A, int N, int* rec_ct, int* base_ct, int* max_depth)
{
if(N==1)
{
*(base_ct)=*(base_ct)+1;
*(max_depth)=*(max_depth)+1;
return A[0];
}

N=N-1;
*(max_depth)=*(max_depth)+1;
 *(rec_ct)=*(rec_ct)+1;
return min_rec_tail(A + (A[0] > A[N]), N,&(*rec_ct),&(*base_ct),&(*max_depth));
}


int min_rec_1(int* A, int N,int* rec_ct,int* base_ct,int* max_depth)
{
	// crashes for N < = 0
	if (N==1)
    {
    *(base_ct)=*(base_ct)+1;
    *(max_depth)=*(max_depth)+1;
    return A[0];
    }
 //   *(max_depth)=*(max_depth)+1;
   // *(rec_ct)=*(rec_ct)+1;
   *(max_depth)=*(max_depth)+1;
	int temp = min_rec_1(A,N-1,&(*rec_ct),&(*base_ct),&(*max_depth));
	*(rec_ct)=*(rec_ct)+1;
	if (temp < A[N-1])
    {
    return temp;
    //printf("temp is %d",temp);

    }
	else
    {
		return A[N-1];
    }

}

int min_rec_2(int* A, int N,int* rec_ct,int* base_ct,int* max_depth)
{
	// crashes for N < = 0
	if (N==1)
    {
    *(base_ct)=*(base_ct)+1;

   // *(max_depth)=*(max_depth)+1;
   *(max_depth)=*(rec_ct)-*(base_ct);
    return A[0];

	}
	//*(rec_ct)=*(rec_ct)+1;
	if (min_rec_2(A,N-1,&(*rec_ct),&(*base_ct),&(*max_depth)) < A[N-1])
    {
     *(rec_ct)=*(rec_ct)+2;
  //   *(max_depth)=*(max_depth)+1;
    return min_rec_2(A,N-1,&(*rec_ct),&(*base_ct),&(*max_depth));

    }
	else
		return A[N-1];
}















