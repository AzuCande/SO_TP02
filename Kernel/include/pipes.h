#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <lib.h>
#include <semaphores.h>

#define MAX_PIPES 10
#define PIPE_BUF_SIZE 1024
#define ERROR -1
#define PIPE_IN_USE 1
#define PIPE_FREE 0

typedef struct pipe{
    int state;
    // TODO Despues nos fijamos bien si usar id o no
    int id;
    char buffer[PIPE_BUF_SIZE];

    unsigned int readIdx;
    unsigned int writeIdx;

    int readSem;
    int writeSem;

    int processCount; // Number of processes using the pipe
} pipe_t;

void pipeOpen(uint32_t id, int *toReturn);
void pipeClose(uint32_t id, int *toReturn);
void pipeRead(uint32_t id, int *toReturn);
void pipeWrite(uint32_t id, char *string, int *toReturn);
int createPipe(uint32_t id);
void printPipes(char *buffer);

#endif