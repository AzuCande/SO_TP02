#include "include/scheduler.h"

#define START_PID_BAPTIZER 1

unsigned int pidBaptizer = START_PID_BAPTIZER;
processData * currentProcess = 0;
processData * dummy;
circularList * processList;

void initScheduler() {
    processList = newCircularList();
    if(processList == NULL) {
        return;
    }
}

void createProcess(void (*entryPoint) (int, char **), unsigned int foreground, char *name) {

    if(entryPoint == NULL) {
        return;
    }
    
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
    p->tickets = DEFAULT_TICKETS;
    p->state = READY;

    p->bp = bp;
    p->sp = sp;
    p->ep = ep;
}

void setNewStackFrame(processData *process, void (*entryPoint) (int, char **), int argc, char **argv, void *rbp) {
    registerStruct *stackFrame = (registerStruct *) rbp - 1;

    stackFrame->r15 = 0x001;
    stackFrame->r14 = 0x002;
    stackFrame->r13 = 0x003;
    stackFrame->r12 = 0x004;
    stackFrame->r11 = 0x005;
    stackFrame->r10 = 0x006;
    stackFrame->r9 = 0x007;
    stackFrame->r8 = 0x008;
    // Data to run the process
    stackFrame->rsi = (uint64_t) argc;
    stackFrame->rdi = (uint64_t) entryPoint;
    stackFrame->rbp = 0x00B;
    stackFrame->rdx = (uint64_t) argv;
    stackFrame->rcx = 0x00D;
    stackFrame->rbx = 0x00E;
    stackFrame->rax = 0x00F;
    stackFrame->rip = wrapper;
    stackFrame->cs = 0x008;
    stackFrame->flags = 0x202;
    stackFrame->rsp = (uint64_t) process->bp;
}

static void wrapper(void (*entryPoint) (int, char**), int argc, char ** argv) {
    entryPoint(argc, argv);
    exitProcess();
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

    processData * p = changeProcessPriorityOnCircularList(processList,pid,assignPriority);
    p->priority = assignPriority;
}

unsigned int getPid() {
    return currentProcess->pid;
}

void blockProcess(unsigned int pid) {
    processData * process;
    process = findProcessOnList(processList, pid);
    
    if(process != NULL) {
        process->state = (process->state == READY? BLOCKED : READY);
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

static void exitProcess() {
    killProcess(currentProcess->pid);
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
    index += 52;
    buffer[index++] = '\n';
    
    toBeginingCircularList(processList);
    do
    {
        char aux[11] = {0};
        
        strcat(buffer, aux, &index);
        index += 
        
        buffer[index++] = '\n';
    } while (hasNextCircularList(processList));

    buffer[index] = '\0';
    
}

char * intToString()