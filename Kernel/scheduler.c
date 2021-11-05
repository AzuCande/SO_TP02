#include <scheduler.h>

typedef enum {ERROR, BLOCKED, KILLED, READY}State;

typedef struct {
    uint64_t *BP;
    uint64_t *SP;
    uint64_t *EP;
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

void endProcess(uint64_t pid, int *result){
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