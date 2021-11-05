#include <scheduler.h>
// #include <listADT.h>

typedef enum {ERROR, BLOCKED, KILLED, READY}State;

typedef struct {
    uint64_t *BP;
    uint64_t *SP;
    uint64_t *EP; // End Pointer
    char **argv;
    uint64_t argc; 
    uint64_t pid;
    State state;
    char *name;
    uint8_t foreground;
    uint8_t priority;
    int fds[2];
}process;

int equals(void* n1, void* n2){
    process aux1 = *((process*)n1);
    process aux2 = *((process*)n2);
    return aux1.pid == aux2.pid;
}

listADT processList = NULL;
listADT fgBlocked = NULL; 

process *current = NULL;

int currentCountdownPriority;

//private:
void changeProcess();
int changeState(uint64_t pid , State state);
void addTitle(char *dest,int *j);
void strconcat(char *dest, char *src, int *j);
void movIndex(char *dest,int to, int *from);


uint64_t * scheduler(uint64_t *currentProces){
    if (processList == NULL)
        return currentProces;
    
    if (current==NULL) {
        current = (process *)next(processList);
        return (*current).SP;
    }
    
    (*current).SP = currentProces;

    changeProcess();

    return (*current).SP;
}

void changeProcess(){
    if (currentCountdownPriority && current->state == READY) {
        currentCountdownPriority--;
    } else {
        current = (process *)next(processList);
        while ((*current).state!=READY) {
            current = (process *)next(processList);
        }
        currentCountdownPriority = current->priority;
    }
}

void nice(uint64_t pid, uint64_t priority, int *result){
    process *processAux = mallocMemory(sizeof(process));
    if (processAux==NULL) {
        *result = -1;
        return;
    }
    process *toFree = processAux;
    (*processAux).pid = pid;
    processAux = (process*)getElem(processList, processAux);
    if (processAux!=NULL) {
        processAux->priority = priority; 
        (*result) = priority; 
        freeMemory(toFree);
        return; 
    }  
    freeMemory(toFree);
    (*result) = -1;
}

void yield(){
    currentCountdownPriority = 0;
    _hlt();
}

void addProcess(uint64_t *currentProces, char *name,uint64_t *pid,uint64_t *ep,int foreground, uint64_t argc, char** argv, int *fds){
    if(processList == NULL){
        processList = newList(sizeof(process),equals);
        if(processList == NULL)
            return;
    }
    if (fgBlocked == NULL) {
        fgBlocked = newList(sizeof(int),NULL);
        if (fgBlocked == NULL)
            return;
    }
    process newProcess;
    newProcess.SP = currentProces;
    newProcess.pid = size(processList);
    newProcess.state = READY;
    newProcess.foreground = foreground;
    newProcess.BP = currentProces;
    newProcess.EP = ep;
    newProcess.name = name;
    newProcess.priority = 3; 
    newProcess.argc = argc;
    newProcess.argv = argv;
    newProcess.fds[0] = fds ? fds[0] : 0;
    newProcess.fds[1] = fds ? fds[1] : 1;
    
    insertBeforeNext(processList, &newProcess);
    //BLOCK al foreground anterior
    if ( current != NULL && newProcess.foreground) {
        insert(fgBlocked,(void*)&(current->pid));
        int aux;
        blockProcess(current->pid,&aux);
    }
    (*pid) = newProcess.pid;
    return;
}

void exceptionProcess(){
    freeList(processList);
    freeList(fgBlocked);
    fgBlocked = NULL; 
    current = NULL;
    processList = NULL;
}

void freeEP(void * aux1) {
    process *aux = (process *)aux1;
    freeMemory(aux->EP);
    freeMemory(aux->name);
    for (int i = 0; i < aux->argc; i++)
        freeMemory(aux->argv[i]);
    freeMemory(aux->argv);
}

void endProcessWrapper(uint64_t pid, int *result){
    if(pid > 0){
        process aux;
        aux.pid = pid;
        process aux2 = *(process*)getElem(processList,&aux);
        if (aux2.foreground==1) {
            void* check= pop(fgBlocked);
            int pidA = *((int*) check);
            int res;
            unlockProcess(pidA,&res);
        }
        if (deleteElem(processList, &aux,freeEP)){
            (*result) = 0;
            if (current->pid == pid) yield();
            return;
        }
    }
    (*result) = -1;
}

void getPid(uint64_t *pid) {
    (*pid) =  (*current).pid;
    return;
}

void ps(char *result){
    process **arrayOfProcess = ListToArray(processList);
    int i = 0;
    int j = 0;
    char statesA[4][8] = {"Error","Blocked","Killed","Ready"}; 
    char title[51] = "pid   prio   state     F/B     SP     BP      name";
    strconcat(result,title,&j);
    movIndex(result,59,&j);
    result[j++] = '\n';
    while (arrayOfProcess[i] != NULL) {
        result[j++] = ' ';
        char pid[3], prio[10],SP[10],BP[10];
        itoa(arrayOfProcess[i]->pid,pid, 10);
        strconcat(result,pid,&j);
        movIndex(result,6,&j);
        itoa(arrayOfProcess[i]->priority,prio, 10);
        strconcat(result,prio,&j);
        movIndex(result,13,&j);
        strconcat(result,statesA[arrayOfProcess[i]->state],&j);
        movIndex(result,23,&j);
        result[j++] = arrayOfProcess[i]->foreground ? 'F' : 'B';
        movIndex(result,31,&j);
        itoa((uint64_t)arrayOfProcess[i]->SP,SP, 16);
        strconcat(result,SP,&j);
        movIndex(result,38,&j);
        itoa((uint64_t)arrayOfProcess[i]->BP, BP, 16);
        strconcat(result,BP,&j);
        movIndex(result,46,&j);
        strconcat(result,arrayOfProcess[i]->name,&j);
        movIndex(result,59,&j);
        result[j++] = '\n';
        i++;
    }
    result[j] = 0;
    freeMemory(arrayOfProcess);
}

void movIndex(char *dest,int to, int *from){
    for ( ; (*from)%60 < to; (*from)++) {
        dest[(*from)] = ' ';
    }
}

void strconcat(char *dest, char *src, int *j) {
    int k = 0;
    for ( k = 0 ; src[k] ;  (*j)++, k++ ) {
        dest[*j] = src[k];
    }
}

void blockProcess(uint64_t pid, int *result){
    if ( pid==0 && !size(fgBlocked) ) {
        (*result) = -1;
        return;
    }
    if (changeState(pid, BLOCKED)) {
        (*result) = 0;
        if (current->pid == pid) 
            yield();
        return;
    }
    (*result) = -1;
}

void unlockProcess(uint64_t pid, int *result){
    if (changeState(pid, READY)) {
        (*result) = 0;
        return;
    }
    (*result) = -1;
}

int changeState(uint64_t pid , State state){
    process *processAux = mallocMemory(sizeof(process));
    if (processAux == NULL) return 0;
    process *toFree = processAux;
    (*processAux).pid = pid;
    processAux = (process*)getElem(processList, processAux);
    if (processAux!=NULL) {
        (*processAux).state = state;  
        freeMemory(toFree);
        return 1; 
    }
    freeMemory(toFree);
    return 0;
}

// Returns read fd from current process
int currentReadFd() {
    if(current) {
        return current->fds[0];
    }
    return -1;
}

// Returns write fd from current process
int currentWriteFd() {
    if(current) {
        return current->fds[1];
    }
    return -1;
}

int isCurrentFg() {
    if(current) {
        return current->foreground;
    }
    return -1;
}
























//----------------------------------------
// #include <scheduler.h>
// #include <circularListADT.h>

// #define START_PID_BAPTIZER 1

// static void dummyFunc(int argc, char **argv);
// static void wrapper(void (*entryPoint) (int, char**), int argc, char ** argv);
// static void argscpy(char **destination, char **source, int size);


// unsigned int pidBaptizer = START_PID_BAPTIZER;
// processData * currentProcess = NULL; // TODO: revisar esto, estaba como currentProcess = 0
// processData * dummyProcess;
// circularList * processList;
// static uint64_t processCountdown;

// void initScheduler() {
//     processList = newCircularList();
//     if(processList == NULL) {
//         return;
//     }

//     // Runs by default a dummy process
//     char *argv[] = {"Dummy Process"};
//     createProcess((void *) &dummyFunc, 1, argv, 0, 0);
//     dummyProcess = nextCircularList(processList);
//     // The dummy process should not be in the list
//     deleteProcessOnList(processList, dummyProcess->pid);
// }

// static void dummyFunc(int argc, char **argv) {
//     while(1) {
//         _hlt();
//     }
// }

// int createProcess(void (*entryPoint) (/*int, */char **), int argc, char **argv, unsigned int foreground, int *fds) {

//     if(entryPoint == NULL) {
//         return -1;
//     }
    
//     processData *process = mallocMemory(sizeof(processData));
//     // Check if there is enough memory available
//     if( process == NULL) { 
//         return -1;
//     }

//    if(setProcessData(process, pidBaptizer, argv[0], foreground, fds)) {
//        freeMemory(process);
//        return -1;
//    }
//     pidBaptizer++;

//     char **args = mallocMemory(sizeof(char *) * argc);
//     // Fijarse si era NULL o 0
//     if(args == NULL) {
//         return -1;
//     }
//     argscpy(args, argv, argc);

//     process->argc = argc;
//     process->argv = args;

//     setNewStackFrame(entryPoint, argc, args, process->bp);

//     process->state = READY;

//     addProcessOnCircularList(processList, process);
    
//     // Check if it is necessary to block parent
//     if(process->foreground && process->ppid) {
//         blockProcess(process->ppid);
//     }

//     return process->pid;
// }

// int setProcessData(processData * p, unsigned int pid, char * name, unsigned int foreground, int *fds) {

//     p->pid = pid;

//     // Checks if this is a child process
//     if(currentProcess == NULL) {
//         p->ppid = 0;
//     } else {
//         p->ppid = currentProcess->pid;
//     }

//     p->name[0] = 0;
//     strcpy(p->name, name);

//     // Checks if there is parent
//     if(currentProcess == NULL) {
//         p->foreground = foreground;
//     } else {
//         if(currentProcess->foreground)
//             p->foreground = foreground;
//         else
//             p->foreground = 0;
//     }

//     p->fds[0] = (fds[0]) ? fds[0] : 0;
//     p->fds[1] = (fds[1]) ? fds[1] : 1;

//     p->priority = DEFAULT_PRIORITY;
//     p->state = READY;

//     p->bp = mallocMemory(STACK_SIZE);
//     if(p->bp == NULL) 
//         return -1;
    
//     p->bp = (void *) ((char*) p->bp + STACK_SIZE - 1);
//     p->sp = (void *) ((registerStruct *) p->bp - 1);

//     return 0;
// }

// void setNewStackFrame(void (*entryPoint) (/*int, */char **), int argc, char **argv, void *bp) {
//     registerStruct *stackFrame = (registerStruct *) bp - 1;

//     stackFrame->r15 = 0x001;
//     stackFrame->r14 = 0x002;
//     stackFrame->r13 = 0x003;
//     stackFrame->r12 = 0x004;
//     stackFrame->r11 = 0x005;
//     stackFrame->r10 = 0x006;
//     stackFrame->r9 = 0x007;
//     stackFrame->r8 = 0x008;
//     // Data to run the process
//     stackFrame->rsi = (uint64_t) argc;
//     stackFrame->rdi = (uint64_t) entryPoint;
//     stackFrame->rbp = 0x00B;
//     stackFrame->rdx = (uint64_t) argv;
//     stackFrame->rcx = 0x00D;
//     stackFrame->rbx = 0x00E;
//     stackFrame->rax = 0x00F;
//     stackFrame->rip = (uint64_t) wrapper;
//     stackFrame->cs = 0x008;
//     stackFrame->flags = 0x202;
//     stackFrame->rsp = (uint64_t) bp;
//     stackFrame->ss = 0x000;
// }

// static void wrapper(void (*entryPoint) (int, char**), int argc, char ** argv) {
//     entryPoint(argc, argv);
//     exitProcess();
// }

// static void argscpy(char **destination, char **source, int size) {
//     for(int i = 0; i < size; i++) {
//         destination[i] = mallocMemory(sizeof(char) * (strlen(source[i]) + 1));
//         strcpy(source[i], destination[i]);
//     }
// }

// void *scheduler(uint64_t *sp) {
//     if(currentProcess != NULL) {
        
//         if(currentProcess->state == READY && processCountdown > 0) {
//             processCountdown--;
//             return currentProcess->sp;
//         }

//         currentProcess->sp = sp;

//         // The dummy process should never be pushed to the list
//         if(currentProcess->pid != dummyProcess->pid) {
//             // Check the process' state
//             if(currentProcess->state == KILLED) {
//                 // Check process' parent
//                 processData *parent = findProcessOnList(processList, currentProcess->ppid);
//                 if(parent != NULL && currentProcess->foreground && parent->state == BLOCKED) {
//                     // Unblocks process (do not mind the function's name)
//                     blockProcess(parent->pid);
//                 }
//                 // Now free memory of child
//                 freeProcess(currentProcess);
//             } 
//             // else {
//             //     addProcessOnCircularList(processList, currentProcess);
//             // }
//             // Check round robin algorythm
//         }

//     }

//     // Obtain process to run
//     if(processList->readyCount > 0) {
//         currentProcess = nextCircularList(processList);
//         // Current process should be in READY state
//         while(currentProcess->state != READY) {
//             if(currentProcess->state == KILLED) { // TODO: revisar, estaba como currentProcess == KILLED y tiraba warning, hay que ver si currentProcess->state == KILLED funciona
//                 deleteProcessOnList(processList, currentProcess->pid);
//                 freeProcess(currentProcess);
//             }
//             // If the process is BLOCKED, continue the cycle
//             currentProcess = nextCircularList(processList);
//         }

//     } else {
//         // If there is no READY process available
//         currentProcess = dummyProcess;
//     }
//     processCountdown = currentProcess->priority;
//     return currentProcess->sp;
// }

// void changeProcessPriority(unsigned int pid, unsigned int assignPriority) {
//     if(assignPriority>MIN_PRIORITY || assignPriority<MAX_PRIORITY) {
//         return;
//     }

//     processData * p = changeProcessPriorityOnCircularList(processList,pid,assignPriority); // TODO: fix cast warning
//     p->priority = assignPriority;
// }

// unsigned int getPid() {
//     return currentProcess->pid;
// }

// void blockProcess(unsigned int pid) {

//     processData * process;
//     process = findProcessOnList(processList, pid);

//     if(process != NULL) {
//         process->state = BLOCKED;
//         _timerTick();
//     }
// }

// void unblockProcess(unsigned int pid) {

//     processData * process;
//     process = findProcessOnList(processList, pid);

//     if(process != NULL) {
//         process->state = READY;
//     }
// }

// void freeProcess(processData *process) {
//     for(int i = 0; i < process->argc; i++) {
//         freeMemory(process->argv[i]);
//     }
//     freeMemory(process->argv);
//     freeMemory((void *) ((char *)process->bp - STACK_SIZE + 1));
//     freeMemory((void *) process);
// }

// void killProcess(unsigned int pid) {
//     processData *toKill;
//     toKill = deleteProcessOnList(processList, pid);

//     if(toKill == NULL)
//         return;

//     freeProcess(toKill);

//     _timerTick();
// }

// void resignCPU() {

//     processCountdown = 0;
//     _timerTick();
// }

// // Returns read fd from current process
// int currentReadFd() {
//     if(currentProcess) {
//         return currentProcess->fds[0];
//     }
//     return -1;
// }

// // Returns write fd from current process
// int currentWriteFd() {
//     if(currentProcess) {
//         return currentProcess->fds[1];
//     }
//     return -1;
// }

// int isCurrentFg() {
//     if(currentProcess) {
//         return currentProcess->foreground;
//     }
//     return -1;
// }

// void exitProcess() {
//     killProcess(currentProcess->pid);
//     _timerTick();
// }

// void printProcessList(char * buffer) { 
    
//     if(isEmptyCircularList(processList)) {
//         buffer[0] = '\0';
//         return;
//     }

//     char header[41] = "PID\t Name\t Priority\t State\t FG\t SP\t BP\n";
//     char states[4][8] = {"Ready", "Blocked", "Killed"};
//     unsigned int index = 0;

//     strcat(buffer, header, &index);
//     processData * currentP;
    
//     toBeginingCircularList(processList);

//     while(( currentP = nextCircularList(processList)) != NULL) {
//         char aux[11] = {0};
        
//         itoa(currentP->pid,aux,10);
//         strcat(buffer, aux, &index);

//         strcat(buffer,currentP->name,&index);
        
//         itoa(currentP->priority,aux,10);
//         strcat(buffer, aux, &index);
        
//         strcat(buffer,states[currentP->state], &index);

//         strcat(buffer,currentP->foreground? "FG":"BG", &index);
        
//         itoa(*(currentP->sp),aux,16);
//         strcat(buffer,aux,&index);
        
//         itoa(*(currentP->bp),aux,16);
//         strcat(buffer,aux,&index);

//         buffer[index++] = '\n'; 
//     }

//     buffer[index] = '\0';
// }

