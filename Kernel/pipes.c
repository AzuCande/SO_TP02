#include <pipes.h>

static int writeCharPipe(int pipeIdx, char c);
static int findPipe(uint32_t id);
static int getAvailablePipe();

pipe_t pipes[MAX_PIPES];
// Semaphore id
uint32_t sem_id = 1; // Notify user of the id of pipes' semaphores
unsigned int pipesCount = 0;

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

// When pipe closed, the memory assigned to semaphore will be freed
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
    pipesCount--;

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
    pipesCount++;

    // Check if semaphores were created
    if((newPipe.readSem = openSemaphore(sem_id++, 0)->id) == ERROR) {
        return ERROR;
    }

    if((newPipe.writeSem = openSemaphore(sem_id++, 0)->id) == ERROR) {
        return ERROR;
    }

    return id;
}

// Returns position of pipe in the array
static int findPipe(uint32_t id) {
    for(int i = 0; i < MAX_PIPES; i++) {
        if(pipes[i].active && pipes[i].id == id)
            return i;
    }
    return ERROR;
}

// Get position of the array where there is no active pipe
static int getAvailablePipe() {
    for(int i = 0; i < MAX_PIPES; i++) {
        if(!pipes[i].active) {
            return i;
        }
    }
    return ERROR;
}

void printPipes(char *buffer) {
    unsigned int i = 0;
    if(pipesCount == 0)
        strcat(buffer, "There are no pipes to print", &i);
    
    char header[8] = "\nPIPES\n";
    char *subheader = "Pipe ID\t ReadIdx\t WriteIdx\t ReadSem\t WriteSem\n";

    strcat(buffer, header, &i);
    strcat(buffer, subheader, &i);

    for(int j = 0; j < pipesCount; j++) {
        pipe_t toPrint = pipes[j];
        
        if(toPrint.active) {
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