#include "circularListADT.h"

static void freeNodesRec(node * n);


circularList * newCircularList() {
    circularList * list;
    list = mallocMemory(sizeof(circularList));
    if(list == NULL) {
        return NULL;
    }

    list->first = NULL;
    list->last = NULL;
    list->iterator = NULL;

    return list;
}

void addProcessOnCircularList(circularList * list, processData * process) {
    node * newNode = mallocMemory(sizeof(node));
    if(newNode == NULL) {
        return;
    }
    
    if (isEmptyCircularList(list)) {
        list->first = newNode;
        list->last = newNode;
    } else {
        list->last->next = newNode;
        list->last = list->last->next;
    }

    newNode->next = list->first;

    newNode->value = process;

    if(process->state == READY)
        list->readyCount++;

}

processData * changeProcessPriorityOnCircularList(circularList * list, unsigned int pid, unsigned int priority) {

    node * current = list->first;
    node * prev = list->last;
    int firstChecked = 0;
    int foundProcess = 0;

    node * newPosition = list->first;
    node * prevNewPos = list->last;

    while ((current != list->first || !firstChecked) && !foundProcess) {

        if(newPosition->value->priority <= priority) {
            newPosition = newPosition->next;
            prevNewPos = prevNewPos->next;
        }

        if(current->value->pid == pid) {
            foundProcess = 1;

        } else {
            firstChecked = 1;
            current = current->next;
            prev = prev->next;
        }
    }

    if(foundProcess) {
        prev->next = current->next;

        current->next = newPosition;
        prevNewPos->next = current;
    }

    return NULL;
}

processData * deleteProcessOnList(circularList * list, unsigned int pid) { //todo
    if(isEmptyCircularList(list)) {
        return NULL;
    }

    processData * deleted = NULL;

    if(list->first == list->last && list->first->value->pid == pid) {
        deleted = list->first->value;
        freeMemory(list->first);

        list->first = NULL;
        list->last = NULL;
        list->iterator = NULL;

        if(deleted->state == READY)
            list->readyCount--;

        return deleted;

    } else if(list->first != list->last) {

        node * current = list->first;
        node * previous = list->last;
        int hasToCheckFirst = 1;

        while(current != list->first || hasToCheckFirst) {
            if(current->value->pid == pid) {
                deleted = current->value;

                previous->next = current->next;
                if(current == list->first) {
                    list->first = current->next;
                }
                if(current == list->last) {
                    list->last = previous;
                }
                if(list->iterator == current) {
                    list->iterator = current->next;
                }

                freeMemory(current);

                if(deleted->state == READY)
                    list->readyCount--;

                return deleted;
            }

            hasToCheckFirst = 0;
            current = current->next;
            previous = previous->next;
        }
    }

    return NULL;
}

int isEmptyCircularList(circularList * list) {
    return list->first == NULL;
}

int hasNextCircularList(circularList * list) {
    if(isEmptyCircularList(list) || list->iterator->next == NULL || list->iterator==list->first) {
        return 0;
    }
    return 1;
}

processData * nextCircularList(circularList * list) {
    if(list->iterator == NULL) {
        return NULL;
    }

    processData * retProcess = list->iterator->value;
    if(list->iterator->next == list->first) {
        list->iterator = NULL;
    } else {
        list->iterator = list->iterator->next;
    }

    return retProcess;
}

void toBeginingCircularList(circularList * list) {
    list->iterator = list->first;
    return;
}

processData * findProcessOnList(circularList * list, unsigned int pid) {

    if(isEmptyCircularList(list)) {
        return NULL;
    }

    node * current = list->first;
    int hasToCheckFirst = 1;

    while(current!=list->first || hasToCheckFirst) {
        
        if(current->value->pid == pid) {
            return current->value;
        }

        hasToCheckFirst = 0;
        current = current->next;
    }

    return NULL;
}

static void freeNodesRec(node * n) {
    if(n==NULL) {
        return;
    }

    freeNodesRec(n->next);
    freeMemory(n);
}

void freeCircularList(circularList * list) {

    if(!isEmptyCircularList(list)) {
        list->last->next = NULL;
        freeNodesRec(list->first);
    }

    freeMemory(list);
}