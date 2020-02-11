/*
 * mm.c
 *
 * This is the only file you should modify.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
#define DEBUG
#ifdef DEBUG
# define dbg_printf(...) printf(__VA_ARGS__)
#else
# define dbg_printf(...)
#endif


/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(p) (((size_t)(p) + (ALIGNMENT-1)) & ~0x7)


/* Basic constants and macros */
#define WSIZE       4	/* word size (bytes) */  
#define DSIZE       8	/* doubleword size (bytes) */
#define CHUNKSIZE   16	/* initial heap size (bytes) */
#define MINIMUM    24  /* minimum block size */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(int *)(p))
#define PUT(p, val)  (*(int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((void *)(bp) - WSIZE)
#define FTRP(bp)       ((void *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((void *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp)  ((void *)(bp) - GET_SIZE(HDRP(bp) - WSIZE))

/* Given block ptr bp, compute address of next and previous free blocks */
#define NEXT_FREEP(bp)(*(void **)(bp + DSIZE))
#define PREV_FREEP(bp)(*(void **)(bp))

static char *heap_listp = 0; /* Pointer to the first block */
static char *free_listp = 0;/* Pointer to the first free block */


/* Function prototypes for internal helper routines */
static void *extendHeap(size_t words);
static void place(void *bp, size_t asize);
static void *findFit(size_t asize);
static void *coalesce(void *bp);
static void printBlock(void *bp); 
static void checkBlock(void *bp);
static void insertAtFront(void *bp); /* Linked list function */
static void removeBlock(void *bp); /* Linked list function */


/*
 * Initialize: return -1 on error, 0 on success.
 */
int mm_init(void) 
{
	/*return -1 if unable to get heap space*/
	if ((heap_listp = mem_sbrk(2*MINIMUM)) == NULL) 
		return -1;

	PUT(heap_listp, 0); //Alignment padding

	/*initialize dummy block header*/
	PUT(heap_listp + WSIZE, PACK(MINIMUM, 1)); //WSIZE = padding
	PUT(heap_listp + DSIZE, 0); //PREV pointer
	PUT(heap_listp + DSIZE+WSIZE, 0); //NEXT pointer
	
	/*initialize dummy block footer*/
	PUT(heap_listp + MINIMUM, PACK(MINIMUM, 1)); 
	
	/*initialize dummy tail block*/
	PUT(heap_listp+WSIZE + MINIMUM, PACK(0, 1)); 
	
	/*initialize the free list pointer to the tail block*/
	free_listp = heap_listp + DSIZE; 

	/*return -1 if unable to get heap space*/
	if (extendHeap(CHUNKSIZE/WSIZE) == NULL) 
		return -1;
	return 0;
}


/*
 * malloc
 */
void *mm_malloc(size_t size) 
{
	size_t asize;      /* adjusted block size */
	size_t extendsize; /* amount to extend heap if no fit */
	char *bp;

	/* Ignore spurious requests */
	if (size <= 0)
		return NULL;

	/* Adjust block size to include overhead and alignment reqs */
	asize = MAX(ALIGN(size) + DSIZE, MINIMUM);

	/* Search the free list for a fit */
	if ((bp = findFit(asize))) 
	{
		place(bp, asize);
		return bp;
	}

	/* No fit found. Get more memory and place the block */
	extendsize = MAX(asize, CHUNKSIZE);
	//return NULL if unable to get heap space
	if ((bp = extendHeap(extendsize/WSIZE)) == NULL) 
		return NULL;
	place(bp, asize);
	return bp;
} 


/*
 * free
 */
void mm_free(void *bp)
{
	if(!bp) return; //return if the pointer is NULL
	size_t size = GET_SIZE(HDRP(bp));

	//set header and footer to unallocated
	PUT(HDRP(bp), PACK(size, 0)); 
	PUT(FTRP(bp), PACK(size, 0));
	coalesce(bp); //coalesce and add the block to the free list
}
/*
 * realloc - you may want to look at mm-naive.c
 */
void *mm_realloc(void *ptr, size_t size)
{
	size_t oldsize;
	void *newptr;
	size_t asize = MAX(ALIGN(size) + DSIZE, MINIMUM);
	/* If size <= 0 then this is just free, and we return NULL. */
	if(size <= 0) {
		free(ptr);
		return 0;
	}

	/* If oldptr is NULL, then this is just malloc. */
	if(ptr == NULL) {
		return malloc(size);
	}

	/* Get the size of the original block */
	oldsize = GET_SIZE(HDRP(ptr));
	
	/* If the size doesn't need to be changed, return orig pointer */
	if (asize == oldsize)
		return ptr;
	
	/* If the size needs to be decreased, shrink the block and 
	 * return the same pointer */
	if(asize <= oldsize)
	{
		size = asize;

		/* If a new block couldn't fit in the remaining space, 
		 * return the pointer */
		if(oldsize - size <= MINIMUM)
			return ptr;
		PUT(HDRP(ptr), PACK(size, 1));
		PUT(FTRP(ptr), PACK(size, 1));
		PUT(HDRP(NEXT_BLKP(ptr)), PACK(oldsize-size, 1));
		free(NEXT_BLKP(ptr));
		return ptr;
	}

	newptr = malloc(size);

	/* If realloc() fails the original block is left untouched  */
	if(!newptr) {
		return 0;
	}

	/* Copy the old data. */
	if(size < oldsize) oldsize = size;
	memcpy(newptr, ptr, oldsize);

	/* Free the old block. */
	free(ptr);

	return newptr;
}

/*
 * calloc - you may want to look at mm-naive.c
 * This function is not tested by mdriver, but it is
 * needed to run the traces.
 */
void *mm_calloc (size_t nmemb, size_t size) {

        size_t bytes = nmemb * size;
        void *newptr;

        newptr = malloc(bytes);
        memset(newptr, 0, bytes);

        return newptr;
}


/*
 * Return whether the pointer is in the heap.
 * May be useful for debugging.
 */
static int in_heap(const void *p) {
    return p < mem_heap_hi() && p >= mem_heap_lo();
}

/*
 * Return whether the pointer is aligned.
 * May be useful for debugging.
 */
static int aligned(const void *p) {
    return (size_t)ALIGN(p) == (size_t)p;
}

/*
 * mm_checkheap
 */
void mm_checkheap(int verbose) 
{
	void *bp = heap_listp; //Points to the first block in the heap

	if (verbose)
		printf("Heap (%p):\n", heap_listp);

	/* If first block's header's size or allocation bit is wrong, 
	 * the prologue header is wrong 
	*/
	if ((GET_SIZE(HDRP(heap_listp)) != MINIMUM) || 
			!GET_ALLOC(HDRP(heap_listp)))
		printf("Bad prologue header\n");
	checkBlock(heap_listp); //

	/* Print the stats of every free block in the free list */
	for (bp = free_listp; GET_ALLOC(HDRP(bp))==0; bp = NEXT_FREEP(bp)) 
	{
		if (verbose) 
			printBlock(bp);
		checkBlock(bp);
	}

	/* Print the stats of the last block in the heap */
	if (verbose)
		printBlock(bp);
	/* If last block's header's size or allocation bit is wrong, 
	 * the epilogue's header is wrong
	*/
	if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
		printf("Bad epilogue header\n");
}

static void *extendHeap(size_t words) 
{
	char *bp;
	size_t size;

	/* Allocate an even number of words to maintain alignment */
	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
	if (size < MINIMUM)
		size = MINIMUM;
	if ((long)(bp = mem_sbrk(size)) == -1) 
		return NULL;

	/* Initialize free block header/footer and the epilogue header */
	PUT(HDRP(bp), PACK(size, 0));         /* free block header */
	PUT(FTRP(bp), PACK(size, 0));         /* free block footer */
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* new epilogue header */

	/* Coalesce if the previous block was free and add the block to 
	 * the free list */
	return coalesce(bp);
}

static void place(void *bp, size_t asize)
{
	/* Gets the size of the whole free block */
	size_t csize = GET_SIZE(HDRP(bp));

	/* If the difference is at least 24 bytes, change the header and footer
	 * info for the allocated block (size = asize, allocated = 1) and
	 * remove the block from the free list.
	 * Then, split the block by:
	 * (1) Changing the header and footer info for the free block created from the
	 * remaining space (size = csize-asize, allocated = 0)
	 * (2) Coalescing the new free block with adjacent free blocks
	*/
	if ((csize - asize) >= MINIMUM) {
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		removeBlock(bp);
		bp = NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(csize-asize, 0));
		PUT(FTRP(bp), PACK(csize-asize, 0));
		coalesce(bp);
	}
	/* If the remaining space is not enough for a free block, don't split the block */
	else {
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
		removeBlock(bp);
	}
}
static void *findFit(size_t asize)
{
	void *bp;
	/* First fit search */

	/* Iterates through free list to find a free block large enough to 
	 * accomodate the size we want to allocate.
	*/
	for (bp = free_listp; GET_ALLOC(HDRP(bp)) == 0; bp = NEXT_FREEP(bp)) 
	{
		if (asize <= (size_t)GET_SIZE(HDRP(bp)))
			return bp;
    	}
	return NULL; // No fit
}

static void *coalesce(void *bp)
{
	size_t prev_alloc;
	prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp))) || PREV_BLKP(bp) == bp;
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	/* Case 1, extend the block leftward */
	if (prev_alloc && !next_alloc) 
	{			
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		removeBlock(NEXT_BLKP(bp));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}

	/* Case 2, extend the block rightward */
	else if (!prev_alloc && next_alloc) 
	{		
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		bp = PREV_BLKP(bp);
		removeBlock(bp);
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}

	/* Case 3, extend the block in both directions */
	else if (!prev_alloc && !next_alloc) 
	{		
		size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
				GET_SIZE(HDRP(NEXT_BLKP(bp)));
		removeBlock(PREV_BLKP(bp));
		removeBlock(NEXT_BLKP(bp));
		bp = PREV_BLKP(bp);
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}
	
	insertAtFront(bp);
	
	return bp;
}

static void printBlock(void *bp)
{
	int hsize, halloc, fsize, falloc;

	/* Basic header and footer information */
	hsize = GET_SIZE(HDRP(bp));
	halloc = GET_ALLOC(HDRP(bp));
	fsize = GET_SIZE(FTRP(bp));
	falloc = GET_ALLOC(FTRP(bp));

	if (hsize == 0) 
	{
		printf("%p: EOL\n", bp);
		return;
	}
	
	/* Prints out header and footer info if it's an allocated block.
	 * Prints out header and footer info and next and prev info
	 * if it's a free block.
	*/
	if (halloc)
		printf("%p: header:[%d:%c] footer:[%d:%c]\n", bp,
			hsize, (halloc ? 'a' : 'f'),
			fsize, (falloc ? 'a' : 'f'));
	else
		printf("%p:header:[%d:%c] prev:%p next:%p footer:[%d:%c]\n",
			bp, hsize, (halloc ? 'a' : 'f'), PREV_FREEP(bp),
			NEXT_FREEP(bp), fsize, (falloc ? 'a' : 'f'));
}

static void checkBlock(void *bp)
{
	// Reports if the next and prev pointers are within heap bounds
	if (NEXT_FREEP(bp)< mem_heap_lo() || NEXT_FREEP(bp) > mem_heap_hi())
		printf("Error: next pointer %p is not within heap bounds \n"
				, NEXT_FREEP(bp));
	if (PREV_FREEP(bp)< mem_heap_lo() || PREV_FREEP(bp) > mem_heap_hi())
		printf("Error: prev pointer %p is not within heap bounds \n"
				, PREV_FREEP(bp));
	/* Reports if there isn't 8-byte alignment by checking if the block pointer
	 * is divisible by 8.
	*/
	if ((size_t)bp % 8)
		printf("Error: %p is not doubleword aligned\n", bp);

	// Reports if the header information does not match the footer information
	if (GET(HDRP(bp)) != GET(FTRP(bp)))
		printf("Error: header does not match footer\n");
}

static void insertAtFront(void *bp)
{
	NEXT_FREEP(bp) = free_listp; //Sets next ptr to start of free list
	PREV_FREEP(free_listp) = bp; //Sets current's prev to new block
	PREV_FREEP(bp) = NULL; // Sets prev pointer to NULL
	free_listp = bp; // Sets start of free list as new block
}

static void removeBlock(void *bp)
{
	/* If there's a previous block, set its next pointer to the 
	 * next block.
	 * If not, set the block's previous pointer to the prev block.
	*/
	if (PREV_FREEP(bp)) 
		NEXT_FREEP(PREV_FREEP(bp)) = NEXT_FREEP(bp);
	else
		free_listp = NEXT_FREEP(bp); 
	PREV_FREEP(NEXT_FREEP(bp)) = PREV_FREEP(bp);
}

