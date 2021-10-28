#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdint.h>
#include <memManager.h>

#define MAX_BLOCKED_PID 20

typedef struct semType {
    uint32_t id;
    uint32_t value; // state
    uint32_t blockedPIDs[MAX_BLOCKED_PID];
    struct semType * next;
} semType;

typedef struct semList {
    uint64_t semQty;
    semType * first;
    semType * last;
} semList;

#endif