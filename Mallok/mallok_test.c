#include <stdio.h>
#include "mallok.h"

// pool of 1000 bytes
// count how many times block size of 10 is requested  
void test1()
{
    printf("######## Start Test 1 ########\n");
    create_pool(1000);
    int counter = 0;
    printf("Creating 100, 10-byte blocks.\n");
    printf("Counter: %d\n", counter);
    while (my_malloc(10) != NULL)
    {
        counter++;
    }
    printf("Counter: %d\n", counter);
  
    printAll();
    free_pool();

    printf("######## End Test 1 ########\n\n");
}

// pool of 1000 bytes, make 5 requests for 5 blocks of 200 bytes
// free all 5 blocks
// request/free several times to make sure reusable
void test2()
{
    printf("######## Start Test 2 ########\n");
    create_pool(1000);
    void *foo1 = my_malloc(200);
    void *foo2 = my_malloc(200);
    void *foo3 = my_malloc(200);
    void *foo4 = my_malloc(200);
    void *foo5 = my_malloc(200);
    printAll();
    printf("Creating 5 blocks of 200 bytes:\n");
    printf("First block: %p\n", foo1);
    printf("Second block: %p\n", foo2);
    printf("Third block: %p\n", foo3);
    printf("Fourth block: %p\n", foo4);
    printf("Fifth block: %p\n", foo5);

    my_free(foo1);
    my_free(foo2);
    my_free(foo3);
    my_free(foo4);
    my_free(foo5);
    //printAll();
    printf("Freeing 5 blocks of 200 bytes:\n");
    printf("First block: %p\n", foo1);
    printf("Second block: %p\n", foo2);
    printf("Third block: %p\n", foo3);
    printf("Fourth block: %p\n", foo4);
    printf("Fifth block: %p\n", foo5);

    printf("\nCreating 2 blocks of 200 bytes:\n");
    foo1 = my_malloc(200);
    foo2 = my_malloc(200);
    printf("First block: %p\n", foo1);
    printf("Second block: %p\n", foo2);
    printAll();

    printf("\nFreeing 2 blocks of 200 bytes:\n");
    my_free(foo1);
    my_free(foo2);
    printf("First block: %p\n", foo1);
    printf("Second block: %p\n", foo2);
    printAll();

    free_pool();

    printf("######## End Test 2 ########\n\n");
}

// create pool of 1000 bytes
// make 5 requests for blocks of 200 bytes
// free middle block
// request 210 bytes (fail)
// request 150 bytes (success)
// request 60 bytes (fail)
// request 50 bytes (success)
void test3()
{
    printf("######## Start Test 3 ########\n");
    create_pool(1000);
    void *foo1 = my_malloc(200);
    void *foo2 = my_malloc(200);
    void *foo3 = my_malloc(200);
    void *foo4 = my_malloc(200);
    void *foo5 = my_malloc(200);
    printAll();

    my_free(foo3); // free middle node
    printAll();

    void *foo6 = my_malloc(210); // fail
    if (foo6 == NULL)
        printf("Request for 210 bytes: FAILED\n");
    else
        printf("Request for 210 bytes: PASSED\n");
  
    foo3 = my_malloc(150); // success
    if (foo3 == NULL)
        printf("Request for 150 bytes: FAILED\n");
    else
        printf("Request for 150 bytes: PASSED\n");
      
    //printAll();

    foo6 = my_malloc(60); // fail
    if (foo6 == NULL)
        printf("Request for 60 bytes: FAILED\n");
    else
        printf("Request for 60 bytes: PASSED\n");

    foo6 = my_malloc(50); // success
    if (foo6 == NULL)
        printf("Request for 50 bytes: FAILED\n");
    else
        printf("Request for 50 bytes: PASSED\n");
      
    printAll();

    free_pool();
    printf("######## End Test 3 ########\n\n");
}

// create pool of 1000 bytes
// request 5 blocks of 200 bytes
// fill first block with 'A', second with 'B', etc.
// verify values stored in each block still there
void test4()
{
    int i;
    printf("######## Start Test 4 ########\n");
    create_pool(1000);
    char *foo1 = (char *) my_malloc(200);
    char *foo2 = (char *) my_malloc(200);
    char *foo3 = (char *) my_malloc(200);
    char *foo4 = (char *) my_malloc(200);
    char *foo5 = (char *) my_malloc(200);
    printAll();

    for (i = 0; i < 200; i++)
    {
        foo1[i] = 'A';
        foo2[i] = 'B';
        foo3[i] = 'C';
        foo4[i] = 'D';
        foo5[i] = 'E';
    }

    //int i = 0;
    while (i < 200)
    {
        if (foo1[i] != 'A')
        {
            printf("FAILED CHARACTER TEST ON 'A'!\n");
            break;
        }
      
        if (foo2[i] != 'B')
        {
            printf("FAILED CHARACTER TEST ON 'B'!\n");
            break;
        }
      
        if (foo3[i] != 'C')
        {
            printf("FAILED CHARACTER TEST ON 'C'!\n");
            break;
        }
      
        if (foo4[i] != 'D')
        {
            printf("FAILED CHARACTER TEST ON 'D'!\n");
            break;
        }
      
        if (foo5[i] != 'E')
        {
            printf("FAILED CHARACTER TEST ON 'E'!\n");
            break;
        }

        i++;
    }

    printf("Successful if no characters printed out.\n");
  
    //free_pool();
    printf("######## End Test 4 ########\n\n");
}

// create pool of 1000 bytes
// request and return 1000 bytes
// request and return 4 blocks of 250 bytes
// request and return 10 blocks of 100
void test5()
{
    printf("######## Start Test 5 ########\n");
    create_pool(1000);

    void *foo1, *foo2, *foo3, *foo4, *foo5,
         *foo6, *foo7, *foo8, *foo9, *foo10;

    printf("Requesting 1000 bytes\n");
    foo1 = my_malloc(1000);
    //printAll();
    printf("Returning the 1000 bytes\n");
    my_free(foo1);
    //printAll();

    printf("Requesting four blocks of 250 bytes\n");
    //printAll();
    foo1 = my_malloc(250);
    foo2 = my_malloc(250);
    foo3 = my_malloc(250);
    foo4 = my_malloc(250);
    printAll();
    printf("Returning the blocks of 250 bytes\n");
    my_free(foo1);
    my_free(foo2);
    my_free(foo3);
    my_free(foo4);
    printAll();

    printf("Requesting ten blocks of 100 bytes\n");
    foo1 = my_malloc(100);
    foo2 = my_malloc(100);
    foo3 = my_malloc(100);
    foo4 = my_malloc(100);
    foo5 = my_malloc(100);
    foo6 = my_malloc(100);
    foo7 = my_malloc(100);
    foo8 = my_malloc(100);
    foo9 = my_malloc(100);
    foo10 = my_malloc(100);
    //printAll();
    printf("Returning the blocks of 100 bytes\n");
    my_free(foo1);
    my_free(foo2);
    my_free(foo3);
    my_free(foo4);
    my_free(foo5);
    my_free(foo6);
    my_free(foo7);
    my_free(foo8);
    my_free(foo9);
    my_free(foo10);
    //printAll();

    free_pool();
  
    printf("######## End Test 5 ########\n\n");
}

int main(void)
{
    test1();
    test2();
    test3();
    test4();
    test5();
  
    return 0;
}