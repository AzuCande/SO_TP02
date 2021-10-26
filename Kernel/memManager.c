// To compile: gcc -std=c99 -pedantic -Wall dummyMM.c -o dummyMM
// To execute: ./dummyMM
#include <memManager.h>

#ifdef BUDDY
/* Here starts the 'Buddy' Memory Manager */

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
    maxHeap = (base + TOTAL_HEAP_SIZE);
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

uint8_t heap[TOTAL_HEAP_SIZE];

freeMemList_p memoryManager = NULL;
freeMemList_p currentMemory = NULL;

static freeMemList_p startListMem, endListMem;

static int freeListInitialised = 0;

static size_t freeRemainingBytes;
static const size_t blockStructSize = sizeof(freeMemList_t);

// Insert a free block in a list ordered by size (from smaller to bigger size)
#define insertFreeBlockOnList(freeMemList_p blockToInsert) {
    freeMemList_t *iterator;
    unsigned int blockSize;

    // Iterate through the list until a block is found that has a larger size
    // than the block we are inserting
    blockSize = blockToInsert->freeBlockSize;
    for(iterator = startListMem; iterator->nextFreeBlock->freeBlockSize < blockSize; iterator = iterator->nextFreeBlock) {
        // Just iterate to the correct position
    }

    // Update the list to include the block being inserted in the correct position
    blockToInsert->nextFreeBlock = iterator->nextFreeBlock;
    iterator->nextFreeBlock = blockToInsert;
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