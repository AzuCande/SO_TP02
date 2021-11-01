#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

// #include <stdio.h>
// #include <stdlib.h>
#include <stdint.h>
#include <listForBuddy.h>
#include <lib.h>

#define TOTAL_HEAP_SIZE (1024*1024)
#define MEM_SIZE (64*1024*1024)
#define MIN_BLOCK_SIZE 100
#define BYTE_ALIGNMENT 8
#define BYTE_ALIGNMENT_MASK 0x0007
#define ADJUSTED_HEAP_SIZE	( TOTAL_HEAP_SIZE - BYTE_ALIGNMENT )
#define POINTER_SIZE_TYPE uint32_t
#define HEADER_SIZE 8
#define MIN_ALLOC_LOG2 4
#define MAX_ALLOC_LOG2 30 // Nos fijamos despues
#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)
#define MIN_ALLOC (1<<(MIN_ALLOC_LOG2))
#define MAX_ALLOC (1<<(MAX_ALLOC_LOG2))
#define BIN_POW(x) (1<<(x))

typedef struct freeMemList_t {
    struct freeMemList_t * nextFreeBlock;
    uint64_t freeBlockSize;
}freeMemList_t;

static void * memoryPosition = (void *) 0x700000;

typedef freeMemList_t * freeMemList_p;

void initMemory();
void *mallocMemory(unsigned long sizeRequired);
void freeMemory(void * free);
void insertFreeBlockOnList(freeMemList_p block);
void printMemory(char *buffer);

#endif