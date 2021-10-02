// To compile: gcc -std=c99 -pedantic -Wall dummyMM.c -o dummyMM
// To execute: ./dummyMM

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TOTAL_HEAP_SIZE (1024*1024)
#define MIN_BLOCK_SIZE 100
#define BYTE_ALIGNMENT 8
#define BYTE_ALIGNMENT_MASK 0x0007
#define ADJUSTED_HEAP_SIZE	( TOTAL_HEAP_SIZE - BYTE_ALIGNMENT )
#define POINTER_SIZE_TYPE uint32_t

typedef struct freeBlocksInMemList_t {
    struct freeBlocksInMemList_t * nextFreeBlock;
    size_t freeBlockSize;
} freeBlocksInMemList_t;

freeBlocksInMemList_t memoryManager = NULL;
freeBlocksInMemList_t currentMemory = NULL;

freeBlocksInMemList_t startListMem = NULL;
freeBlocksInMemList_t endListMem = NULL;

static size_t freeRemainingBytes;
static const size_t blockStructSize = sizeof(freeBlocksInMemList_t);

void insertFreeBlockOnList(freeBlocksInMemList_t * block) {
    size_t blockSize = block.freeBlockSize;
    freeBlocksInMemList_t * iterator = startListMem;

    while(iterator!=NULL && iterator->nextFreeBlock!=NULL && iterator->nextFreeBlock->freeBlockSize < blockSize) {
        iterator = iterator->nextFreeBlock;
    }

    if(iterator == NULL) { 
        //List empty
        startListMem = block;
    } else if(iterator == startListMem && iterator->freeBlockSize > blockSize) {
        //Must be starting block
        startListMem = block;
        block->nextFreeBlock = iterator;
    } else {
        //Other cases
        block->nextFreeBlock = iterator->nextFreeBlock;
        iterator->nextFreeBlock = block;
    }

    return;
}

void *malloc(size_t sizeRequired) {
    freeBlocksInMemList_t *currentBlock, *prevBlock, *newBlockLink;
    static int freeListInitialised = 0;
    void *blockToReturn = NULL;
    
    //If this is the first call to malloc, the heap needs to be initialised in order to create the list of free blocks
    if(!freeListInitialised) {
        initBlocksMM();
        heapInitialised = 1;
    }

    if(sizeRequired > 0) {
        sizeRequired += blockStructSize;
    }

    if(sizeRequired < configADJUSTED_HEAP_SIZE) {
        prevBlock = &startListMem;
        currentBlock = startListMem.nextFreeBlock;

        while((currentBlock->freeBlockSize < sizeRequired) && (currentBlock->nextFreeBlock != NULL)) {
            prevBlock = currentBlock;
            currentBlock = currentBlock->nextFreeBlock;
        }

        if(currentBlock != &endListMem) {
            //Return the memory space desired
            blockToReturn = (void *) ( ( (uint8_t *) prevBlock->nextFreeBlock) + blockStructSize);

            //As this memory block will be used, it must be removed from this free memory list
            prevBlock->nextFreeBlock = currentBlock->nextFreeBlock;
            
            //If the block is larger than required, it can be split into two
            if( (currentBlock->freeBlockSize - sizeRequired) > MIN_BLOCK_SIZE) {
                //To split the block into two, a new block will be created with the number of bytes requested.
                newBlockLink = (void *) (((uint8_t) currentBlock) + sizeRequired);

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


void initBlocksMM(void) {
    freeBlocksInMemList_t *firstFreeBlock;
    uint8_t *alignedHeapPtr;

    //FALTA
}

void free(void * free){
    uint8_t *toFree = (uint8_t *) free;
    freeBlocksInMemList_t *toAddFreeBlock;

    if(toFree != NULL) {
        //The memory space to be freed will have a freeBlocksMemList_t structure before it
        toFree -= blockStructSize;

        //Cast to freeBlocksMemList_t * to avoid compilation issues
        toAddFreeBlock = (freeBlocksInMemList_t *) toFree;

        //Add this new block to the list of free blocks
        insertFreeBlockOnList(toAddFreeBlock);
    }
}

void * allocateMM(size_t size) {

    if(currentMM == NULL){
        perror("Error memory not initialized");
        exit(1);
    }

    if(size>MEM_SIZE || memory+MEM_SIZE<currentMM+size){
        perror("Error allocating memory");
        exit(1);
    }

    void * aux = currentMM;
    currentMM += size;
    return aux;
}

int main() {

    initMM();

    char * p1 = allocateMM(200);
    char * p2 = allocateMM(500);
    //char * p3 = allocateMM(400);

    return 0;
}