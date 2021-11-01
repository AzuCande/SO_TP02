#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdint.h>
#include <memManager.h>
#include <lib.h>
#include <stddef.h>

#define MAX_BLOCKED_PID 20

typedef struct semType {
    uint32_t id;
    uint32_t value;
    int mutex;
    uint32_t blockedPIDs[MAX_BLOCKED_PID];
    uint32_t blockedPIDsQty;
    struct semType * next;
} semType;

typedef struct semList {
    uint64_t semQty;
    semType * first;
    semType * last;
    semType * iterator;
} semList;

int initSemaphores();
semType * openSemaphore(uint32_t id, uint32_t initValue);
int waitSemphore(uint32_t id);
int postSemaphore(uint32_t id);
int closeSemaphore(uint32_t id);
void printSemaphore(char * buffer);


#endif