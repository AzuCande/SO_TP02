#include "include/scheduler.h"

#define START_PID_BAPTIZER 1

static void dummyFunc(int argc, char **argv);
static void wrapper(void (*entryPoint) (int, char**), int argc, char ** argv);
static int argscpy(char **destination, char **source, int size);
static void exitProcess();


unsigned int pidBaptizer = START_PID_BAPTIZER;
processData * currentProcess = 0;
processData * dummyProcess;
circularList * processList;
static uint64_t processCountdown;

void initScheduler() {
    processList = newCircularList();
    if(processList == NULL) {
        return;
    }

    // Runs by default a dummy process
    char *argv[] = {"Dummy Process"};
    createProcess((void *) &dummyFunc, 1, argv, 0);
    ProcessData *dummyP = nextCircularList(processList);
    dummyProcess = deleteProcessOnList(processList, dummyP->pid);
}

static void dummyFunc(int argc, char **argv) {
    while(1) {
        _hlt();
    }
}

int createProcess(void (*entryPoint) (int, char **), int argc, char **argv, unsigned int foreground) {

    if(entryPoint == NULL) {
        return -1;
    }
    
    processData *process;
    // Check if there is enough memory available
    if( (process = mallocMemory(sizeof(processData)) == NULL)) {
        return -1;
    }

   if(setProcessData(process, pidBaptizer, argv[0], foreground)) {
       freeMemory(process);
       return -1;
   }
    pidBaptizer++;

    char **args = mallocMemory(sizeof(char *) * argc);
    // Fijarse si era NULL o 0
    if(args == NULL) {
        return -1;
    }
    argscpy(args, argv, argc);

    process->argc = argc;
    process->argv = args;

    setNewStackFrame(entryPoint, argc, args, process->bp);

    process->state = READY;

    addProcessOnCircularList(processList, process);
    
    // Check if it is necessary to block parent
    if(process->foreground && process->ppid) {
        blockProcess(process->ppid);
    }

    return process->pid;
}

int setProcessData(processData * p, unsigned int pid, char * name, unsigned int foreground) {

    p->pid = pid;

    // Checks if this is a child process
    if(currentProcess == NULL) {
        p->ppid = 0;
    } else {
        p->ppid = currentProcess->pid;
    }

    p->name[0] = 0;
    strcpy(p->name, name);

    // Checks if there is parent
    if(currentProcess == NULL) {
        p->foreground = foreground;
    } else {
        if(currentProcess->foreground)
            p->foreground = foreground;
        else
            p->foreground = 0;
    }

    p->priority = DEFAULT_PRIORITY;
    p->state = READY;

    p->bp = mallocMemory(STACK_SIZE);
    if(p->bp == NULL) 
        return -1;
    
    p->sp = (void *) ((registerStruct *) p->bp - 1);
}

void setNewStackFrame(void (*entryPoint) (int, char **), int argc, char **argv, void *bp) {
    registerStruct *stackFrame = (registerStruct *) bp - 1;

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
    stackFrame->rsp = (uint64_t) bp;
    stackFrame->ss = 0x0;
}

static void wrapper(void (*entryPoint) (int, char**), int argc, char ** argv) {
    entryPoint(argc, argv);
    exitProcess();
}

static int argscpy(char **destination, char **source, int size) {
    for(int i = 0; i < size; i++) {
        destination[i] = mallocMemory(sizeof(char) * (strlen(source[i]) + 1));
        strcpy(source[i], destination[i]);
    }
}

void *scheduler(uint64_t *sp) {
    if(currentProcess != NULL) {
        
        if(currentProcess->state == READY && processCountdown > 0) {
            processCountdown--;
            return currentProcess->sp;
        }

        currentProcess->sp = sp;

        // The dummy process should never be pushed to the list
        if(currentProcess->pid != dummyProcess->pid) {
            // Check the process' state
            if(currentProcess->state == KILLED) {
                // Check process' parent
                processData *parent = findProcessOnList(processList, currentProcess->ppid);
                if(parent != NULL && currentProcess->foreground && parent->state == BLOCKED) {
                    // Unblocks process (do not mind the function's name)
                    unblockProcess(parent->pid);
                }
                // Now free memory of child
                freeProcess(currentProcess);
            } else {
                addProcessOnCircularList(processList, currentProcess);
            }
        }

    }

    // Ahora debo obtener el proceso a cambiar
    if(processList->readyCount > 0) {
        currentProcess = nextCircularList(processList);
        // Current process should be in READY state
        while(currentProcess->state != READY) {
            if(currentProcess == KILLED) {
                deleteProcessOnList(processList, currentProcess->pid);
                freeProcess(currentProcess);
            }
            // If the process is BLOCKED, continue the cycle
            currentProcess = nextCircularList(processList);
        }
        // Once found, delete it from the list
        deleteProcessOnList(processList, currentProcess->pid);
    } else {
        // If there is no READY process available
        currentProcess = dummyProcess;
    }
    processCountdown = currentProcess->priority;
    return currentProcess->sp;
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

    if(process != NULL && process->state == READY) {
        process->state = BLOCKED;
        _timerTick();
        
    } else {
        process->state = READY;
    }
}

void killProcess(unsigned int pid) {
    processData *toKill;
    toKill = deleteProcessOnList(processList, pid);

    if(toKill == NULL)
        return;

    for(int i=0; toKill->argc; i++) {
        freeMemory(toKill->argv[i]);
    }

    freeMemory(toKill);

    _timerTick();
}

void resignCPU() {

    processCountdown = 0;
    _timerTick();
}

static void exitProcess() {
    killProcess(currentProcess->pid);
    _timerTick();
}

void printProcessList(char * buffer) { 
    
    if(isEmptyCircularList(processList)) {
        buffer[0] = '\0';
        return;
    }

    char header[51] = "PID    Name    Priority    State    FG    SP    BP";
    char states[4][8] = {"Ready", "Blocked", "Killed"};
    unsigned int index = 0;

    strcat(buffer, header, &index);
    index += 52;
    buffer[index++] = '\n';
    processData * currentP;
    
    toBeginingCircularList(processList);

    while(( currentP = nextCircularList(processList)) != NULL) {
        char aux[11] = {0};
        
        intToString(aux,currentP->pid);
        strcat(buffer, aux, &index);

        strcat(buffer,currentP->name,&index);
        
        intToString(aux,currentP->priority);
        strcat(buffer, aux, &index);
        
        strcat(buffer,states[currentP->state], &index);

        strcat(buffer,currentP->foreground? "FG":"BG", &index);

        intToBaseString(16,aux,currentP->sp);
        strcat(buffer,aux,&index);
        
        intToBaseString(16,aux,currentP->bp);
        strcat(buffer,aux,&index);

        buffer[index++] = '\n'; 
    }

    buffer[index] = '\0';
}

