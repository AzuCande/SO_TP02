#include <semaphores.h>

static void acquire(int *lock);
static void release(int *lock);
static void createNewSemaphore(uint32_t id, uint32_t initValue, int semIndex);
static int findSemaphore(uint32_t id);
static int findFreeSemaphore();

int semTotal = 0;
semType semaphores[MAX_SEM_COUNT];

void openSemaphore(uint32_t id, uint32_t initValue, int *toReturn) {

    int index = findSemaphore(id);

    if(index == ERROR_CODE) {
        if((index = findFreeSemaphore()) == ERROR_CODE) {
            *toReturn = ERROR_CODE;
            return;
        }

        createNewSemaphore(id, initValue, index);
    } else {
        semaphores[index].listeners++;
    }

    *toReturn = index;
}


static void createNewSemaphore(uint32_t id, uint32_t initValue, int semIndex) {
   
    semaphores[semIndex].state = SEM_USED;
    semaphores[semIndex].value = initValue;
    semaphores[semIndex].blockedPIDsQty = 0;
    semaphores[semIndex].listeners = 1;
    semaphores[semIndex].semId = id;
    semaphores[semIndex].mutex = 0;
    
    semTotal++;
}

void waitSemaphore(uint32_t id, int *toReturn) {

    int index = findSemaphore(id);

    if(index == ERROR_CODE) {
        *toReturn = ERROR_CODE;
        return;
    }

    acquire(&(semaphores[index].mutex));

    if (semaphores[index].value > 0) {
        semaphores[index].value--;
        release(&(semaphores[index].mutex));
        *toReturn = 0;
    } else {
        uint64_t thisPID;
        getPid(&thisPID);
        if (semaphores[index].blockedPIDsQty >= MAX_BLOCKED_PID) {
            *toReturn = ERROR_CODE;
            return;
        }
        semaphores[index].blockedPIDs[semaphores[index].blockedPIDsQty++] = thisPID;
        release(&(semaphores[index].mutex));
        int res;
        blockProcess(thisPID, &res);
    }
    *toReturn = 0;
}

void postSemaphore(uint32_t id, int *toReturn) {
    int index = findSemaphore(id);
    
    if(index == ERROR_CODE) {
        *toReturn = ERROR_CODE;
        return;
    }

    acquire(&(semaphores[index].mutex));

    if(semaphores[index].blockedPIDsQty > 0) {
        int nextPID = semaphores[index].blockedPIDs[0];

        for(int i = 0; i < semaphores[index].blockedPIDsQty - 1; i++) {
            semaphores[index].blockedPIDs[i] = semaphores[index].blockedPIDs[i + 1];
        }

        semaphores[index].blockedPIDsQty--;
        int res;
        unlockProcess(nextPID, &res);
        release(&(semaphores[index].mutex));
        *toReturn = 0;
        return;
    } else {
        semaphores[index].value++;
    }

    release(&(semaphores[index].mutex));
}

void closeSemaphore(uint32_t id, int* toReturn) {
    
    int index = findSemaphore(id);
    if(index == ERROR_CODE) {
        *toReturn = ERROR_CODE;
        return;
    }

    // Check if it is the last process
    semaphores[index].listeners--;
    if(semaphores[index].listeners > 0) {
        *toReturn = 0;
        return;
    }

    // Check if there are any blocked processes
    uint32_t toUnblock;
    int blockCount = semaphores[index].blockedPIDsQty;
    for(int i = 0; i < blockCount; i++) {
        toUnblock = semaphores[index].blockedPIDs[i];

        int result;
        unlockProcess(toUnblock, &result);
        semaphores[index].blockedPIDsQty--;
    }
    
    semaphores[index].state = SEM_FREE;
    semaphores[index].value = 0;
    semaphores[index].blockedPIDsQty = 0;
    semaphores[index].listeners = 0;
    semaphores[index].semId = 0;
    semaphores[index].mutex = 0;
    semTotal--;

    *toReturn = 1;
}

void printSemaphore(char * buffer) { 
    
    unsigned int index = 0;
    unsigned int usedSemaphores = 0;

    if(semTotal == 0) {
        strcat(buffer, "\nThere are no semaphores to print\n", &index);
        return;
    }

    char header[13] = "\nSEMAPHORES\n";
    char header2[45] = "Index|\tID|\tValue|\tQty|\tBlocked Qty|\tBlocked PIDs\n";
    
    strcat(buffer, header, &index);
    strcat(buffer, header2, &index);
    // buffer[index++] = '\n';

    for(int i = 0; i < MAX_SEM_COUNT; i++) {
        if(semaphores[i].state == SEM_USED){
            char aux[100] = {0};

            itoa(usedSemaphores++, aux, 10);
            strcat(buffer, aux, &index);
            strcat(buffer, "|\t", &index);

            itoa(semaphores[i].semId, aux, 10);
            strcat(buffer, aux, &index);
            strcat(buffer, "|\t", &index);

            itoa(semaphores[i].value, aux, 10);
            strcat(buffer, aux, &index);
            strcat(buffer, "|\t", &index);

            itoa(semaphores[i].listeners, aux, 10);
            strcat(buffer, aux, &index);
            strcat(buffer, "\t", &index);

            itoa(semaphores[i].blockedPIDsQty, aux, 10);
            strcat(buffer, aux, &index);
            strcat(buffer, "|\t", &index);

            strcat(buffer, "[ ", &index);
            for (int j = 0; j < semaphores[i].blockedPIDsQty; j++) {
                itoa(semaphores[i].blockedPIDs[j], aux, 10);
                strcat(buffer, aux, &index);
                strcat(buffer, " ", &index);
            }
            strcat(buffer, "]", &index);

            strcat(buffer, "\n", &index);
        }
         
    }

    buffer[index] = '\0';
}

static int findSemaphore(uint32_t id) {
    for(int i = 0; i < MAX_SEM_COUNT; i++) {
        if(semaphores[i].semId == id && semaphores[i].state == SEM_USED) {
            return i;
        }
    }
    return ERROR_CODE;
}

static int findFreeSemaphore() {
    for(int i = 0; i < MAX_SEM_COUNT; i++) {
        if(semaphores[i].state == SEM_FREE)
            return i;
    }
    return -1;
}

static void acquire(int *lock) {
    while (_xchg(lock, 1) != 0);
}

static void release(int *lock) {
    _xchg(lock, 0);
}