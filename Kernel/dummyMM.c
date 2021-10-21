// To compile: gcc -std=c99 -pedantic -Wall dummyMM.c -o dummyMM
// To execute: ./dummyMM
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <memManager.h>
#include <listForBuddy.h>

#define MEM_SIZE 1024

char memory[MEM_SIZE];
char * currentMM = NULL;


// Por ahora los dos MM son iguales y son ambos un dummy de prueba, despues
// el primero va a ser el Buddy y el segundo va a ser el default


#ifdef BUDDY
/* Here starts the 'Buddy' Memory Manager */

#define HEADER_SIZE 8
#define MIN_ALLOC_LOG2 4
#define MAX_ALLOC_LOG2 30 // Nos fijamos despues
#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)
#define MIN_ALLOC (1<<(MIN_ALLOC_LOG2))
#define MAX_ALLOC (1<<(MAX_ALLOC_LOG2))
#define BIN_POW(x) (1<<(x))
#define HEAP_SIZE (1024 * 1024)

// Stores the free list for an allocation size
static block_t *buckets[BUCKET_COUNT];
// Any leaf node is disregarded, parent nodes are taken into account
static uint64_t nodeIsSplit[(BIN_POW((BUCKET_COUNT - 1))) / 8];
static uint64_t bucketLimit = BUCKET_COUNT - 1;
static uint64_t *base;
static uint64_t *maxHeap;
static uint64_t totalMemUsed;

void initMemory(void *initAddress) {
    base = initAddress;
    if (base == NULL) {
        return;
    }
    maxHeap = (base + HEAP_SIZE);
    listInit(&buckets[BUCKET_COUNT - 1]);
    listPush(&buckets[BUCKET_COUNT - 1], (listBuddy *) base);
    totalMemUsed = 0;
}

void *mallocMemory(size_t sizeRequested) {
    uint64_t originalBucket, bucket;
    if (sizeRequested + HEADER_SIZE > MAX_ALLOC) {
        return NULL;
    }
    bucket = bucketForRequest(sizeRequested + HEADER_SIZE);
    originalBucket = bucket;
    while (bucket + 1 != 0) {
        uint64_t size, bytesNeeded, i;
        uint64_t *ptr;
        if (!lowerBucketLimit(bucket)) {
            return NULL;
        }
        ptr = (uint64_t *) listPop(&buckets[bucket]);
        if (!ptr) {
            if (bucket != bucketLimit || bucket == 0) {
                bucket--;
                continue;
            }
            if (!lowerBucketLimit(bucket - 1)) {
                return NULL;
            }
            ptr = (uint64_t *) listPop(&buckets[bucket]);
        }
        size = (uint64_t) 1 << (MAX_ALLOC_LOG2 - bucket);
        bytesNeeded = bucket < originalBucket ? size / 2 + sizeof(listBuddy) : size;
        if (checkMaxPointer(ptr + bytesNeeded)) {
            listPush(&buckets[bucket], (listBuddy *) ptr);
            return NULL;
        }
        i = nodeForPointer(ptr, bucket);
        if (i != 0) {
            flipParentIsSplit(i);
        }
        while (bucket < originalBucket) {
            i = i * 2 + 1;
            bucket++;
            flipParentIsSplit(i);
            listPush(&buckets[bucket], (listBuddy *) pointerForNode(i + 1, bucket));
        }
        *(uint64_t *) ptr = sizeRequested;
        totalMemUsed += (BIN_POW(MAX_ALLOC_LOG2 - bucket));
        return ptr + HEADER_SIZE;
    }
    return NULL;
}

void freeMemory(void *free) {
    uint64_t bucket;
    uint64_t i;
    if (free == NULL || checkMaxPointer(free) || (uint64_t *) free < base) {
        return;
    }
    free = (uint64_t *) free - HEADER_SIZE;
    bucket = bucketForRequest(*(uint64_t *) free + HEADER_SIZE);
    totalMemUsed -= (BIN_POW(MAX_ALLOC_LOG2 - bucket));
    i = nodeForPointer((uint64_t *) free, bucket);
    while (i != 0) {
        flipParentIsSplit(i);
        if (parentIsSplit(i) || bucket == bucketLimit) {
            break;
        }
        listRemove((listBuddy *) pointerForNode(((i - 1) ^ 1) + 1, bucket));
        i = (i - 1) / 2;
        bucket--;
    }
    listPush(&buckets[bucket], (listBuddy *) pointerForNode(i, bucket));
}

static int checkMaxPointer(uint64_t *value) {
    if (value >= maxHeap || value < base) {
        return 1;
    }
    return 0;
}

// Given the index of a node, this returns the "is split" flag of the parent
static int parentIsSplit(uint64_t index) {
    index = (index - 1) / 2;
    return (nodeIsSplit[index / 8] >> (index % 8)) & 1;
}

// Given the index of a node, this flips the "is split" flag of the parent
static void flipParentIsSplit(uint64_t index) {
    index = (index - 1) / 2;
    nodeIsSplit[index / 8] ^= 1 << (index % 8);
}

// Given the requested size passed to "malloc", this function returns the index of the smallest bucket that can fit that size.
static uint64_t bucketForRequest(uint64_t sizeRequested) {
    uint64_t bucket = BUCKET_COUNT - 1;
    uint64_t size = MIN_ALLOC;
    while (size < sizeRequested) {
        bucket--;
        size *= 2;
    }
    return bucket;
}

static int lowerBucketLimit(uint64_t list) {
    while (list < bucketLimit) {
        uint64_t root = nodeForPointer(base, bucketLimit);
        uint64_t *rightChild;
        if (!parentIsSplit(root)) {
            listRemove((listBuddy *) base);
            initList(&buckets[--bucketLimit]);
            listPush(&buckets[bucketLimit], (listBuddy *) base);
            continue;
        }
        rightChild = pointerForNode(root + 1, bucketLimit);
        if (checkMaxPointer(rightChild + sizeof(listBuddy))) {
            return 0;
        }
        listPush(&buckets[bucketLimit], (listBuddy *) rightChild);
        initList(&buckets[--bucketLimit]);

        root = (root - 1) / 2;
        if (root != 0) {
            flipParentIsSplit(root);
        }
    }
    return 1;
}

static uint64_t *pointerForNode(uint64_t idx, uint64_t bucket) {
    return base + ((idx - (BIN_POW(bucket)) + 1) << (MAX_ALLOC_LOG2 - bucket));
}

static uint64_t nodeForPointer(uint64_t *ptr, uint64_t bucket) {
    return ((ptr - base) >> (MAX_ALLOC_LOG2 - bucket)) + (1 << bucket) - 1;
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