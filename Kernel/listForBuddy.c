#include <stdio.h>
#include <listForBuddy.h>

static void listInit(listBuddy *list) {
    list->prev = list;
    list->next = list;
}

static void listPush(listBuddy *list, listBuddy *entry) {
    listBuddy *prev = list->prev;
    entry->prev = prev;
    entry->next = list;
    prev->next = entry;
    list->prev = entry;
}

static void listRemove(listBuddy *entry) {
    listBuddy *prev = entry->prev;
    listBuddy *next = entry->next;
    prev->next = next;
    next->prev = prev;
}

static listBuddy *listPop(listBuddy *list) {
    listBuddy *back = list->prev;
    if (back == list) 
        return NULL;
    listRemove(back);
    return back;
}