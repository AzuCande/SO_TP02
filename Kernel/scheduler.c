#include "include/scheduler.h"

#define START_PID_BAPTIZER 1

unsigned int pidBaptize = START_PID_BAPTIZER;

void setProcessData(processData * p, unsigned int pid, char * name) {
    p->pid = pid;
}