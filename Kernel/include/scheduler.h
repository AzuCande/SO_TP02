#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "lib.h"
#include  <circularListADT.h>

#define STRING_SIZE 15

#define MAX_PRIORITY 1
#define MIN_PRIORITY 5
#define DEFAULT_PRIORITY 3

typedef enum {
    READY,
    BLOCKED, 
    KILLED, 
    RESIGNED
} processState;

typedef struct processData {
    unsigned int pid;
    char name[STRING_SIZE];

    unsigned int foreground;
    unsigned int priority;
    processState state;

    uint64_t * bp;
    uint64_t * sp;
    uint64_t * ep;
    
} processData;

#endif