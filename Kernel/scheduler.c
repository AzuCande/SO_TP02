#include "include/scheduler.h"

#define START_PID_BAPTIZER 1

unsigned int pidBaptizer = START_PID_BAPTIZER;
processData * currentProcess = 0;
circularList * processList;

void initScheduler() {
    processList = newCircularList();
    if(processList == NULL) {
        return;
    }
}

void createProcess(unsigned int foreground, char *name) {

    processData *process;
    // Check if there is enough memory available
    if( (process = mallocMemory(sizeof(processData)) == NULL)) {
        return;
    }

    setProcessData(process, pidBaptizer, name, foreground, currentProcess,,); //todo
    pidBaptizer++;
    addProcessOnCircularList(processList, process);
}

void setProcessData(processData * p, unsigned int pid, char * name, unsigned int foreground, uint64_t bp, uint64_t sp, uint64_t ep) {
    p->pid = pid;
    p->name[0] = 0;
    strcpy(p->name, name);

    p->foreground = foreground;
    p->priority = DEFAULT_PRIORITY;
    p->state = READY;

    p->bp = bp;
    p->sp = sp;
    p->ep = ep;
}

uint64_t contextSwitch(uint64_t *sp) {
    if(currentProcess != NULL) {
        currentProcess->sp = sp;
        
        if(currentProcess->state == READY)
            return currentProcess->sp;

        if(currentProcess->state == RESIGNED)
            currentProcess->state = READY;
    }

    // Ahora debo obtener el proceso a cambiar
}

void changeProcessPriority(unsigned int pid, unsigned int assignPriority) {
    if(assignPriority>MIN_PRIORITY || assignPriority<MAX_PRIORITY) {
        return;
    }

    processData * process = findProcessOnList(processList, pid);
    if(process != NULL) {
        process->priority = assignPriority;
    }
}

unsigned int getPid() {
    return currentProcess->pid;
}

void blockProcess(unsigned int pid) {
    processData * process;
    process = findProcessOnList(processList, pid);
    
    if(process != NULL) {
        process->state = BLOCKED;
    }

    // Llamar al timer tick
}

void unblockProcess(unsigned int pid) {
    processData * process;
    process = findProcessOnList(processList, pid);
    if(process != NULL) {
        process->state = READY;
    }

    // Llamar al timer tick
}

void killProcess(unsigned int pid) {
    processData *toKill;
    toKill = deleteProcessOnList(processList, pid);

    if(toKill == NULL)
        return;
    
    // Llamar al timer tick
}

void resignCPU() {
    currentProcess->state = RESIGNED;

    // Llamar al timer tick
}

void printProcessList(char * buffer) { //todo
    
    if(isEmptyCircularList(processList)) {
        buffer[0] = '\0';
        return;
    }

    char header[51] = "PID    Name    Priority    State    FG    SP    BP";
    char states[4][8] = {"Ready", "Blocked", "Killed", "Resigned"};
    int index = 0;
    strcat(buffer, header, &index);
    
    
    toBeginingCircularList(processList);
    do
    {

        
        buffer[index] = '\n';
        index++;
    } while (hasNextCircularList(processList));

    buffer[j] = '\0';
    
}

