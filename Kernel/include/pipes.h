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
#define BLOCKED 1
#define UNBLOCKED 0

typedef struct pipe_t {
    int state;
    int id;
    char buffer[PIPE_BUF_SIZE];
    uint16_t readIdx;
    uint16_t writeIdx;
    int readBlocked;
    int writeBlocked;
    int processCount; // Number of processes using the pipe
    int semId;
} pipe_t;

void pipeOpen(uint32_t id, int *toReturn);
void pipeClose(uint32_t id, int *toReturn);
void pipeRead(uint32_t id, char *str, int *toReturn);
void pipeWrite(uint32_t id, char *string, int *toReturn);
void printPipes(char *buffer);

#endif