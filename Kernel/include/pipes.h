#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <lib.h>
#include <semaphores.h>

#define MAX_PIPES 10
#define PIPE_BUF_SIZE 1024
#define ERROR -1

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

int pipeOpen(uint32_t id);
int pipeClose(uint32_t id);
int pipeRead(uint32_t id);
int pipeWrite(uint32_t id, char *string);
int createPipe(uint32_t id);
void printPipes(char *buffer);

#endif