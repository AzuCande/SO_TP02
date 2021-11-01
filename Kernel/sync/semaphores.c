#include <semaphores.h>

static void acquire(int *lock);
static void release(int *lock);
static void createNewSemaphore(uint32_t id, uint32_t initValue, semType * semaphore);


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

semType * openSemaphore(uint32_t id, uint32_t initValue) {

    if(semaphoresList == NULL) {
        initSemaphores();
    }

    semType * semaphore = findSemaphore(id);

    if(semaphore == NULL) {
        createNewSemaphore(id, initValue, semaphore);
    }
    return semaphore;
}

static void createNewSemaphore(uint32_t id, uint32_t initValue, semType * semaphore) {

    semaphore = mallocMemory(sizeof(semaphore));

    semaphore->id = id;
    semaphore->value = initValue;
    semaphore->next = NULL;
    semaphore->blockedPIDsQty = 0;
    semaphore->mutex = 0;

    if(semaphoresList->first == NULL) {
        semaphoresList->first = semaphore;
        semaphoresList->iterator = semaphore;
    } else {
        semaphoresList->last->next = semaphore;
    }

    semaphoresList->semQty++;
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
        int thisPID = getPid();
        sem->blockedPIDs[sem->blockedPIDsQty++] = thisPID;
        release(&(sem->mutex));
        blockProcess(thisPID);
    }

    return 0;
}

int postSemaphore(uint32_t id) {
    semType * sem = findSemaphore(id);
    if (sem == NULL) {
        return -1;
    }

    acquire(&(sem->mutex));

    if(sem->blockedPIDsQty > 0) {
        int nextPID = sem->blockedPIDs[0];

        for(int i = 0; i < sem->blockedPIDsQty - 1; i++) {
            sem->blockedPIDs[i] = sem->blockedPIDs[i + 1];
        }

        sem->blockedPIDsQty--;
        blockProcess(nextPID);
        release(&(sem->mutex));
        return 0;

    } else {
        sem->value++;
    }

    release(&(sem->mutex));

    return 0;
}

int closeSemaphore(uint32_t id) {
    semType * semaphore = findSemaphore(id);
    if(semaphore == NULL) {
        return -1;
    }
    
    if(semaphore == semaphoresList->first) {
        semaphoresList->first = semaphore->next;
    } else {
        semaphoresList->iterator = semaphoresList->first;

        while(semaphoresList->iterator->next != semaphore) {
            semaphoresList->iterator = semaphoresList->iterator->next;
        }
        semaphoresList->iterator->next = semaphore->next;
    }

    semaphoresList->semQty--;
    freeMemory(semaphore);
}

void printSemaphore(char * buffer) { 
    
    semaphoresList->iterator = semaphoresList->first;
    unsigned int index = 0;

    if(semaphoresList->iterator == NULL) {
        strcat(buffer, "There are no semaphores to print", &index);
    }

    char header[13] = "\nSEMAPHORES\n";
    char header2[24] = "ID\t Value\t Qty Process";
    
    strcat(buffer, header, &index);
    strcat(buffer, header2, &index);
    buffer[index++] = '\n';

    while(semaphoresList->iterator != NULL) {
        char aux[11] = {0};

        intToString(aux,semaphoresList->iterator->id);
        strcat(buffer, aux, &index);

        intToString(aux,semaphoresList->iterator->value);
        strcat(buffer, aux, &index);

        buffer[index++] = '\n';
        semaphoresList->iterator = semaphoresList->iterator->next; 
    }

    buffer[index] = '\0';
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