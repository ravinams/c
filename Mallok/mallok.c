
#include <stdio.h> // temporarily
#include <stdlib.h>
#include "mallok.h"

struct node
{
    void *startAddr; // [0-pool]
    int blockSize; // size of block
    int isFree; // Bool: 0 = false, 1 = true
    struct node *next;
};

typedef struct node Node;
typedef enum {false, true} Bool;

//void printNode(Node *);

// static global vars
static Node *head = NULL;
static void *heapSpace;

// allocates initial pool of memory (in bytes)
void create_pool(int size)
{
    // track internal state
    heapSpace = malloc(size);

    // create first node of free space
    head = (Node *) malloc(sizeof(Node));
    head->startAddr = heapSpace;
    head->blockSize = size;
    head->isFree = true;
    head->next = NULL;
}

// allocates specified chunk from my heap
void *my_malloc(int size)
{
    Node *curr = head;
    void *heapAddr = NULL;
  
    // traverse list
    while (curr != NULL)
    {
        if (curr->isFree && curr->blockSize == size)
        {
            curr->isFree = false;
            heapAddr = curr->startAddr;
            break;
        }
        else if (curr->isFree && curr->blockSize > size)
        {
            // create new node, copy curr node data to new
            Node *newNode = (Node *) malloc(sizeof(Node));
            newNode->startAddr = curr->startAddr + size;
            newNode->blockSize = curr->blockSize - size;
            newNode->isFree = curr->isFree;
            newNode->next = curr->next;

            // update curr node
            curr->blockSize = size;
            curr->isFree = false;
            curr->next = newNode;
            heapAddr = curr->startAddr;
            break;
        }

        curr = curr->next;
    }
  
    return heapAddr;
}

// returns memory back to my heap
void my_free(void *block)
{
    if (block != NULL)
    {
        Node *curr = head;
        Node *prev = NULL;

        // find node to be freed
        while (curr != NULL && curr->startAddr != block)
        {
            prev = curr;
            curr = curr->next;
        }

        if (prev == NULL) // removing head node
        {
            if (curr->next != NULL && curr->next->isFree) // mergeable
            {
                Node *temp = curr->next;
                curr->blockSize += curr->next->blockSize;
                curr->next = curr->next->next;
                free(temp);
            }
          
            curr->isFree = true;
        }
        else // somewhere else in list
        {
            // check if we can merge previous node
            if (prev->isFree)
            {
                // is this not needed?
                if (prev->startAddr < head->startAddr)
                {
                    head = prev;
                }
              
                prev->blockSize += curr->blockSize;
                prev->next = curr->next;
                free(curr);
                curr = prev;
            }

            // check if we can merge next node
            if (curr->next != NULL && curr->next->isFree)
            {
                // update current
                curr->blockSize += curr->next->blockSize;
                curr->next = curr->next->next;
                free(curr->next);
            }
            else // can't merge: make this node free space
            {
                curr->isFree = true;
            }
        }
    }
}

// print entire list of nodes

void printAll()
{
    printf("~~~~~ list start ~~~~~\n");
    printf("    head node at addr: %p\n", head);
    Node *curr = head;

    int count = 1;
    while (curr != NULL)
    {
        printf("node #%d at addr: %p\n", count, curr);
        printf("    startAddr: %p\n", curr->startAddr);
        printf("    blockSize: %d\n", curr->blockSize);
        printf("    isFree: %d (0 false, 1 true)\n", curr->isFree);
        printf("    next: %p\n", curr->next);

        curr = curr->next;
        count++;
    }
  
    printf("~~~~~ list end ~~~~~\n\n");
}

// frees all memory
void free_pool()
{
    Node *curr = NULL;

    while (head != NULL)
    {
        curr = head;
        head = head->next;
        free(curr);
    }

    free(heapSpace);
    free(head);
    heapSpace = NULL;
    head = NULL;
}