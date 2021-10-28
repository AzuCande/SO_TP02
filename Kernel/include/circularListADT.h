#include "scheduler.h"
#include "memManager.h"

typedef struct node {
    processData * value;
    struct node * next;
    
} node;

typedef struct circularList {
    node * first;
    node * last;
    node * firstWithPriority[AMOUNT_PRIORITY];
    node * iterator;
} circularList;

circularList * newCircularList();
void freeCircularList(circularList * list);

int hasNextCircularList(circularList * list);
processData * nextCircularList(circularList * list);

int isEmptyCircularList(circularList * list);
void toBeginingCircularList(circularList * list);

void addProcessOnCircularList(circularList * list, processData * process);
processData * deleteProcessOnList(circularList * list, unsigned int pid);
processData * findProcessOnList(circularList * list, unsigned int pid);

