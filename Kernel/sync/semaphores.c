#include <semaphores.h>

semList * semaphoresList = NULL;

int initSemaphores() {
    semaphoresList = mallocMemory(sizeof(semList));

    if(semaphoresList == NULL) {
        return -1;
    }

    semaphoresList->first = NULL;
    semaphoresList->iterator = NULL;
    semaphoresList->last = NULL;
    semaphoresList->semQty = 0;

    return 0;
}

semType * openSemaphore(uint32_t id) {

    if(semaphoresList == NULL) {
        initSemaphores();
    }

    semType * semaphore = findSemaphore(id);

    if(semaphore == NULL) {
        createNewSemaphore(id, semaphore);
    }
}

static void createNewSemaphore(uint32_t id, semType * semaphore) {

    semaphore = mallocMemory(sizeof(semaphore));

    semaphore->id;
    semaphore->value;
    semaphore->next = NULL;
    semaphore->mutex = 0;

    if(semaphoresList->first == NULL) {
        semaphoresList->first = semaphore;
        semaphoresList->last = semaphore;
        semaphoresList->iterator = semaphore;
    }

    semaphoresList->semQty++;
    semaphoresList->last->next = semaphore;
    semaphoresList->last = semaphore;

    return;
}

int waitSemphore(uint32_t id) {
    
    semType * sem = findSemaphore(id);
    if (sem == NULL) {
        return -1;
    }

    acquire(&(sem->mutex));

    if (sem->value > 0)
    {
        sem->value--;
        release(&(sem->mutex));
        return 0;
    }
    else
    {
        int thisPid = getPid();
        sem->blockedPIDs[sem->blockedPIDsQty++] = thisPid;
        release(&(sem->mutex));
        blockProcess(thisPid);
    }

    return 0;
}

int postSemaphore(uint32_t id) {

}

void closeSemaphore(uint32_t id) {
    semType * semaphore = findSemaphore(uint32_t id);
    if(semaphore != NULL) {
        
    }
}

void printSemaphore() {
    
}

static semType * findSemaphore(uint32_t id) {
    semType * s = semaphoresList->first;

    while(s != NULL) {
        if(s->id == id) {
            return s;
        }
    }

    return NULL;
}

static void acquire(int *lock) {
    while (_xchg(lock, 1) != 0);
}

static void release(int *lock) {
    _xchg(lock, 0);
}