#include <pipes.h>

static int findPipe(uint32_t id);
static int getAvailablePipe();
static int createPipe(uint32_t id);

pipe_t pipes[MAX_PIPES];
// Semaphore id
uint32_t sem_id = 200; // Notify user of the id of pipes' semaphores
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
    
    pipes[pipeIdx].processCount--;

    if(pipes[pipeIdx].processCount > 0) {
        *toReturn = 0;
        return;
    }

    closeSemaphore(pipes[pipeIdx].semId, toReturn);

    if(*toReturn == ERROR) 
        return;

    pipes[pipeIdx].state = PIPE_FREE;
    pipesCount--;
}

void pipeRead(uint32_t id, char *str, int *toReturn) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR) {
        *toReturn = ERROR;
        return;
    }
    
    pipes[id].readBlocked = BLOCKED;
    waitSemaphore(pipes[id].semId, toReturn);
    pipes[id].readBlocked = UNBLOCKED;

    int n = strlen(str);
    
    for(int i = 0; i < n; i++) {
        while(pipes[id].readIdx == pipes[id].writeIdx) {
            postSemaphore(pipes[id].semId, toReturn);
            pipes[id].readBlocked = BLOCKED;
            yield();
            waitSemaphore(pipes[id].semId, toReturn);
            pipes[id].readBlocked = UNBLOCKED;
        }
        str[i] = pipes[pipeIdx].buffer[ pipes[pipeIdx].readIdx++ % PIPE_BUF_SIZE ];
        if(str[i] == '\0')
            break;
    }

    postSemaphore(pipes[id].semId, toReturn);
}

void pipeWrite(uint32_t id, char *str, int *toReturn) {
    int pipeIdx = findPipe(id);

    // Check if pipe exists
    if(pipeIdx == ERROR) {
        *toReturn = ERROR;
        return;
    }

    pipes[id].writeBlocked = BLOCKED;
    waitSemaphore(pipes[id].semId, toReturn);
    pipes[id].writeBlocked = UNBLOCKED;

    int n = strlen(str);
    for(int i = 0; i < n; i++) {
        while(pipes[id].writeIdx == pipes[id].readIdx + PIPE_BUF_SIZE) {
            postSemaphore(pipes[id].semId, toReturn);
            pipes[id].writeBlocked = BLOCKED;
            yield();
            waitSemaphore(pipes[id].semId, toReturn);
            pipes[id].writeBlocked = UNBLOCKED;
        }
        pipes[id].buffer[pipes[id].writeIdx++ % PIPE_BUF_SIZE] = str[i];
        if (str[i] == '\0')
            break;
    }
    
    postSemaphore(pipes[id].semId, toReturn);
}


static int createPipe(uint32_t id) {
    int pipeIdx = getAvailablePipe();

    if(pipeIdx == ERROR)
        return ERROR;

    // Check if semaphore was created
    openSemaphore(sem_id++, 0, &pipes[pipeIdx].semId);
    if(pipes[pipeIdx].semId == ERROR) {
        return ERROR;
    }
    
    pipes[pipeIdx].id = id;
    pipes[pipeIdx].readIdx = 0;
    pipes[pipeIdx].writeIdx = 0;
    pipes[pipeIdx].readBlocked = 0;
    pipes[pipeIdx].writeBlocked = 0;
    pipes[pipeIdx].processCount = 0;
    pipes[pipeIdx].state = PIPE_IN_USE;

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
    char subheader[59] = "Pipe ID|\t ReadIdx|\t WriteIdx|\t ReadBlocked|\t WriteBlocked\n";

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

            if(pipes[j].readBlocked == BLOCKED)
                strcat(buffer, "Y", &i);
            else
                strcat(buffer, "N", &i);

            buffer[i++] = '\t';

           if(pipes[j].writeBlocked == BLOCKED)
                strcat(buffer, "Y", &i);
            else
                strcat(buffer, "N", &i);

            buffer[i++] = '\n';
        }
    }

    buffer[i] = '\0';
}