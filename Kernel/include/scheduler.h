#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <interrupts.h>
#include <lib.h>
#include <stddef.h>
#include <circularListADT.h>

void yield();
uint64_t * scheduler(uint64_t *currentProces);
void addProcess(uint64_t *currentProces, char *name,uint64_t *pid,uint64_t *ep,int foreground, uint64_t argc, char** argv, int *fds);
void endProcessWrapper(uint64_t pid, int *result);
void getPid(uint64_t *pid);
// void listAllProcess(char **ProcessList); TODO delete
void blockProcess(uint64_t pid, int *result);
void unlockProcess(uint64_t pid, int *result);
void exceptionProcess();
void ps(char *array);
void nice(uint64_t pid, uint64_t priority, int *result);

#endif



























//----------------------------------------
// #ifndef SCHEDULER_H
// #define SCHEDULER_H

// #include <stdint.h>
// #include <lib.h>
// #include <registers.h>
// #include <interrupts.h>
// #include <memManager.h>

// #define STRING_SIZE 15
// #define STACK_SIZE (4*1024)

// #define MAX_PRIORITY 1
// #define MIN_PRIORITY 5
// #define AMOUNT_PRIORITY 5
// #define DEFAULT_PRIORITY 1
// #define DEFAULT_TICKETS 1

// typedef enum {
//     READY,
//     BLOCKED, 
//     KILLED
// } processState;

// typedef struct processData {
//     unsigned int pid;
//     unsigned int ppid;
//     char name[STRING_SIZE];

//     unsigned int foreground;    // foreground 1 | background 0
//     int fds[2];         // 0 -> IN | 1 -> OUT

//     unsigned int priority;
//     processState state;

//     uint64_t * bp;
//     uint64_t * sp;

//     int argc;
//     char **argv;
    
// } processData;

// void initScheduler();
// int createProcess(void (*entryPoint) (/*int, */char **), int argc, char **argv, unsigned int foreground, int *fds);
// void *scheduler(uint64_t *sp);
// int setProcessData(processData *, unsigned int, char *, unsigned int, int * fds);
// void setNewStackFrame(void (*entryPoint) (/*int, */char **), int argc, char **argv, void *bp);
// void changeProcessPriority(unsigned int pid, unsigned int assignPriority);
// unsigned int getPid();
// void blockProcess(unsigned int pid);
// void unblockProcess(unsigned int pid);
// void killProcess(unsigned int pid);
// void resignCPU();
int currentReadFd();
int currentWriteFd();
int isCurrentFg();
// void printProcessList(char * buffer);
// void freeProcess(processData *process);
// void exitProcess();


// #endif