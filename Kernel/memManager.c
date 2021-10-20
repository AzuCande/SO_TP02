// To compile: gcc -std=c99 -pedantic -Wall memManager.c -o memManager
// To execute: ./memManager

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

#define TOTAL_HEAP_SIZE (1024*1024)
#define MIN_BLOCK_SIZE 100
#define BYTE_ALIGNMENT 8
#define BYTE_ALIGNMENT_MASK 0x0007
#define ADJUSTED_HEAP_SIZE	( TOTAL_HEAP_SIZE - BYTE_ALIGNMENT )
#define POINTER_SIZE_TYPE uint32_t

typedef struct freeMemList_t {
    struct freeMemList_t * nextFreeBlock;
    size_t freeBlockSize;
}freeMemList_t;

typedef freeMemList_t * freeMemList_p;

void initBlocksMM();
void *mallocMemory(size_t sizeRequired);
void freeMemory(void * free);
void insertFreeBlockOnList(freeMemList_p block);

uint8_t heap[TOTAL_HEAP_SIZE];

freeMemList_p memoryManager = NULL;
freeMemList_p currentMemory = NULL;

freeMemList_p startListMem = NULL;
freeMemList_p endListMem = NULL;

static int freeListInitialised = 0;

static size_t freeRemainingBytes;
static const size_t blockStructSize = sizeof(freeMemList_t);

void insertFreeBlockOnList(freeMemList_p block) {
    size_t blockSize = block->freeBlockSize;
    freeMemList_p iterator = startListMem;

    while(iterator!=NULL && iterator->nextFreeBlock!=NULL && iterator->nextFreeBlock->freeBlockSize < blockSize) {
        iterator = iterator->nextFreeBlock;
    }

    if(iterator == NULL) { 
        //List empty
        startListMem = block;
    } else if(iterator == startListMem && iterator->freeBlockSize > blockSize) {
        //Must be the starting block
        startListMem = block;
        block->nextFreeBlock = iterator;
    } else {
        //Other cases
        block->nextFreeBlock = iterator->nextFreeBlock;
        iterator->nextFreeBlock = block;
    }

    return;
}

void *mallocMemory(size_t sizeRequired) {
    freeMemList_p currentBlock, prevBlock, newBlockLink;

    void *blockToReturn = NULL;
    
    //If this is the first call to malloc, the heap needs to be initialised in order to create the list of free blocks
    if(!freeListInitialised) {
        initBlocksMM();
        freeListInitialised = 1;
    }

    if(sizeRequired > 0) {
        sizeRequired += blockStructSize;

        // Ensures alignment of the block
        if(sizeRequired & BYTE_ALIGNMENT_MASK) {
            sizeRequired += (BYTE_ALIGNMENT - (sizeRequired & BYTE_ALIGNMENT_MASK));
        }
    }

    if(sizeRequired < freeRemainingBytes) { //antes estaba ADJUSTED_HEAP_SIZE
        prevBlock = startListMem;
        currentBlock = startListMem->nextFreeBlock;

        while((currentBlock->freeBlockSize < sizeRequired) && (currentBlock->nextFreeBlock != NULL)) {
            prevBlock = currentBlock;
            currentBlock = currentBlock->nextFreeBlock;
        }

        if(currentBlock != endListMem) {
            //Return the memory space desired
            blockToReturn = (void *) ( ( (uint8_t *) prevBlock->nextFreeBlock) + blockStructSize);

            //As this memory block will be used, it must be removed from this free memory list
            prevBlock->nextFreeBlock = currentBlock->nextFreeBlock;
            
            //If the block is larger than required, it can be split into two
            if( (currentBlock->freeBlockSize - sizeRequired) > MIN_BLOCK_SIZE) {
                //To split the block into two, a new block will be created with the number of bytes requested.
                newBlockLink = (void *) (((uint8_t *) currentBlock) + sizeRequired);

                //Calculate the size fo both blocks created from splitting the original one
                newBlockLink->freeBlockSize = currentBlock->freeBlockSize - sizeRequired;
                currentBlock->freeBlockSize = sizeRequired;

                //Insert the new block into the list of free blocks
                insertFreeBlockOnList(newBlockLink);
            }

            freeRemainingBytes -= currentBlock->freeBlockSize;
        }
    }

    return blockToReturn;
    
}


void initBlocksMM() {
    freeMemList_p firstFreeBlock;
    uint8_t *alignedHeapPtr;

    // Ensures that the heap starts on a correct aligned boundary
    alignedHeapPtr = (uint8_t *) ( ( (POINTER_SIZE_TYPE) &heap[BYTE_ALIGNMENT]) & ( ~( ( POINTER_SIZE_TYPE ) BYTE_ALIGNMENT_MASK ) ) );

    // startListMem is used to refer to the first item in the list of free blocks
    startListMem->nextFreeBlock = (void *) alignedHeapPtr;
    startListMem->freeBlockSize = (size_t) 0;

    // endListMem is used to mark the end of the list of free blocks
    endListMem->freeBlockSize = ADJUSTED_HEAP_SIZE;
    endListMem->nextFreeBlock = NULL;

    // Prepares a single first block in the freeMem List
    firstFreeBlock = (void *) alignedHeapPtr;
    firstFreeBlock->freeBlockSize = ADJUSTED_HEAP_SIZE;
    firstFreeBlock->nextFreeBlock = endListMem;

    freeRemainingBytes = firstFreeBlock->freeBlockSize;
}

void freeMemory(void * free){
    uint8_t *toFree = (uint8_t *) free;
    freeMemList_p toAddFreeBlock;

    if(toFree != NULL) {
        //The memory space to be freed will have a freeBlocksMemList_t structure before it
        toFree -= blockStructSize;

        //Cast to freeBlocksMemList_t * to avoid compilation issues
        toAddFreeBlock = (freeMemList_t *) toFree;

        //Add this new block to the list of free blocks
        insertFreeBlockOnList(toAddFreeBlock);
    }
}

// int main(){


//     void * m1 = mallocMemory(100);

//     freeMemory(m1);
//     return 0;
// }