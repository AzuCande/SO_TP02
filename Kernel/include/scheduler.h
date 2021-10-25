#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "lib.h"

#define STRING_SIZE 15
enum processState {READY = 0, BLOCKED, KILLED, RESIGNED};

typedef struct processData {
    unsigned int pid;
    char name[STRING_SIZE];

    unsigned int foreground;
    unsigned int pid;
    unsigned int priority;
    enum processState state;

    uint64_t * bp;
    uint64_t * sp;
    uint64_t * ep;
    
} processData;

#endif