#include <pipes.h>

static int writeCharPipe(int pipeIdx, char c);
static int findPipe(uint32_t id);
static int getAvailablePipe();

pipe_t pipes[MAX_PIPES];
// Semaphore id
uint32_t sem_id = 1; // Notify user of the id of pipes' semaphores
unsigned int pipesCount = 0;

// TODO: Arreglar pipes
void pipeOpen(uint32_t id, int *toReturn) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR) {
        pipeIdx = createPipe(id);

        // Check if created
        if(pipeIdx == ERROR) {
            *toReturn = ERROR;
            return;
        }
        
    }

    pipes[pipeIdx].processCount++;

    *toReturn = id;
}

// When pipe closed, the memory assigned to semaphore will be freed
void pipeClose(uint32_t id, int *toReturn) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR) {
        *toReturn =ERROR;
        return;
    }
    
    pipe_t toClose = pipes[pipeIdx];
    toClose.processCount--;

    if(toClose.processCount > 0) {
        *toReturn = 1;
        return;
    }

    int closeR, closeW;

    closeSemaphore(toClose.readSem, &closeR);
    closeSemaphore(toClose.writeSem, &closeW);

    if(closeR == ERROR || closeW == ERROR) {
        *toReturn = ERROR;
        return;
    }

    toClose.state = PIPE_FREE;
    pipesCount--;

    *toReturn = 1;
}

void pipeRead(uint32_t id, int *toReturn) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR) {
        *toReturn = ERROR;
        return;
    }
    
    pipe_t toRead = pipes[pipeIdx];

    waitSemaphore(toRead.readSem, toReturn);
    if(*toReturn == ERROR)
        return;

    char c = toRead.buffer[toRead.readIdx % PIPE_BUF_SIZE];
    toRead.readIdx++;

    postSemaphore(toRead.writeSem, toReturn);

    // return c;
}

void pipeWrite(uint32_t id, char *string, int *toReturn) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR) {
        *toReturn = ERROR;
        return;
    }
    
    while(*string != 0)
        writeCharPipe(pipeIdx, *string++);
    
    *toReturn = 1;
}

static int writeCharPipe(int pipeIdx, char c) {
    pipe_t toWrite = pipes[pipeIdx];

    int ans;

    waitSemaphore(toWrite.writeSem, &ans);
    
    if(ans == ERROR)
        return ans;

    toWrite.buffer[toWrite.writeIdx] = c;
    toWrite.writeIdx++;

    int aux = 0;

    postSemaphore(toWrite.readSem, &aux);
    
    return aux;
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
    newPipe.state = PIPE_IN_USE;
    pipesCount++;

    // Check if semaphores were created
    openSemaphore(sem_id++, 0, &newPipe.readSem);
    if(newPipe.readSem == ERROR) {
        return ERROR;
    }

    openSemaphore(sem_id++, 0, &newPipe.writeSem);
    if(newPipe.writeSem == ERROR) {
        return ERROR;
    }

    return id;
}

// Returns position of pipe in the array
static int findPipe(uint32_t id) {
    for(int i = 0; i < MAX_PIPES; i++) {
        if(pipes[i].state && pipes[i].id == id)
            return i;
    }
    return ERROR;
}

// Get position of the array where there is no used pipe
static int getAvailablePipe() {
    for(int i = 0; i < MAX_PIPES; i++) {
        if(!pipes[i].state) {
            return i;
        }
    }
    return ERROR;
}

void printPipes(char *buffer) {
    unsigned int i = 0;
    if(pipesCount == 0) {
        strcat(buffer, "There are no pipes to print", &i);
        return;
    }
    char header[8] = "\nPIPES\n";
    char *subheader = "Pipe ID\t ReadIdx\t WriteIdx\t ReadSem\t WriteSem\n";

    strcat(buffer, header, &i);
    strcat(buffer, subheader, &i);

    for(int j = 0; j < pipesCount; j++) {
        pipe_t toPrint = pipes[j];
        
        if(toPrint.state) {
            char aux[11] = {0};

            itoa(toPrint.id,aux,10);
            strcat(buffer, aux, &i);

            buffer[i++] = '\t';
            
            itoa(toPrint.readIdx,aux,10);
            strcat(buffer, aux, &i);

            buffer[i++] = '\t';

            itoa(toPrint.writeIdx, aux,10);
            strcat(buffer, aux, &i);

            buffer[i++] = '\t';

            itoa(toPrint.readSem,aux,10);
            strcat(buffer, aux, &i);

            buffer[i++] = '\t';

            itoa(toPrint.writeSem,aux, 10);
            strcat(buffer, aux, &i);

            buffer[i++] = '\n';
        }
    }

    buffer[i] = '\0';
}