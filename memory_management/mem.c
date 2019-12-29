/* mem.c 
 * Jonathan Pugliese
 * ECE 2230 Spring 2019
 * MP4
 *
 * Propose: A template for MP4, implementing a user written version
 * of malloc and free in order to better understand dynamic memory management
 * 
 * Assumptions: 
 *  PAGESIZE and sizeof(chunk_t) are set value and will not change
 *  of the program
 * Bugs: None
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include "mem.h"

/* chunk_t definition (do not modify).
 * sizeof(chunk_t) must equal SIZEOF_CHUNK_T in mem.h.
 */
typedef struct chunk_tag {
    struct chunk_tag *next;  /* pointer to the next free block */
    long size;               /* size of memory block in units of
                                sizeof(chunk_t) */
} chunk_t;

typedef struct heap_stats {
    int numItems;    /* number of chunks in the free list    */
    int min;         /* size of the smallest chunk, in bytes */
    int max;         /* size of the largest chunk, in bytes  */
    int average;     /* average size of the chunks, in bytes */
    int totalBytes;  /* total size of all chunks, in bytes   */

    /* the following two fields are updated in morecore() */
    int numSbrkCalls;  /* number of successful calls to sbrk()  */
    int numPages;      /* number of pages allocated with sbrk() */
} heap_stats_t;

/* global variables exported via mem.h */
int SearchPolicy = FIRST_FIT;
int SearchLoc = HEAD_FIRST;
int Coalescing = FALSE;

/* global variables restricted to mem.c only */
static chunk_t Dummy = {
    &Dummy,  /* next */
    0        /* size */
};
static chunk_t * Rover = &Dummy;
static heap_stats_t stats;  /* initialized by the O/S to all 0s */


/* prototypes for functions private to mem.c */
void mem_validate(void);
chunk_t *morecore(int);

/* function to request 1 or more pages from the operating system.
 *
 * new_bytes must be the number of bytes that are being requested from
 *           the OS with the sbrk command.  It must be an integer 
 *           multiple of the PAGESIZE
 *
 * returns a pointer to the new memory location.  If the request for
 * new memory fails this function simply returns NULL, and assumes some
 * calling function will handle the error condition.  Since the error
 * condition is catastrophic, nothing can be done but to terminate 
 * the program.
 */
chunk_t *morecore(int new_bytes) 
{
    char *cp;
    chunk_t *new_p;

    /* assert preconditions */
    assert(new_bytes % PAGESIZE == 0 && new_bytes > 0);
    assert(PAGESIZE % sizeof(chunk_t) == 0);

    /* allocate memory using sbrk() */
    cp = sbrk(new_bytes);
    if (cp == (char *) -1)  /* no space available */
        return NULL;
    new_p = (chunk_t *) cp;
    
    /* update statistics */
    stats.numSbrkCalls++;
    stats.numPages += new_bytes/PAGESIZE;

    return new_p;
}

/* Mem_free takes in a pointer, then links it into the circular list
    so that it will become available for allocation again */
void Mem_free(void *return_ptr)
{

   chunk_t *node;
 
   node = ((chunk_t *)return_ptr) - 1; // reach header block of pointer
   node->next = Rover->next;
   Rover->next = node;
   Rover = node;

}

/* Mem_alloc takes in the requested number of bytes needed, then checks through the free list
   to find a chunk to carve from based on the search policy. Note that bytes are converted to units of
   size chunk_t. If memory size needed is not found, function requests a new page from memory to carve from. */
void *Mem_alloc(int nbytes) {
    /* assert preconditions */
    assert(nbytes > 0);
    if(SearchLoc == HEAD_FIRST){
	Rover = &Dummy;	//Resets Rover to beginning of list to search
    }
    chunk_t *current;
    chunk_t *temp;
    void *ret = NULL;
    int searching = 1;

    int nunits;
	 if(nbytes % sizeof(chunk_t) == 0){	//convert bytes to units
		nunits = nbytes/sizeof(chunk_t) + 1;
	 }
	 else{
		nunits = nbytes/sizeof(chunk_t) + 2;
	}

    int req_bytes = floor(nbytes/PAGESIZE) + 1;
    temp = Rover;
    if(temp == temp->next){	//Free list only holds dummy block
	
	//allocate tail end
	temp = morecore(req_bytes*PAGESIZE);
	temp->next = &Dummy;
	Rover->next = temp;
	temp->size = ((req_bytes*PAGESIZE)/sizeof(chunk_t));
	Rover = temp;
	
    } 

 if(SearchPolicy == FIRST_FIT){
    for(current = temp->next; searching == 1; current = current->next){
		
		if(current->size >= nunits){		// Block is big enough to carve from.

			if(current->size == nunits){	// requested is exactly block size
				Rover = Rover->next;
				while(temp->next != current){
					temp = temp->next;
				}
				temp->next = Rover;        //adjust list
			}
			else{
				current->size = current->size - nunits;	//carve size
				Rover = current;
				current = current + current->size;  //move pointer forward a value equal to its size
				current->size = nunits;	// set new pointer created to proper size
				//Rover = temp;
				
			}
			ret = current + 1; //returns memory accesible by user
			searching = 0;
		}

        if(current == Rover){   //moved through entire free list
                current = morecore(req_bytes*PAGESIZE); //grabs new page
                if(current == NULL){            //system out of memory
                        ret = NULL;
                        searching = 0;
                }
                else{
                        current->size = ((req_bytes*PAGESIZE)/sizeof(chunk_t));
                        Mem_free(current + 1); //Links new pointer to list
                        Rover = current;

                }
        }

    }

return ret;
}

  else if(SearchPolicy == BEST_FIT){
		//loop looking for best
		current = temp = Rover;
		while(temp->next != Rover){
			if(temp->size >= nunits){
				if(current == Rover){current = temp;}

				else if(current->size > temp->size){
					current = temp;			
				}

			}
		    temp = temp->next;
		}

	Rover = temp = current;	//create uniform starting point
	if(current->size == nunits){    // requested is exactly block size
               //Rover = temp = current;
	       Rover = Rover->next;
               while(temp->next != current){
                      temp = temp->next;
               }
               temp->next = Rover;        //adjust list
         }
	 else{
	       current->size = current->size - nunits;
               Rover = current;
	       current = current + current->size;
               current->size = nunits;

         }

	ret = current + 1;
	return ret;
		
   }
	
	else if(SearchPolicy == WORST_FIT){
		//move through and look for max 
                current = temp = Rover;
		temp = temp->next;	//move the search pointer forward one position
                while(temp != Rover){
                        if(temp->size >= nunits){
                                if(current == Rover){current = temp;}

                                else if(temp->size > current->size){
                                        current = temp;
                                }

                        }
                    temp = temp->next;
                }

	 Rover = temp = current;
        if(current->size == nunits){    // requested is exactly block size
               //Rover = temp = current;
               Rover = Rover->next;
               while(temp->next != current){
                      temp = temp->next;
               }
               temp->next = Rover;        //adjust list
         }
         else{
               current->size = current->size - nunits;
               Rover = current;
               current = current + current->size;
               current->size = nunits;

         }

        ret = current + 1;
        return ret;


	}

return ret; // Returns NULL, program should never make it here unless there is an error with Search Policy
}

/* prints stats about the current free list
 *
 * -- number of items in the linked list including dummy item
 * -- min, max, and average size of each item (in bytes)
 * -- total memory in list (in bytes)
 * -- number of calls to sbrk and number of pages requested
 *
 * A message is printed if all the memory is in the free list
 */
void Mem_stats(void)
{
    /* TODO calculate the latest stats and put them in the stats struct */
    chunk_t *start = &Dummy;
    chunk_t *temp = start;
    stats.numItems = 0;
    stats.totalBytes = 0;
    stats.average = 0;
    stats.min = 0;
    stats.max = 0;
 
    while(temp->next != start){
	stats.numItems++;
	temp = temp->next;
	stats.totalBytes += temp->size*sizeof(chunk_t);

	if(temp->size*sizeof(chunk_t) > stats.max){
		stats.max = temp->size*sizeof(chunk_t);
		if(stats.min == 0){
			stats.min = stats.max;
		}
	}

	if(temp->size*sizeof(chunk_t) < stats.min){
		stats.min = temp->size*sizeof(chunk_t);
	}

    }
    if(stats.numItems != 0){
    	stats.average = (stats.totalBytes)/stats.numItems;
    }
    else{
	stats.average = 0;
    }
        
	
    /* ======= DO NOT MODIFY FROM HERE TO END OF Mem_stats() ======= */
    printf("\n\t\tMP4 Heap Memory Statistics\n"
            "----------------------------------------------------\n\n"
            "\tNumber of blocks in free list: %d\n"
            "\tMin: %d\n"
            "\tMax: %d\n"
            "\tAverage: %d\n"
            "\tTotal bytes in free list: %d\n"
            "\tNumber of sbrk calls: %d\n"
            "\tNumber of requested pages: %d\n"
            "\tHeap status:  %s\n",
            stats.numItems, stats.min, stats.max, stats.average, stats.totalBytes,
            stats.numSbrkCalls, stats.numPages,
            stats.totalBytes == stats.numPages*PAGESIZE ? \
            "all memory is in the heap -- no leaks are possible\n"\
            : "heap is in-use -- leaks are possible\n");
}

/* print table of memory in free list 
 *
 * The print should include the dummy item in the list 
 */
void Mem_print(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    chunk_t *p = Rover;
    chunk_t *start = p; /* save my starting block */
    do {
        printf("p=%p, size=%ld, end=%p, next=%p %s\n", 
                p, p->size, p + p->size, p->next, p->size!=0?"":"<-- dummy");
        p = p->next;
    } while (p != start); /* only 1 time through the list */
    mem_validate();
}

/* This is an experimental function to attempt to validate the free
 * list when coalescing is used.  It is not clear that these tests
 * will be appropriate for all designs.  If your design utilizes a different
 * approach, that is fine.  You do not need to use this function and you
 * are not required to write your own validate function.
 */
void mem_validate(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    assert(Rover->size >= 0);
    int wrapped = FALSE;
    int found_dummy = FALSE;
    int found_rover = FALSE;
    chunk_t *p, *largest, *smallest;

    /* for validate begin at Dummy */
    p = &Dummy;
    do {
        if (p->size == 0) {
            assert(found_dummy == FALSE);
            found_dummy = TRUE;
        } else {
            assert(p->size > 0);
        }
        if (p == Rover) {
            assert(found_rover == FALSE);
            found_rover = TRUE;
        }
        p = p->next;
    } while (p != &Dummy);
    assert(found_dummy == TRUE);
    assert(found_rover == TRUE);

    if (Coalescing) {
        do {
            if (p >= p->next) {
                /* this is not good unless at the one wrap */
                if (wrapped == TRUE) {
                    printf("validate: List is out of order, already found wrap\n");
                    printf("first largest %p, smallest %p\n", largest, smallest);
                    printf("second largest %p, smallest %p\n", p, p->next);
                    assert(0);   /* stop and use gdb */
                } else {
                    wrapped = TRUE;
                    largest = p;
                    smallest = p->next;
                }
            } else {
                assert(p + p->size < p->next);
            }
            p = p->next;
        } while (p != &Dummy);
        assert(wrapped == TRUE);
    }
}

/* vi:set ts=8 sts=4 sw=4 et: */

