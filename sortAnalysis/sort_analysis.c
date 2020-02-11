#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#define TEN_K 10000
#define TWNETY_K 20000
#define FIFTY_K  50000
#define HUNDRED_K 100000

#define MILLION 1000000.0;

// swap two elements 
void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

int partition (int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);  
    int j =0 ;
    for ( j = low; j <= high- 1; j++) 
    { 
        if (arr[j] < pivot) 
        { 
            i++ ;
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
}

void quick_sort(int arr[], int low , int high) 
{ 
   
    if (low < high) 
    { 
        /* pi is partitioning index, arr[p] is now 
           at right place */
        int pi = partition(arr, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        quick_sort(arr, low, pi - 1); 
        quick_sort(arr, pi + 1, high); 
    } 
} 

// To heapify a subtree rooted with node i which is 
// an index in arr[]. n is size of heap 
void heapify(int arr[], int n, int i) 
{ 
    int largest = i; // Initialize largest as root 
    int l = 2*i + 1; // left = 2*i + 1 
    int r = 2*i + 2; // right = 2*i + 2 
  
    // If left child is larger than root 
    if (l < n && arr[l] > arr[largest]) 
        largest = l; 
  
    // If right child is larger than largest so far 
    if (r < n && arr[r] > arr[largest]) 
        largest = r; 
  
    // If largest is not root 
    if (largest != i) 
    { 
        swap(&arr[i], &arr[largest]); 
  
        // Recursively heapify the affected sub-tree 
        heapify(arr, n, largest); 
    } 
} 
void heap_sort(int arr[] , int n) 
{
    // Build heap (rearrange array) 
    int i = 0 ;
    for (i = n / 2 - 1; i >= 0; i--) 
        heapify(arr, n, i); 
  
    // One by one extract an element from heap 
    for (i=n-1; i>=0; i--) 
    { 
        // Move current root to end 
        swap(&arr[0], &arr[i]); 
  
        // call max heapify on the reduced heap 
        heapify(arr, i, 0); 
    } 

}

// Helper function that merges two sorted arrays.
void merge( int* dst,
            int* src0, size_t begin0, size_t end0,
            int* src1, size_t begin1, size_t end1 )
{
  
  
   int t[(end0 - begin0) + (end1 - begin1)];
  int i = begin0, j = begin1, k = 0,d=end1;

  while (i <= end0 && j <= end1) {
    if (src0[i] < src1[j])
      t[k++] = src0[i++];
    else
      t[k++] = src1[j++];
  }

  //collect remaining elements 
  while (i <= end0)
    t[k++] = src0[i++];

  while (j <= end1)
    t[k++] = src1[j++];

  for (i = begin0, j = 0; i <= end1; i++, j++)
    dst[i] = t[j];
}

//------------------------------------------------------------------------
// merge_sort
//------------------------------------------------------------------------
// Sorts the array with merge sort.

void merge_sort( int arr[], size_t size )
{
 
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
 int curr_size;  // For current size of subarrays to be merged 
                   // curr_size varies from 1 to n/2 
   int left_start; // For picking starting index of left subarray 
                   // to be merged 
  
   // Merge subarrays in bottom up manner.  First merge subarrays of 
   // size 1 to create sorted subarrays of size 2, then merge subarrays 
   // of size 2 to create sorted subarrays of size 4, and so on. 
   for (curr_size=1; curr_size<=size-1; curr_size = 2*curr_size) 
   { 
       // Pick starting point of different subarrays of current size 
       for (left_start=0; left_start<size-1; left_start += 2*curr_size) 
       { 
           // Find ending point of left subarray. mid+1 is starting  
           // point of right 
           int mid = (left_start + curr_size - 1<size-1 ) ? left_start + curr_size - 1 :size-1 ; //find_min(arr,left_start + curr_size - 1, size-1); 
  
           int right_end = ( left_start + 2*curr_size - 1 < size-1 ) ? left_start + 2*curr_size - 1 : size-1 ;  //find_min(arr,left_start + 2*curr_size - 1, size-1); 
  
           // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end] 
            merge( arr,
            arr, left_start, mid,
            arr, mid+1, right_end ) ;
       } 
   } 
}

int main (int argc, char **argv)
{
     int arr_10000_random_qs[TEN_K] ,arr_10000_asc_qs[TEN_K] , arr_10000_desc_qs[TEN_K] ,
         arr_10000_random_ms[TEN_K] ,arr_10000_asc_ms[TEN_K] , arr_10000_desc_ms[TEN_K] ,
         arr_10000_random_hs[TEN_K] ,arr_10000_asc_hs[TEN_K] , arr_10000_desc_hs[TEN_K] ;

     int arr_20000_random_qs[TWNETY_K] ,arr_20000_asc_qs[TWNETY_K] , arr_20000_desc_qs[TWNETY_K]  ,
         arr_20000_random_ms[TWNETY_K] ,arr_20000_asc_ms[TWNETY_K] , arr_20000_desc_ms[TWNETY_K]  ,
         arr_20000_random_hs[TWNETY_K] ,arr_20000_asc_hs[TWNETY_K] , arr_20000_desc_hs[TWNETY_K]  ;

     int arr_50000_random_qs[FIFTY_K] ,arr_50000_asc_qs[FIFTY_K] , arr_50000_desc_qs[FIFTY_K] , 
         arr_50000_random_ms[FIFTY_K] ,arr_50000_asc_ms[FIFTY_K] , arr_50000_desc_ms[FIFTY_K]  ,
         arr_50000_random_hs[FIFTY_K] ,arr_50000_asc_hs[FIFTY_K] , arr_50000_desc_hs[FIFTY_K] ;

     int arr_100000_random_qs[HUNDRED_K] ,arr_100000_asc_qs[HUNDRED_K] , arr_100000_desc_qs[HUNDRED_K],
         arr_100000_random_ms[HUNDRED_K] ,arr_100000_asc_ms[HUNDRED_K] , arr_100000_desc_ms[HUNDRED_K],
         arr_100000_random_hs[HUNDRED_K] ,arr_100000_asc_hs[HUNDRED_K] , arr_100000_desc_hs[HUNDRED_K];



     FILE *fp;
     int i = 0 ,value=0;

     struct timeval start_time;
     struct timeval end_time;
     double elapsed_time ;

     // Step#1 - Populate the corresponding arrays from the files 

     // 10000_random.txt
     i = 0 ,value=0;
     fp = fopen ("10000_random.txt", "r");
     while ( i < TEN_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_10000_random_qs[i] = value;
       arr_10000_random_ms[i] = value;
       arr_10000_random_hs[i] = value;
       i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 

     // 10000_asc.txt    
     i = 0 ,value=0;
     fp = fopen ("10000_asc.txt", "r");
     while ( i < TEN_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_10000_asc_qs[i] = value;
       arr_10000_asc_ms[i] = value;
       arr_10000_asc_hs[i] = value;
        i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 

     // 10000_desc.txt

      i = 0 ,value=0;
     fp = fopen ("10000_desc.txt", "r");
     while ( i < TEN_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_10000_desc_qs[i] = value;
       arr_10000_desc_ms[i] = value;
       arr_10000_desc_hs[i] = value;
       i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 

     // printf(" *** %d ***  \n",i);
     // int c = 0;
     // for (c = 0; c < TEN_K; c++) 
     //   printf("%d \n", arr_10000_desc[c]);

     

     // 20000_random.txt
     i = 0 ,value=0;
     fp = fopen ("20000_random.txt", "r");
     while ( i < TWNETY_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_20000_random_qs[i] = value;
       arr_20000_random_ms[i] = value;
       arr_20000_random_hs[i] = value;
       i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 

     // 20000_asc.txt    
     i = 0 ,value=0;
     fp = fopen ("20000_asc.txt", "r");
     while ( i < TWNETY_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_20000_asc_qs[i] = value;
       arr_20000_asc_ms[i] = value;
       arr_20000_asc_hs[i] = value;
       i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 

     // 20000_desc.txt

      i = 0 ,value=0;
     fp = fopen ("20000_desc.txt", "r");
     while ( i < TWNETY_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_20000_desc_qs[i] = value;
       arr_20000_desc_ms[i] = value;
       arr_20000_desc_hs[i] = value;
       i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 


  // 50000_random.txt
     i = 0 ,value=0;
     fp = fopen ("50000_random.txt", "r");
     while ( i < FIFTY_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_50000_random_qs[i] = value;
       arr_50000_random_ms[i] = value;
       arr_50000_random_hs[i] = value;
       i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 

     // 50000_asc.txt    
     i = 0 ,value=0;
     fp = fopen ("50000_asc.txt", "r");
     while ( i < FIFTY_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_50000_asc_qs[i] = value;
       arr_50000_asc_ms[i] = value;
       arr_50000_asc_hs[i] = value;
       i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 

     // 50000_desc.txt

      i = 0 ,value=0;
     fp = fopen ("50000_desc.txt", "r");
     while ( i < FIFTY_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_50000_desc_qs[i] = value;
       arr_50000_desc_ms[i] = value;
       arr_50000_desc_hs[i] = value;
       i++;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 


// 100000_random.txt
     i = 0 ,value=0;
     fp = fopen ("100000_random.txt", "r");
     while ( i < HUNDRED_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_100000_random_qs[i] = value;
        arr_100000_random_ms[i] = value;
         arr_100000_random_hs[i] = value;
         i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 

     // 100000_asc.txt    
     i = 0 ,value=0;
     fp = fopen ("100000_asc.txt", "r");
     while ( i < HUNDRED_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_100000_asc_qs[i] = value;
        arr_100000_asc_ms[i] = value;
         arr_100000_asc_hs[i] = value;
         i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 

     // 100000_desc.txt

      i = 0 ,value=0;
     fp = fopen ("100000_desc.txt", "r");
     while ( i < HUNDRED_K && !feof (fp))
     {  
       fscanf (fp, "%d", &value);
       //printf ("%d ", value);
       arr_100000_desc_qs[i] = value;
       arr_100000_desc_ms[i] = value;
       arr_100000_desc_hs[i] = value;
       i++ ;
     // printf(" *** %d %d ***  \n",i ,arr_10000_random[i]);
     }     
     fclose(fp); 



   // STEP2  sort the arrays

      /***********************************
        START QUICK SORT
      ************************************/

     printf("\n***********QUICK SORT*******************\n");

   /***********************************
          TEN_K  RANDOM
      ************************************/ 

	 gettimeofday( &start_time, NULL );
     quick_sort(arr_10000_random_qs,0,TEN_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 10000 of random integers :%f \n", elapsed_time);

        /***********************************
          TEN_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     quick_sort(arr_10000_asc_qs,0,TEN_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 10000 of ascending integers :%f \n", elapsed_time);

    
        /***********************************
          TEN_K  DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     quick_sort(arr_10000_desc_qs,0,TEN_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 10000 of descending integers :%f \n", elapsed_time);

       // 20K

     
      /***********************************
          TWENTY_K  RANDOM
      ************************************/ 

     gettimeofday( &start_time, NULL );
     quick_sort(arr_20000_random_qs,0,TWNETY_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 20000 of random integers :%f \n", elapsed_time);

    

      /***********************************
          TWENTY_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     quick_sort(arr_20000_asc_qs,0,TWNETY_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 20000 of ascending integers :%f \n", elapsed_time);

     
      /***********************************
          TWENTY_K  DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     quick_sort(arr_20000_desc_qs,0,TWNETY_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 20000 of descending integers :%f \n", elapsed_time);


       // 50K

      /***********************************
          FIFTY_K  RANDOM
      ************************************/ 

     gettimeofday( &start_time, NULL );
     quick_sort(arr_50000_random_qs,0,FIFTY_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 50000 of random integers :%f \n", elapsed_time);
  
     /***********************************
          FIFTY_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     quick_sort(arr_50000_asc_qs,0,FIFTY_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 50000 of ascending integers :%f \n", elapsed_time);

     
      /***********************************
          FIFTY_K  DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     quick_sort(arr_50000_desc_qs,0,FIFTY_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 50000 of descending integers :%f \n", elapsed_time);

       

      /***********************************
          HUNDRED_K  RANDEOM
      ************************************/ 
     gettimeofday( &start_time, NULL );
     quick_sort(arr_100000_random_qs,0,HUNDRED_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 100000 of random integers :%f \n", elapsed_time);

     /***********************************
          HUNDRED_K  ASC
      ************************************/

     gettimeofday( &start_time, NULL );
     quick_sort(arr_100000_asc_qs,0,HUNDRED_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 100000 of ascending integers :%f \n", elapsed_time);

     
      /***********************************
          HUNDRED_K  DESC
      ************************************/

     gettimeofday( &start_time, NULL );
     quick_sort(arr_100000_desc_qs,0,HUNDRED_K-1);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'quick sort' with input size 100000 of descending integers :%f \n", elapsed_time);

     /***********************************
          END QUICK SORT
      ************************************/

   

      /***********************************
        START MERGE SORT
      ************************************/

      printf("\n***********MERGE SORT*******************\n");

      /***********************************
          TEN_K  RANDOM
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_10000_random_ms,TEN_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 10000 of random integers :%f \n", elapsed_time);

      /***********************************
          TEN_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_10000_asc_ms,TEN_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 10000 of asc integers :%f \n", elapsed_time);

      /***********************************
          TEN_K  DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_10000_desc_ms,TEN_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 10000 of desc integers :%f \n", elapsed_time);


     /***********************************
          TWNETY_K  RANDOM
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_20000_random_ms,TWNETY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 20000 of random integers :%f \n", elapsed_time);

      /***********************************
          TWNETY_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_20000_asc_ms,TWNETY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 20000 of asc integers :%f \n", elapsed_time);

      /***********************************
          TWNETY_K  DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_20000_desc_ms,TWNETY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 20000 of desc integers :%f \n", elapsed_time);


/***********************************
          FIFTY_K  RANDOM
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_50000_random_ms,FIFTY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 50000 of random integers :%f \n", elapsed_time);

      /***********************************
          FIFTY_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_50000_asc_ms,FIFTY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 50000 of asc integers :%f \n", elapsed_time);

      /***********************************
          FIFTY_K  DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_50000_desc_ms,FIFTY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 50000 of desc integers :%f \n", elapsed_time);


     /***********************************
          HUNDRED_K  RANDOM
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_100000_random_ms,HUNDRED_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 100000 of random integers :%f \n", elapsed_time);

      /***********************************
          HUNDRED_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_100000_asc_ms,HUNDRED_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 100000 of asc integers :%f \n", elapsed_time);

      /***********************************
         HUNDRED_K   DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     merge_sort(arr_100000_desc_ms,HUNDRED_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'merge sort' with input size 100000 of desc integers :%f \n", elapsed_time);


     /***********************************
          END MERGE SORT
      ************************************/


 /***********************************
        START HEAP SORT
      ************************************/

      printf("\n***********HEAP SORT*******************\n");

      /***********************************
          TEN_K  RANDOM
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_10000_random_hs,TEN_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 10000 of random integers :%f \n", elapsed_time);

      /***********************************
          TEN_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_10000_asc_hs,TEN_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 10000 of asc integers :%f \n", elapsed_time);

      /***********************************
          TEN_K  DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_10000_desc_hs,TEN_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 10000 of desc integers :%f \n", elapsed_time);


     /***********************************
          TWNETY_K  RANDOM
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_20000_random_hs,TWNETY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 20000 of random integers :%f \n", elapsed_time);

      /***********************************
          TWNETY_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_20000_asc_hs,TWNETY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 20000 of asc integers :%f \n", elapsed_time);

      /***********************************
          TWNETY_K  DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_20000_desc_hs,TWNETY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 20000 of desc integers :%f \n", elapsed_time);


/***********************************
          FIFTY_K  RANDOM
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_50000_random_hs,FIFTY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 50000 of random integers :%f \n", elapsed_time);

      /***********************************
          FIFTY_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_50000_asc_hs,FIFTY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 50000 of asc integers :%f \n", elapsed_time);

      /***********************************
          FIFTY_K  DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_50000_desc_hs,FIFTY_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 50000 of desc integers :%f \n", elapsed_time);


     /***********************************
          HUNDRED_K  RANDOM
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_100000_random_hs,HUNDRED_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 100000 of random integers :%f \n", elapsed_time);

      /***********************************
          HUNDRED_K  ASC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_100000_asc_hs,HUNDRED_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 100000 of asc integers :%f \n", elapsed_time);

      /***********************************
         HUNDRED_K   DESC
      ************************************/ 

     gettimeofday( &start_time, NULL );
     heap_sort(arr_100000_desc_hs,HUNDRED_K);
     gettimeofday( &end_time, NULL );
     elapsed_time = ( end_time.tv_sec - start_time.tv_sec ) +
                        ( end_time.tv_usec - start_time.tv_usec ) / MILLION;
     printf("\nTime elapsed for 'heap sort' with input size 100000 of desc integers :%f \n", elapsed_time);


     /***********************************
          END HEAP SORT
      ************************************/


 }