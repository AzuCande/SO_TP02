// To compile: gcc -std=c99 -pedantic -Wall dummyMM.c -o dummyMM
// To execute: ./dummyMM
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <memManager.h>

#define MEM_SIZE 1024

char memory[MEM_SIZE];
char * currentMM = NULL;


// Por ahora los dos MM son iguales y son ambos un dummy de prueba, despues
// el primero va a ser el Buddy y el segundo va a ser el default


#ifdef BUDDY
/* Here starts the 'Buddy' Memory Manager */

typedef struct block_t {
    struct block_t *nextBlock;
    size_t level;
} block_t

#define HEADER_SIZE sizeof(block_t)
#define MIN_ALLOC_LOG2 4
#define MAX_ALLOC_LOG2 30 // Nos fijamos despues
#define LEVELS_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)
#define MIN_ALLOC (1<<(MIN_ALLOC_LOG2))
#define MAX_ALLOC (1<<(MAX_ALLOC_LOG2))
#define POW(x) (1<<(x))
#define SIZE_OF_BLOCK_LIST(currentLevel) (size_t) POW(MAX_ALLOC_LOG2 - (currentLevel))


// Array with pointer to block list per level
static block_t *blockListsPtr[LEVELS_COUNT];
// Array of occupied blocks
static block_t *occupiedBlocks;

void *mallocMemory(size_t sizeRequired) {
    sizeRequired += HEADER_SIZE;

    // Check if there is memory available
    if(sizeRequired > getRemainingBytes()) {
        return NULL;
    }

    size_t level = getBlockLevel(sizeRequired);
    if(level == -1) {
        return NULL;
    }

    // FIJARSE SI LO HACEMOS RECURSIVO O NO
}

// Ordered list
void insertNewBlock(void *blockToAdd, size_t level) {
    block_t * blockPtr = blockListsPtr[level];
    block_t * toAdd = (block_t *) blockToAdd;
    toAdd->level = level;

    // If there is no block in this level, add this new block as first
    if(blockPtr == NULL) {
        blockListsPtr[level] = toAdd;
        toAdd->nextBlock = NULL;
        return;
    } 

    // If memory address of toAdd is lower than the first one in the list,
    // then now toAdd is the first block
    if(toAdd < blockPtr) {
        toAdd->nextBlock = blockPtr;
        blockListsPtr[level] = toAdd;
        return;
    }

    // Else, find the position to place the new block in the list
    while(blockPtr->nextBlock != NULL && toAdd > blockPtr->nextBlock) {
        blockPtr = blockPtr->nextBlock;
    }
    if(blockPtr->nextBlock == NULL) {
        blockPtr->nextBlock = toAdd;
        toAdd->nextBlock = NULL;
    } else {
        toAdd->nextBlock = blockPtr->nextBlock;
        blockPtr->nextBlock = toAdd;
    }
}

void addOccupiedBlock(void *blockToAdd) {
    // TODO
}

size_t getBlockLevel(size_t size) {
    size_t level = 0;
    size_t totalBytes = MAX_ALLOC_LOG2;

    // If the size requested is larger than the maximum offered
    if(size > totalBytes) {
        return -1;
    }
    while(level < LEVELS_COUNT) {
        if(size < totalBytes) {
            level++;
            totalBytes <<= 1;
        } else {
            if(size == totalBytes) {
                return level;
            } else {
                return level - 1;
            }
        }
    }
    return LEVELS_COUNT;
}

void freeMemory(void * free) {

}

size_t getRemainingBytes() {
    int level = 0;
    // Find current level
    while(blockListsPtr[i] == NULL) {
        level++;
    }
    return SIZE_OF_BLOCK_LIST(level);
}
// void initMM() {
//     currentMM = memory;
// }

// void * allocateMM(size_t size) {

//     if(currentMM == NULL){
//         perror("Error memory not initialized");
//         exit(1);
//     }

//     if(size>MEM_SIZE || memory+MEM_SIZE<currentMM+size){
//         perror("Error allocating memory");
//         exit(1);
//     }

//     void * aux = currentMM;
//     currentMM += size;
//     return aux;
// }

// int main() {

//     initMM();

//     char * p1 = allocateMM(200);
//     // assert(p1 == NULL);
//     char * p2 = allocateMM(500);
//     // assert(p2 == NULL);
//     //char * p3 = allocateMM(400);

//     strcpy(p1, "Hello world");
//     assert(strcmp(p1, "Hello world") == 0);

//     strcpy(p2, p1);
//     assert(strcmp(p1, p2) == 0);
//     printf("Tests cleared\n");

//     return 0;
// }

#else
/* Here starts the non 'Buddy' (default) Memory Manager */

// void initMM() {
//     currentMM = memory;
// }

// void * allocateMM(size_t size) {

//     if(currentMM == NULL){
//         perror("Error memory not initialized");
//         exit(1);
//     }

//     if(size>MEM_SIZE || memory+MEM_SIZE<currentMM+size){
//         perror("Error allocating memory");
//         exit(1);
//     }

//     void * aux = currentMM;
//     currentMM += size;
//     return aux;
// }


#endif