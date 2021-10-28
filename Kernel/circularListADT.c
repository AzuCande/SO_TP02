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

    list->iterator = newNode;
    newNode->next = list->first;

    newNode->value = process;

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

        return deleted;

    } else if(list->first != list->last) {

        node * current = list->first;
        node * previus = list->last;
        int hasToCheckFirst = 1;

        while(current != list->first || hasToCheckFirst) {
            if(current->value->pid == pid) {
                deleted = current->value;

                previus->next = current->next;
                if(current == list->first) {
                    list->first = current->next;
                }
                if(current == list->last) {
                    list->last = previus;
                }
                if(list->iterator == current) {
                    list->iterator = current->next;
                }

                freeMemory(current);

                return deleted;
            }

            hasToCheckFirst = 0;
            current = current->next;
            previus = previus->next;
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
    if(!hasNextCircularList(list)) {
        return NULL;
    }
    processData * retProcess = list->iterator->value;
    list->iterator = list->iterator->next;

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