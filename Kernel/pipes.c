#include "./include/pipes.h"

typedef struct pipe{
    // Despues nos fijamos bien si usar id o no
    int id;
    char buffer[PIPE_BUF_SIZE];

    unsigned int readIdx;
    unsigned int writeIdx;

    int readSem;
    int writeSem;

    int active; // 1 -> active | 0 -> idle
    int processCount; // Number of processes using the pipe
} pipe_t;

pipe_t pipes[MAX_PIPES];
// Semaphore id
uint32_t sem_id; // TODO: Fijarse que valor poner

int pipeOpen(uint32_t id) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR) {
        pipeIdx = createPipe(id);

        // Check if created
        if(pipeIdx == ERROR)
            return ERROR;
    }

    pipes[pipeIdx].processCount++;

    return id;
}

int pipeClose(uint32_t id) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR)
        return ERROR;
    
    pipe_t toClose = pipes[pipeIdx];
    toClose.processCount--;

    if(toClose.processCount > 0) {
        return 1;
    }

    int closeR = closeSemaphore(toClose.readSem);
    int closeW = closeSemaphore(toClose.writeSem);

    if(closeR == ERROR || closeW == ERROR)
        return ERROR;

    toClose.active = 0;

    return 1;
}

int pipeRead(uint32_t id) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR)
        return ERROR;
    
    pipe_t toRead = pipes[pipeIdx];

    if(waitSemphore(toRead.readSem) == ERROR)
        return ERROR;

    char c = toRead.buffer[toRead.readIdx % PIPE_BUF_SIZE];
    toRead.readIdx++;

    if(postSemaphore(toRead.writeSem) == ERROR)
        return ERROR;

    return c;
}

int pipeWrite(uint32_t id, char *string) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR)
        return ERROR;
    
    while(*string != 0)
        writeCharPipe(pipeIdx, *string++);
    
    return id;
}

static int writeCharPipe(int pipeIdx, char c) {
    pipe_t toWrite = pipes[pipeIdx];

    if(waitSemphore(toWrite.writeSem) == ERROR)
        return ERROR;

    toWrite.buffer[toWrite.writeIdx] = c;
    toWrite.writeIdx++;

    if(postSemaphore(toWrite.readSem) == ERROR)
        return ERROR;
    
    return 0;
}

int createPipe(uint32_t id) {
    int pipeIdx = getAvailablePipe();

    if(pipeIdx == ERROR)
        return ERROR;
    
    pipe_t newPipe = pipes[pipeIdx];
    newPipe.id = id;
    newPipe.readIdx = 0;
    newPipe.writeIdx = 0;
    newPipe.processCount = 0;
    newPipe.active = 1;

    // Check if semaphores were created
    if((newPipe.readSem = openSemaphore(sem_id++, 0)->id) == ERROR) {
        return ERROR;
    }

    if((newPipe.writeSem = openSemaphore(sem_id++, 0)->id) == ERROR) {
        return ERROR;
    }

    return id;
}

int findPipe(uint32_t id) {
    for(int i = 0; i < MAX_PIPES; i++) {
        if(pipes[i].active && pipes[i].id == id)
            return i;
    }
    return ERROR;
}

// Get index of the array if there is no active pipe
int getAvailablePipe() {
    for(int i = 0; i < MAX_PIPES; i++) {
        if(!pipes[i].active) {
            return i;
        }
    }
    return ERROR;
}

void printPipes(char *buffer) {

}