#include <stdio.h>
#include <stdlib.h>


char isCombinationExists(int arr[],int size, int denorm , int year)
{
    
    int i ;
    for(i = 0; i < size*2; i+=2)
    {
       if (arr[i] == denorm && arr[i+1] == year) 
           return 'Y' ;
    }
    return 'N' ;
}

void uniqueDates(int arr[],int size)
{
   
    int uniqueDatesArray[size] ;
    int i , j ,dateArraySize = 0, temp;
    for(i = 1; i < size*2; i+=2) {
        temp = arr[i] ;
       //  printf("index i =%d,temp = %d\n",i,temp) ;
        if (i == 0)            
            uniqueDatesArray[dateArraySize++] = temp ;
        else
        {
            char notFound = 'Y' ;
            for (j = 0 ; j < dateArraySize ; j++)
            {
                if (temp == uniqueDatesArray[j])
                   notFound = 'N' ;
            }
            
            if (notFound == 'Y')
                uniqueDatesArray[dateArraySize++] = temp ;            
        }
    }
    
    printf("Unique dates in set:") ;
    int k ;
    for (k = 0 ; k < dateArraySize ; k++)
        printf("%d ",uniqueDatesArray[k]) ;
 
 }
    
   
void uniqueDenominations(int arr[],int size) 
{
    int m , n ,denomArraySize = 0, tempDenom;
    int uniqueDenomArray[size] ;
    for(m = 0; m < size*2; m+=2) {
        tempDenom = arr[m] ;
       //  printf("index i =%d,temp = %d\n",i,temp) ;
        if (m == 0)            
            uniqueDenomArray[denomArraySize++] = tempDenom ;
        else
        {
            char notFound = 'Y' ;
            for (n = 0 ; n < denomArraySize ; n++)
            {
                if (tempDenom == uniqueDenomArray[n])
                   notFound = 'N' ;
            }
            
            if (notFound == 'Y')
                uniqueDenomArray[denomArraySize++] = tempDenom ;            
        }
    }
    
    printf("Unique denominations in set:") ;
    int k ;
    for (k = 0 ; k < denomArraySize ; k++)
        printf("%d ",uniqueDenomArray[k]) ;
    
}
void incompleteDates(int arr[],int size)
{
     
    
     
     // uniqueDatesArray - START      
    int uniqueDatesArray[size] ;
    int i , j ,dateArraySize = 0, tempDate;
    for(i = 1; i < size*2; i+=2) {
        tempDate = arr[i] ;
       //  printf("index i =%d,temp = %d\n",i,temp) ;
        if (i == 0)            
            uniqueDatesArray[dateArraySize++] = tempDate ;
        else
        {
            char notFound = 'Y' ;
            for (j = 0 ; j < dateArraySize ; j++)
            {
                if (tempDate == uniqueDatesArray[j])
                   notFound = 'N' ;
            }
            
            if (notFound == 'Y')
                uniqueDatesArray[dateArraySize++] = tempDate ;            
        }
    }
    
    
    // uniqueDatesArray - END 
    
    // denomArraySize - START 
    int m , n ,denomArraySize = 0, tempDenom;
    int uniqueDenomArray[size] ;
    for(m = 0; m < size*2; m+=2) {
        tempDenom = arr[m] ;
       //  printf("index i =%d,temp = %d\n",i,temp) ;
        if (m == 0)            
            uniqueDenomArray[denomArraySize++] = tempDenom ;
        else
        {
            char notFound = 'Y' ;
            for (n = 0 ; n < denomArraySize ; n++)
            {
                if (tempDenom == uniqueDenomArray[n])
                   notFound = 'N' ;
            }
            
            if (notFound == 'Y')
                uniqueDenomArray[denomArraySize++] = tempDenom ;            
        }
    }
    // denomArraySize - END 
    
   // process to find incomplete dates  
    int incompleteDates[size] , dateSize = 0 ;
    for(i = 0; i < dateArraySize; i++) {
        int uDate = uniqueDatesArray[i] ;
        for (j= 0 ; j < denomArraySize ; j++)
        {
            int uDenom = uniqueDenomArray[j] ;            
             char found = isCombinationExists(arr,size,uDenom,uDate) ;
            if (found == 'N' )
                incompleteDates[dateSize++] = uDate ;
        }
        
    }
    
    
    if (dateSize == 0)
        printf("There are no incomplete sets of dates.");
    else
    {
        printf("Incomplete sets for the following dates:") ;
        int x = 0;
        for (x = dateSize-1 ; x >=0 ; x--)
             printf("%d ",incompleteDates[x]) ;
        
    }       
}
void incompleteCoins(int arr[],int size)
{
   
     
     // uniqueDatesArray - START      
    int uniqueDatesArray[size] ;
    int i , j ,dateArraySize = 0, tempDate;
    for(i = 1; i < size*2; i+=2) {
        tempDate = arr[i] ;
       //  printf("index i =%d,temp = %d\n",i,temp) ;
        if (i == 0)            
            uniqueDatesArray[dateArraySize++] = tempDate ;
        else
        {
            char notFound = 'Y' ;
            for (j = 0 ; j < dateArraySize ; j++)
            {
                if (tempDate == uniqueDatesArray[j])
                   notFound = 'N' ;
            }
            
            if (notFound == 'Y')
                uniqueDatesArray[dateArraySize++] = tempDate ;            
        }
    }
    
    
    // uniqueDatesArray - END 
    
    // denomArraySize - START 
    int m , n ,denomArraySize = 0, tempDenom;
    int uniqueDenomArray[size] ;
    for(m = 0; m < size*2; m+=2) {
        tempDenom = arr[m] ;
       //  printf("index i =%d,temp = %d\n",i,temp) ;
        if (m == 0)            
            uniqueDenomArray[denomArraySize++] = tempDenom ;
        else
        {
            char notFound = 'Y' ;
            for (n = 0 ; n < denomArraySize ; n++)
            {
                if (tempDenom == uniqueDenomArray[n])
                   notFound = 'N' ;
            }
            
            if (notFound == 'Y')
                uniqueDenomArray[denomArraySize++] = tempDenom ;            
        }
    }
    // denomArraySize - END 
    
       
    // process to find incomplete dates  
    int incompleteCoins[size] , coinsSize = 0 ;
    for(i = 0; i < denomArraySize; i++) {
        int uDenorm = uniqueDenomArray[i] ;
        for (j= 0 ; j < dateArraySize ; j++)
        {
            int uDate = uniqueDatesArray[j] ;            
             char found = isCombinationExists(arr,size,uDenorm,uDate) ;
            if (found == 'N' )
                incompleteCoins[coinsSize++] = uDenorm ;
        }
        
    }
    
    
     if (coinsSize == 0)
        printf("There are no incomplete sets of coins.");
    else
    {
        printf("Incomplete sets for the following coins:") ;
        int x = 0;
        for (x = coinsSize-1 ; x >=0 ; x--)
             printf("%d ",incompleteCoins[x]) ;
        
    }      
    
    
}


int main(int argc, char** argv) {

    int val;

    const int MAX_COINS = 50;

    int nums[MAX_COINS * 2 ];

    //int coins[MAX_COINS][2];

    printf("Enter data now -> ");

    char temp;
    int value ;
    int i=0, j=0,size=0;
    
   do {
        
        scanf("%d%c", &value, &temp);
       
        // stop the input if value is either -1 or MAX coins entered 
        if (value == -1 || i == (MAX_COINS * 2))
            break ; // exit the loop ;
       
       nums[i++]=value;
                   
    } while (temp != '\n');

   fflush(stdin);
    
//   for (j = 0; j < i; j++) {
//       printf("%d ", nums[j]);
//    }
   
    size = i/2 ;
    int userChoice  ;
    char proceed = 'Y' ;
    
    do{
        
        printf("\n1. Display unique dates.\n") ;
        printf("2. Display unique denominations.\n") ;
        printf("3. Display incomplete set data for dates.\n") ;
        printf("4. Display incomplete set data for coins.\n") ;
        printf("Enter option for data set (size %d) -> ",size) ;
        scanf("%d",&userChoice) ;
        if (userChoice < 1 || userChoice > 4)
        {
            printf("\nError! Select a valid option from the menu above!\n");
            proceed = 'Y' ;
        }
        else
        {
            fflush(stdout);
            
            if (userChoice == 1)
                uniqueDates(nums,size) ;
            else if (userChoice == 2)
                uniqueDenominations(nums,size) ;
            else if (userChoice == 3)
                incompleteDates(nums,size) ;
             else if (userChoice == 4)
                 incompleteCoins(nums,size) ;
            
            proceed = 'N' ; ; // exit the program ;
        }
        
               
    }
    while(proceed == 'Y') ;
   

   

    return (EXIT_SUCCESS);
}

