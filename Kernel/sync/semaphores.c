#include <semaphores.h>

typedef struct semType {
    uint32_t pid;
    uint32_t blockedPIDs[100]; // TODO cambiar magic number por macro
}