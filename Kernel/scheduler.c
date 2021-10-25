#include "include/scheduler.h"

#define START_PID_BAPTIZER 1

unsigned int pidBaptize = START_PID_BAPTIZER;

void setProcessData(processData * p, unsigned int pid, char * name, unsigned int foreground
) {
    p->pid = pid;
    p->name[0]=0;
    strcpy(p->name, name);

    p->foreground = foreground;
    p->priority;
    p->state = READY;

    p->bp;
    p->sp;
    p->ep;
}