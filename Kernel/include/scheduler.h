#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "lib.h"
#include <circularListADT.h>
#include <registers.h>

#define STRING_SIZE 15
#define STACK_SIZE (4*1024)

#define MAX_PRIORITY 1
#define MIN_PRIORITY 5
#define AMOUNT_PRIORITY 5
#define DEFAULT_PRIORITY 1
#define DEFAULT_TICKETS 1

typedef enum {
    READY,
    BLOCKED, 
    KILLED
} processState;

typedef struct processData {
    unsigned int pid;
    unsigned int ppid;
    char name[STRING_SIZE];

    // foreground 1 | background 0
    unsigned int foreground;
    unsigned int priority;
    processState state;

    uint64_t * bp;
    uint64_t * sp;

    int argc;
    char **argv;
    
} processData;

void initScheduler();
int createProcess(void (*entryPoint) (int, char **), int argc, char **argv, unsigned int foreground);
void *scheduler(uint64_t *sp);
int setProcessData(processData *, unsigned int, char *, unsigned int, uint64_t);
void setNewStackFrame(void (*entryPoint) (int, char **), int argc, char **argv, void *bp);
void changeProcessPriority(unsigned int pid, unsigned int assignPriority);
unsigned int getPid();
void blockProcess(unsigned int pid);
void killProcess(unsigned int pid);
void resignCPU();
void printProcessList(char * buffer);


#endif