#include <memManager.h>

#ifdef BUDDY
/* Here starts the 'Buddy' Memory Manager */

// #include <memManager.h>
#include <listForBuddy.h>

// Stores the free list for an allocation size
static block_t *buckets[BUCKET_COUNT];
// Any leaf node is disregarded, parent nodes are taken into account
static uint64_t nodeIsSplit[(BIN_POW((BUCKET_COUNT - 1))) / 8];
static uint64_t bucketLimit = BUCKET_COUNT - 1;
static uint64_t *base;
static uint64_t *maxHeap;
static uint64_t totalMemUsed;

void initMemory(void *initAddress)
{
    base = initAddress;
    if (base == NULL)
    {
        return;
    }
    maxHeap = (base + TOTAL_HEAP_SIZE);
    listInit(&buckets[BUCKET_COUNT - 1]);
    listPush(&buckets[BUCKET_COUNT - 1], (listBuddy *)base);
    totalMemUsed = 0;
}

void *mallocMemory(uint64_t sizeRequested)
{
    uint64_t originalBucket, bucket;
    if (sizeRequested + HEADER_SIZE > MAX_ALLOC)
    {
        return NULL;
    }
    bucket = bucketForRequest(sizeRequested + HEADER_SIZE);
    originalBucket = bucket;
    while (bucket + 1 != 0)
    {
        uint64_t size, bytesNeeded, i;
        uint64_t *ptr;
        if (!lowerBucketLimit(bucket))
        {
            return NULL;
        }
        ptr = (uint64_t *)listPop(&buckets[bucket]);
        if (!ptr)
        {
            if (bucket != bucketLimit || bucket == 0)
            {
                bucket--;
                continue;
            }
            if (!lowerBucketLimit(bucket - 1))
            {
                return NULL;
            }
            ptr = (uint64_t *)listPop(&buckets[bucket]);
        }
        size = (uint64_t)1 << (MAX_ALLOC_LOG2 - bucket);
        bytesNeeded = bucket < originalBucket ? size / 2 + sizeof(listBuddy) : size;
        if (checkMaxPointer(ptr + bytesNeeded))
        {
            listPush(&buckets[bucket], (listBuddy *)ptr);
            return NULL;
        }
        i = nodeForPointer(ptr, bucket);
        if (i != 0)
        {
            flipParentIsSplit(i);
        }
        while (bucket < originalBucket)
        {
            i = i * 2 + 1;
            bucket++;
            flipParentIsSplit(i);
            listPush(&buckets[bucket], (listBuddy *)pointerForNode(i + 1, bucket));
        }
        *(uint64_t *)ptr = sizeRequested;
        totalMemUsed += (BIN_POW(MAX_ALLOC_LOG2 - bucket));
        return ptr + HEADER_SIZE;
    }
    return NULL;
}

void freeMemory(void *free)
{
    uint64_t bucket;
    uint64_t i;
    if (free == NULL || checkMaxPointer(free) || (uint64_t *)free < base)
    {
        return;
    }
    free = (uint64_t *)free - HEADER_SIZE;
    bucket = bucketForRequest(*(uint64_t *)free + HEADER_SIZE);
    totalMemUsed -= (BIN_POW(MAX_ALLOC_LOG2 - bucket));
    i = nodeForPointer((uint64_t *)free, bucket);
    while (i != 0)
    {
        flipParentIsSplit(i);
        if (parentIsSplit(i) || bucket == bucketLimit)
        {
            break;
        }
        listRemove((listBuddy *)pointerForNode(((i - 1) ^ 1) + 1, bucket));
        i = (i - 1) / 2;
        bucket--;
    }
    listPush(&buckets[bucket], (listBuddy *)pointerForNode(i, bucket));
}

static int checkMaxPointer(uint64_t *value)
{
    if (value >= maxHeap || value < base)
    {
        return 1;
    }
    return 0;
}

// Given the index of a node, this returns the "is split" flag of the parent
static int parentIsSplit(uint64_t index)
{
    index = (index - 1) / 2;
    return (nodeIsSplit[index / 8] >> (index % 8)) & 1;
}

// Given the index of a node, this flips the "is split" flag of the parent
static void flipParentIsSplit(uint64_t index)
{
    index = (index - 1) / 2;
    nodeIsSplit[index / 8] ^= 1 << (index % 8);
}

// Given the requested size passed to "malloc", this function returns the index of the smallest bucket that can fit that size.
static uint64_t bucketForRequest(uint64_t sizeRequested)
{
    uint64_t bucket = BUCKET_COUNT - 1;
    uint64_t size = MIN_ALLOC;
    while (size < sizeRequested)
    {
        bucket--;
        size *= 2;
    }
    return bucket;
}

static int lowerBucketLimit(uint64_t list)
{
    while (list < bucketLimit)
    {
        uint64_t root = nodeForPointer(base, bucketLimit);
        uint64_t *rightChild;
        if (!parentIsSplit(root))
        {
            listRemove((listBuddy *)base);
            initList(&buckets[--bucketLimit]);
            listPush(&buckets[bucketLimit], (listBuddy *)base);
            continue;
        }
        rightChild = pointerForNode(root + 1, bucketLimit);
        if (checkMaxPointer(rightChild + sizeof(listBuddy)))
        {
            return 0;
        }
        listPush(&buckets[bucketLimit], (listBuddy *)rightChild);
        initList(&buckets[--bucketLimit]);

        root = (root - 1) / 2;
        if (root != 0)
        {
            flipParentIsSplit(root);
        }
    }
    return 1;
}

static uint64_t *pointerForNode(uint64_t idx, uint64_t bucket)
{
    return base + ((idx - (BIN_POW(bucket)) + 1) << (MAX_ALLOC_LOG2 - bucket));
}

static uint64_t nodeForPointer(uint64_t *ptr, uint64_t bucket)
{
    return ((ptr - base) >> (MAX_ALLOC_LOG2 - bucket)) + (1 << bucket) - 1;
}

/* ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
*/


#else

static char * current = (char*)0x600000;

void initMemory() {
    return;
} 

void *mallocMemory(uint64_t size)
{
    void *ans = (void *)current;
    current += size;
    return ans;
}

void mallocSyscall(uint64_t size, void **result)
{
    (*result) = mallocMemory(size);
}


void freeMemory(void * p) 
{
    return;
}

#endif

// #else
// /* Here starts the non 'Buddy' (default) Memory Manager */

// // #include <memManager.h>

// //INFO_BLOCK_SIZE = 32 bytes
// void *firstInfoBlock;
// void *memorySize; //(memorySize-firstInfoBlock) tamaño total de la memoria
// void *memoryDim;  //(memoryDim-firstInfoBlock) bytes usados

// int index = 0; //variable auxiliar del printMem

// struct infoBlock
// {
//     uint64_t size; //12 bytes
//     char free;
//     struct infoBlock *next;
//     struct infoBlock *previous;
// };

// typedef struct infoBlock *infoBlockPtr;

// //private:
// infoBlockPtr findFreeBlock(infoBlockPtr *last, uint64_t size);
// infoBlockPtr requestSpace(infoBlockPtr last, uint64_t size);
// void splitBlock(infoBlockPtr block, uint64_t size);
// infoBlockPtr getBlockPtr(void *ptr);
// void *syscallManager(uint64_t size);

// void initMemory()
// {
//     firstInfoBlock = NULL;
//     memorySize = NULL;
//     memoryDim = NULL;
// }

// void *mallocMemory(uint64_t size)
// {
//     if (size <= 0)
//         return NULL;

//     infoBlockPtr block;
//     if (firstInfoBlock == NULL)
//     { //primer llamado
//         block = requestSpace(NULL, size);
//         if (block == NULL)
//             return NULL;
//         firstInfoBlock = block;
//     }
//     else
//     {
//         infoBlockPtr last = firstInfoBlock;
//         block = findFreeBlock(&last, size);
//         if (block == NULL)
//         {                                     //no se encontro un bloque
//             block = requestSpace(last, size); //agrega un nuevo bloque al final
//             if (block == NULL)
//                 return NULL;
//         }
//         else
//         { //se encontro un boque
//             block->free = 0;
//             if ((int)block->size - (int)size - (int)INFO_BLOCK_SIZE > (int)MAX_DIFF_SIZE) //si es muy grande lo divido
//                 splitBlock(block, size);                                                  //no esta testiado
//         }
//     }
//     return (block + 1); //la direccion justo despues de infoBlock
// }

// void splitBlock(infoBlockPtr block, uint64_t size)
// {
//     infoBlockPtr newBlock = (void *)(block + 1) + size;
//     newBlock->free = 1;
//     newBlock->size = (block->size) - size - INFO_BLOCK_SIZE;
//     newBlock->previous = block;
//     newBlock->next = block->next;

//     if (block->next != NULL)
//         block->next->previous = newBlock;

//     block->next = newBlock;
//     block->size = size;
//     return;
// }

// infoBlockPtr findFreeBlock(infoBlockPtr *last, uint64_t size)
// {
//     infoBlockPtr current = firstInfoBlock;
//     infoBlockPtr blockAptSize = NULL;
//     uint64_t aptSize = SIZE_MAX;
//     while (current != NULL)
//     {
//         if (current->free && current->size >= size && current->size < aptSize)
//         {
//             if (current->size == size)
//                 return current; // current tiene el tamaño mas apto
//             aptSize = current->size;
//             blockAptSize = current;
//         }
//         *last = current;
//         current = current->next;
//     }
//     if (blockAptSize != NULL)
//         return blockAptSize;
//     return current;
// }

// infoBlockPtr requestSpace(infoBlockPtr last, uint64_t size)
// {
//     infoBlockPtr block = syscallManager(size + INFO_BLOCK_SIZE);
//     if (block == NULL) //no hay mas espacio en el heap
//         return NULL;

//     if (last != NULL)
//     { // si last==NULL es el primer llamado
//         last->next = block;
//     }
//     block->size = size;
//     block->free = 0;
//     block->next = NULL;
//     block->previous = last;

//     return block;
// }

// void *syscallManager(uint64_t size)
// {
//     if (memoryDim == NULL)
//     { //primer llamado
//         memorySize = sbrk(0);
//         memoryDim = memorySize;
//     }
//     void *resutl = memoryDim;
//     if (memorySize - memoryDim < size)
//     {
//         void *check = NULL;
//         if (size <= MIN_BYTES_REQUEST)
//         {
//             check = sbrk(MIN_BYTES_REQUEST);
//         }
//         else
//         {
//             check = sbrk(size + MIN_BYTES_REQUEST);
//         }
//         if (check == NULL)
//             return NULL; //no hay mas espacio en el heap
//         memorySize = check;
//         memoryDim += size; //ver si funciona bien
//     }
//     else
//         memoryDim += size;
//     return resutl;
// }

// infoBlockPtr getBlockPtr(void *ptr)
// {
//     return (infoBlockPtr)ptr - 1;
// }

// void freeMemory(void *ptr)
// {
//     if (ptr == NULL || ptr < getStartMemory())
//         return;
//     infoBlockPtr current = getBlockPtr(ptr);
//     current->free = 1;
//     infoBlockPtr aux = current->next;
//     if (aux != NULL && aux->free)
//     { //el siguiente esta libre
//         current->size += aux->size + INFO_BLOCK_SIZE;
//         if (aux->next != NULL)
//             aux->next->previous = current;
//         current->next = aux->next;
//         aux->next = NULL;
//         aux->previous = NULL;
//     }
//     aux = current->previous;
//     if (aux != NULL && aux->free)
//     { //el anterior esta libre
//         aux->size += current->size + INFO_BLOCK_SIZE;
//         if (current->next != NULL)
//             current->next->previous = aux;
//         aux->next = current->next;
//         current->next = NULL;
//         current->previous = NULL;
//     }
//     return;
// }

// void mallocSyscall(uint64_t size, void **result)
// {
//     (*result) = mallocMemory(size);
// }

// /*
//   (*) No puede haber dos free juntos
//   (*) distancia entre bloques - size - INFO_BLOCK_SIZE < MAX_DIFF_SIZE
//   (*) memoryDim-firstInfoBlock = la sumatoria de "block->next - block" en todos
//   los bloques
//   (*) en todo los bloques pasa que, current = current->next->previous
//   (*) currentMemoryLimit = memorySize
//   (*) firstInfoBlock = startMemory
//   (*) totalBytes es igual a la suma de bytesUsedByBLocks, bytesUsedByUser,
//   unusedBytes y bytesUsedByAlign
//   (*) numeberOfBlocks es igual a los bloque libre y usados
// */
// void checkMemory(struct checkMemdata *data)
// {
//     infoBlockPtr current = firstInfoBlock;
//     int freeFlag = 0;

//     while (current != NULL)
//     {
//         data->numeberOfBlocks++;
//         if (current->free)
//         {
//             data->freeBlock++;
//         }
//         else
//         {
//             data->blockused++;
//         }
//         data->bytesUsedByBLocks += INFO_BLOCK_SIZE;

//         if (current->next != NULL)
//         {
//             data->lostBytes += (long)current->next - (long)current - (int)current->size - INFO_BLOCK_SIZE;
//             data->totalBytes += (long)current->next - (long)current;
//         }
//         else
//         {
//             data->lostBytes += (long)memoryDim - (long)current - (int)current->size - INFO_BLOCK_SIZE;
//             data->totalBytes += (long)memoryDim - (long)current;
//         }
//         if (current->free)
//         { // no puede haber dos free juntos
//             data->freeBytes += current->size;
//             if (freeFlag)
//             {
//                 data->freeBlocksTogether++;
//                 data->numError++;
//             }
//             else
//                 freeFlag = 1;
//         }
//         else
//         {
//             data->bytesUsedByUser += current->size;
//             freeFlag = 0;
//         }
//         if (current->next != NULL)
//         {
//             long notUsed = (long)current->next - (long)current - (int)current->size - INFO_BLOCK_SIZE;
//             if (notUsed > MAX_DIFF_SIZE)
//             {
//                 data->lostBytes += (notUsed - MAX_DIFF_SIZE);
//                 data->numError++;
//             }
//             if (current != current->next->previous)
//             {
//                 data->curNextPrev++;
//                 data->numError++;
//             }
//         }
//         current = current->next;
//     }
//     if (data->totalBytes != (data->bytesUsedByBLocks + data->bytesUsedByUser + data->lostBytes + data->freeBytes))
//     {
//         data->bytesError = 1;
//         data->numError++;
//     }
//     if (data->numeberOfBlocks != data->blockused + data->freeBlock)
//     {
//         data->numblocksError = 1;
//         data->numError++;
//     }
//     if (memoryDim - firstInfoBlock != data->totalBytes)
//     {
//         data->memError = 1;
//         data->numError++;
//     }
//     return;
// }

// void printMem(char * buffer, int bufferLength)
// {
//     int i = 0;
//     bufferLength--; //reservo el lugar del \n

//     char *header = "\nBlock\t Free\t Size\t Address\n";
//     strcat(buffer, header, &i);

//     infoBlockPtr current = firstInfoBlock;

//     //avanzo hasta donde imprimi la ultima vez
//     for (int j = 0; j < index && current != NULL; j++)
//         current = current->next;

//     while (current != NULL && i < bufferLength)
//     {

//         char aux[10];

//         intToString(aux, index);
//         strcat(buffer, aux, &i);

//         if (current->free)
//             strcat(buffer, "Y", &i);
//         else
//             strcat(buffer, "N",  &i);

//         intToString(aux, current->size);
//         strcat(buffer, bufferLength, &i);

//         intToBaseString(16, aux, (unsigned long long)(current + 1));
//         strcat(buffer, aux, &i);

//         current = current->next;
//         buffer[i++] = '\n';
//     }

//     if (current == NULL && i != bufferLength)
//         index = 0;
//     if (i == bufferLength)
//         index--;

//     buffer[i] = '\0';
// }

// #endif

/* ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
*/

// LO DE JUAN

// #define NULL 0 // TODO solo usar el NULL de stddef

// typedef long Align;
// typedef union header Header;

// union header
// {
//       struct
//       {
//             union header *ptr;
//             unsigned size;
//       } data;
//       Align x;
// };

// static Header *base;
// static Header *startingNode = NULL;

// unsigned long totalUnits;

// void initMemory()
// {
//       // Initially its all a very large block
//       totalUnits = (MEM_SIZE + sizeof(Header) - 1) / sizeof(Header) + 1;
//       startingNode = base = (Header *)memoryPosition;
//       startingNode->data.size = totalUnits;
//       startingNode->data.ptr = startingNode;
// }

// // Ref for malloc/free : The C Programming Language  - K&R
// void *mallocMemory(unsigned long nbytes)
// {
//       if (nbytes == 0)
//             return NULL;

//       unsigned long nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1; //Normalize to header units

//       Header *currNode, *prevNode;
//       prevNode = startingNode;

//       for (currNode = prevNode->data.ptr;; prevNode = currNode, currNode = currNode->data.ptr)
//       {
//             if (currNode->data.size >= nunits)
//             {
//                   if (currNode->data.size == nunits) // Equal just use
//                         prevNode->data.ptr = currNode->data.ptr;
//                   else
//                   {
//                         currNode->data.size -= nunits;
//                         currNode += currNode->data.size;
//                         currNode->data.size = nunits;
//                   }
//                   startingNode = prevNode;
//                   return (void *)(currNode + 1); //Return new memspace WITHOUT header
//             }
//             if (currNode == startingNode)
//                   return NULL;
//       }
// }

// void freeMemory(void *freeMem)
// {
//       if (freeMem == NULL || (((long)freeMem - (long)base) % sizeof(Header)) != 0)
//             return;

//       Header *freeBlock, *currNode;
//       freeBlock = (Header *)freeMem - 1; //Add header to mem to free

//       if (freeBlock < base || freeBlock >= (base + totalUnits * sizeof(Header)))
//             return;

//       char isExternal = 0;

//       for (currNode = startingNode; !(freeBlock > currNode && freeBlock < currNode->data.ptr); currNode = currNode->data.ptr)
//       {

//             if (freeBlock == currNode || freeBlock == currNode->data.ptr)
//                   return;

//             if (currNode >= currNode->data.ptr && (freeBlock > currNode || freeBlock < currNode->data.ptr))
//             {
//                   isExternal = 1;
//                   break;
//             }
//       }

//       if (!isExternal && (currNode + currNode->data.size > freeBlock || freeBlock + freeBlock->data.size > currNode->data.ptr)) //Absurd!!
//             return;

//       if (freeBlock + freeBlock->data.size == currNode->data.ptr) //Join right
//       {
//             freeBlock->data.size += currNode->data.ptr->data.size;
//             freeBlock->data.ptr = currNode->data.ptr->data.ptr;
//       }
//       else
//             freeBlock->data.ptr = currNode->data.ptr;

//       if (currNode + currNode->data.size == freeBlock) //Join left
//       {
//             currNode->data.size += freeBlock->data.size;
//             currNode->data.ptr = freeBlock->data.ptr;
//       }
//       else
//             currNode->data.ptr = freeBlock;

//       startingNode = currNode;
// }

// void dumpMM()
// {
//       long long idx = 1;
//       Header *original, *current;
//       original = current = startingNode;
//       int flag = 1;

//       print("\nMEMORY DUMP (Free List)\n");
//       print("- - Units of 16 bytes\n");
//       print("------------------------------------------------\n");
//       print("Total memory: %d bytes\n\n", totalUnits * sizeof(Header));
//       if (startingNode == NULL)
//             print("    No free blocks\n");
//       print("Free blocks: \n");
//       print("-------------------------------\n");
//       while (current != original || flag)
//       {
//             flag = 0;
//             print("    Block number %d\n", idx);
//             print("        Base: %x\n", (uint64_t)current);
//             print("        Free units: %d\n", current->data.size);
//             print("-------------------------------\n");
//             current = current->data.ptr;
//             idx++;
//       }
//       print("\n\n");
// }

// lo nuestro:

// static void * memoryPosition = (void *) 0x600000;

// typedef long Align;

// union header {
//     struct {
//         union header *ptr;
//         unsigned size;
//     } allocation;
//     Align x;
// };

// typedef union header Header;

// // Empty list
// static Header *base;
// // Free list start
// static Header *headBlock = NULL;

// unsigned long maxSize;

// void initMemory() {
//     maxSize = (MEM_SIZE + sizeof(Header) - 1) / sizeof(Header) + 1;
//     headBlock = base = (Header *)memoryPosition;
//     headBlock->allocation.size = maxSize;
//     headBlock->allocation.ptr = headBlock;
// }

// void * mallocMemory(unsigned long size) {

//     // Check the size requested
//     if (size == 0) {
//         return NULL;
//     }

//     Header *currentBlock, *prevBlock;
//     // prevBlock = headBlock;
//     unsigned long nunits;

//     nunits = (size + sizeof(Header) - 1) / sizeof(Header) + 1;

//     if ((prevBlock = headBlock) == NULL) { /* no hay lista libre aún */
//         base->allocation.ptr = headBlock = prevBlock = &base;
//         base->allocation.size = 0;
//     }

//     for(currentBlock = prevBlock->allocation.ptr; ; prevBlock = currentBlock, currentBlock = currentBlock->allocation.ptr) {
//         if(currentBlock->allocation.size >= nunits) {
//             // Exact size
//             if(currentBlock->allocation.size == nunits) {
//                 prevBlock->allocation.ptr = currentBlock->allocation.ptr;
//             } else {
//                 currentBlock->allocation.size -= nunits;
//                 currentBlock += currentBlock->allocation.size;
//                 currentBlock->allocation.size = nunits;
//             }
//             headBlock = prevBlock;
//             return (void *) (currentBlock + 1);
//         }
//         if(currentBlock == headBlock) {
//             return NULL;
//         }
//     }
// }

// void freeMemory(void * free) {

//     // Check argument
//     if(free == NULL) {
//         return;
//     }

//     Header *freeBlock, *currentBlock;

//     freeBlock = (Header *) free - 1;

//     if(freeBlock < base || freeBlock >= (base + maxSize * sizeof(Header))) {
//         return;
//     }

//     for(currentBlock = headBlock; !(freeBlock > currentBlock && freeBlock < currentBlock->allocation.ptr);
//     currentBlock = currentBlock->allocation.ptr) {

//         // Free first or last block
//         if(currentBlock >= currentBlock->allocation.ptr && (freeBlock > currentBlock || freeBlock < currentBlock->allocation.ptr)) {
//             break;
//         }
//     }

//     if(freeBlock + freeBlock->allocation.size == currentBlock->allocation.ptr) {
//         freeBlock->allocation.size += currentBlock->allocation.ptr->allocation.size;
//         freeBlock->allocation.ptr = currentBlock->allocation.ptr->allocation.ptr;
//     } else {
//         freeBlock->allocation.ptr = currentBlock->allocation.ptr;
//     }
//     if(currentBlock + currentBlock->allocation.size == freeBlock) {
//         currentBlock->allocation.size += freeBlock->allocation.size;
//         currentBlock->allocation.ptr = freeBlock->allocation.ptr;
//     } else {
//         currentBlock->allocation.ptr = freeBlock;
//     }
//     headBlock = currentBlock;
// }

// A PARTIR DE ACA SON IMPLEMENTACIONES QUE NO ANDUVIERON

// static freeMemList_p startListMem, endListMem;

// static int freeListInitialised = 0;

// static uint64_t freeRemainingBytes;
// static const uint64_t blockStructSize = sizeof(freeMemList_t);

// // Insert a free block in a list ordered by size (from smaller to bigger size)
// void insertFreeBlockOnList(freeMemList_p blockToInsert) {
//     freeMemList_t *iterator;
//     uint64_t blockSize;

//     // Iterate through the list until a block is found that has a larger size
//     // than the block we are inserting
//     blockSize = blockToInsert->freeBlockSize;
//     for(iterator = startListMem; iterator->nextFreeBlock->freeBlockSize < blockSize; iterator = iterator->nextFreeBlock) {
//         // Just iterate to the correct position
//     }

//     // Update the list to include the block being inserted in the correct position
//     blockToInsert->nextFreeBlock = iterator->nextFreeBlock;
//     iterator->nextFreeBlock = blockToInsert;
// }

// void *mallocMemory(uint64_t sizeRequired) {
//     freeMemList_p currentBlock, prevBlock, newBlockLink;
//     void *blockToReturn = NULL;

//     //If this is the first call to malloc, the heap needs to be initialised in order to create the list of free blocks
//     if(!freeListInitialised) {
//         initBlocksMM();
//         freeListInitialised = 1;
//     }

//     if(sizeRequired > 0) {
//         sizeRequired += blockStructSize;

//         // Ensures alignment of the block
//         if(sizeRequired & BYTE_ALIGNMENT_MASK) {
//             sizeRequired += (BYTE_ALIGNMENT - (sizeRequired & BYTE_ALIGNMENT_MASK));
//         }
//     }

//     if(sizeRequired < freeRemainingBytes) { //antes estaba ADJUSTED_HEAP_SIZE
//         prevBlock = startListMem;
//         currentBlock = startListMem->nextFreeBlock;

//         while((currentBlock->freeBlockSize < sizeRequired) && (currentBlock->nextFreeBlock != NULL)) {
//             prevBlock = currentBlock;
//             currentBlock = currentBlock->nextFreeBlock;
//         }

//         if(currentBlock != endListMem) {
//             //Return the memory space desired
//             blockToReturn = (void *) ( ( (uint8_t *) prevBlock->nextFreeBlock) + blockStructSize);

//             //As this memory block will be used, it must be removed from this free memory list
//             prevBlock->nextFreeBlock = currentBlock->nextFreeBlock;

//             //If the block is larger than required, it can be split into two
//             if( (currentBlock->freeBlockSize - sizeRequired) > MIN_BLOCK_SIZE) {
//                 //To split the block into two, a new block will be created with the number of bytes requested.
//                 newBlockLink = (void *) (((uint8_t *) currentBlock) + sizeRequired);

//                 //Calculate the size fo both blocks created from splitting the original one
//                 newBlockLink->freeBlockSize = currentBlock->freeBlockSize - sizeRequired;
//                 currentBlock->freeBlockSize = sizeRequired;

//                 //Insert the new block into the list of free blocks
//                 insertFreeBlockOnList(newBlockLink);
//             }

//             freeRemainingBytes -= currentBlock->freeBlockSize;
//         }
//     }

//     return blockToReturn;

// }

// void initBlocksMM() {
//     freeMemList_p firstFreeBlock;
//     uint8_t *alignedHeapPtr;

//     // Ensures that the heap starts on a correct aligned boundary
//     alignedHeapPtr = (uint8_t *) ( ((unsigned long)(char *)(memoryPosition) + BYTE_ALIGNMENT) & ( ~( ( POINTER_SIZE_TYPE ) BYTE_ALIGNMENT_MASK ) ) );

//     // startListMem is used to refer to the first item in the list of free blocks
//     startListMem->nextFreeBlock = (void *) alignedHeapPtr;
//     startListMem->freeBlockSize = (uint64_t) 0;

//     // endListMem is used to mark the end of the list of free blocks
//     endListMem->freeBlockSize = ADJUSTED_HEAP_SIZE;
//     endListMem->nextFreeBlock = NULL;

//     // Prepares a single first block in the freeMem List
//     firstFreeBlock = (void *) alignedHeapPtr;
//     firstFreeBlock->freeBlockSize = ADJUSTED_HEAP_SIZE;
//     firstFreeBlock->nextFreeBlock = endListMem;

//     freeRemainingBytes = firstFreeBlock->freeBlockSize;
// }

// void freeMemory(void * free){
//     uint8_t *toFree = (uint8_t *) free;
//     freeMemList_p toAddFreeBlock;

//     if(toFree != NULL) {
//         //The memory space to be freed will have a freeBlocksMemList_t structure before it
//         toFree -= blockStructSize;

//         //Cast to freeBlocksMemList_t * to avoid compilation issues
//         toAddFreeBlock = (freeMemList_t *) toFree;

//         //Add this new block to the list of free blocks
//         insertFreeBlockOnList(toAddFreeBlock);
//     }
// }
