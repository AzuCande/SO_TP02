#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <lib.h>
#include <semaphores.h>

#define MAX_PIPES 10
#define PIPE_BUF_SIZE 1024
#define ERROR -1

int pipeOpen(uint32_t id);
int pipeClose(uint32_t id);
int pipeRead(uint32_t id);
int pipeWrite(uint32_t id, char *string);
int createPipe(uint32_t id);
void printPipes(char *buffer);

#endif