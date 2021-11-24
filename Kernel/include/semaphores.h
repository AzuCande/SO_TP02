#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdint.h>
#include <scheduler.h>
#include <memManager.h>
#include <lib.h>
#include <stddef.h>

#define MAX_BLOCKED_PID 16
#define MAX_SEM_COUNT 25

#define SEM_USED 1
#define SEM_FREE 0

#define ERROR_CODE -1

typedef struct semType {
    int state;
    uint64_t value;
    int blockedPIDsQty;
    uint32_t blockedPIDs[MAX_BLOCKED_PID];
    uint16_t listeners;
    uint32_t semId;
    int mutex;
} semType;

void openSemaphore(uint32_t id, uint32_t initValue, int *toReturn);
void waitSemaphore(uint32_t id, int *toReturn);
void postSemaphore(uint32_t id, int *toReturn);
void closeSemaphore(uint32_t id, int *toReturn);
void printSemaphore(char * buffer);


#endif