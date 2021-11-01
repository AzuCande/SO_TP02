#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdint.h>
#include <lib.h>
#include <stddef.h>
#include <checkMemData.h>
#include <memoryDriver.h>

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

#define INFO_BLOCK_SIZE sizeof(struct infoBlock)
#define MAX_DIFF_SIZE 200 //Si un bloque supera esta diferencia entre su tamaño y el dato a guardar se lo divide
#define MIN_BYTES_REQUEST 500 //Cantidad minima de bytes pedidas al SO 

typedef struct freeMemList_t {
    struct freeMemList_t * nextFreeBlock;
    uint64_t freeBlockSize;
}freeMemList_t;

static void * const memoryPosition = (void *) 0x600000;

typedef freeMemList_t * freeMemList_p;

void initMemory();
void *mallocMemory(uint64_t size);
void freeMemory(void * free);
void insertFreeBlockOnList(freeMemList_p block);
void printMemory(char *buffer);

void mallocSyscall(uint64_t size, void** result);
void reallocSyscall(void *ptr, uint64_t newSize, void** result);

//debugger
void checkMemory(struct checkMemdata* data);
void printMem(char *str, int strSize);

#endif