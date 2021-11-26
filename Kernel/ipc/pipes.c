#include <pipes.h>

static int findPipe(uint32_t id);
static int getAvailablePipe();
static int createPipe(uint32_t id);
static int writeCharPipe(int pipeIdx, char c) ;

pipe_t pipes[MAX_PIPES];
// Semaphore id
uint32_t sem_id = 200; // Notify user of the id of pipes' semaphores
unsigned int pipesCount = 0;

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
    
    pipes[pipeIdx].processCount--;

    if(pipes[pipeIdx].processCount > 0) {
        *toReturn = 0;
        return;
    }

    int readClose, writeClose;
    closeSemaphore(pipes[pipeIdx].readSem, &readClose);
    closeSemaphore(pipes[pipeIdx].writeSem, &writeClose);

    if(readClose == ERROR || writeClose == ERROR) {
        *toReturn = -1;
        return;
    }

    pipes[pipeIdx].state = PIPE_FREE;
    pipesCount--;
    *toReturn = readClose;
}

void pipeRead(uint32_t id, char *str, int *toReturn) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR) {
        *toReturn = ERROR;
        return;
    }
    
    waitSemaphore(pipes[id].readSem, toReturn);

    *str = pipes[pipeIdx].buffer[ pipes[pipeIdx].readIdx ];
    pipes[pipeIdx].readIdx = (pipes[pipeIdx].readIdx + 1) % PIPE_BUF_SIZE;

    postSemaphore(pipes[id].writeSem, toReturn);
}

void pipeWrite(uint32_t id, char *str, int *toReturn) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR) {
        *toReturn = ERROR;
        return;
    }
    while(*str != 0) {
        writeCharPipe(pipeIdx, *str++);
    }
    *toReturn = 0;
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


static int createPipe(uint32_t id) {
    int pipeIdx = getAvailablePipe();

    if(pipeIdx == ERROR)
        return ERROR;
    
    pipes[pipeIdx].id = id;
    pipes[pipeIdx].readIdx = 0;
    pipes[pipeIdx].writeIdx = 0;
    pipes[pipeIdx].processCount = 0;
    pipes[pipeIdx].state = PIPE_IN_USE;

    // Check if semaphores were created
    openSemaphore(sem_id++, 0, &pipes[pipeIdx].readSem);
    openSemaphore(sem_id++, PIPE_BUF_SIZE, &pipes[pipeIdx].writeSem);
    if(pipes[pipeIdx].readSem == ERROR || pipes[pipeIdx].writeSem == ERROR) {
        return ERROR;
    }

    pipesCount++;

    return id;
}

// Returns position of pipe in the array
static int findPipe(uint32_t id) {
    for(int i = 0; i < MAX_PIPES; i++) {
        if(pipes[i].state == PIPE_IN_USE && pipes[i].id == id)
            return i;
    }
    return ERROR;
}

// Get position of the array where there is no used pipe
static int getAvailablePipe() {
    for(int i = 0; i < MAX_PIPES; i++) {
        if(pipes[i].state == PIPE_FREE) {
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
    char subheader[51] = "Pipe ID|\t ReadIdx|\t WriteIdx|\t ReadSem|\t WriteSem\n";

    strcat(buffer, header, &i);
    strcat(buffer, subheader, &i);

    for(int j = 0; j < pipesCount; j++) {
        
        if(pipes[j].state) {
            char aux[11] = {0};

            itoa(pipes[j].id,aux,10);
            strcat(buffer, aux, &i);

            buffer[i++] = '\t';
            
            itoa(pipes[j].readIdx,aux,10);
            strcat(buffer, aux, &i);

            buffer[i++] = '\t';

            itoa(pipes[j].writeIdx, aux,10);
            strcat(buffer, aux, &i);

            buffer[i++] = '\t';

            if(pipes[j].readSem == BLOCKED)
                strcat(buffer, "Y", &i);
            else
                strcat(buffer, "N", &i);

            buffer[i++] = '\t';

           if(pipes[j].writeSem == BLOCKED)
                strcat(buffer, "Y", &i);
            else
                strcat(buffer, "N", &i);

            buffer[i++] = '\n';
        }
    }

    buffer[i] = '\0';
}