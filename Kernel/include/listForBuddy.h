#ifndef LIST_FOR_BUDDY_H
#define LIST_FOR_BUDDY_H

#include <stdint.h>
#include <stddef.h>

typedef struct listBuddy {
    struct listBuddy *prev;
    struct listBuddy *next;
} listBuddy;

/*static */void listInit(listBuddy *list);
/*static */void listPush(listBuddy *list, listBuddy *entry);
/*static */void listRemove(listBuddy *entry);
/*static */listBuddy *listPop(listBuddy *list);

#endif