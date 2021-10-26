#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <listForBuddy.h>

#define TOTAL_HEAP_SIZE (1024*1024)
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
    size_t freeBlockSize;
}freeMemList_t;

typedef freeMemList_t * freeMemList_p;

void initBlocksMM();
void *mallocMemory(size_t sizeRequired);
void freeMemory(void * free);
void insertFreeBlockOnList(freeMemList_p block);

#endif