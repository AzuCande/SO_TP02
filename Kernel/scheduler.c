#include "include/scheduler.h"

#define START_PID_BAPTIZER 1

unsigned int pidBaptizer = START_PID_BAPTIZER;
processData * currentProcess = NULL;

void createProcess() {

}

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

unsigned int getPid() {
    
}

void blockProcess(unsigned int pid) {
    processData * process;

}

void unblockProcess(unsigned int pid) {

}

void killProcess(unsigned int pid) {

}

void resignCPU() {

}

void printProcessList() {

}