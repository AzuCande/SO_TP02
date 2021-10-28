#include <semaphores.h>

semList * semaphoresList = NULL;

semType * openSemaphore(uint32_t id) {
    semType * semaphore = findSemaphore(id);

    if(semaphore == NULL) {
        createNewSemaphore(id, semaphore);
    }
}

static void createNewSemaphore(uint32_t id. semType * semaphore) {

    semaphore = mallocMemory(sizeof(semaphore));

    semaphore->id;
    semaphore->value;
    semaphore->next = NULL;

    semaphoresList->last->next = semaphore;
    semaphoresList->last = semaphore;

    return;
}

int waitSemphore(uint32_t id) {
    blockProcess(id);
}

int postSemaphore(uint32_t id) {

}

void closeSemaphore(uint32_t id) {

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