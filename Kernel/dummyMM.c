// To compile: gcc -std=c99 -pedantic -Wall dummyMM.c -o dummyMM
// To execute: ./dummyMM

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#define MEM_SIZE 1024

char memory[MEM_SIZE];
char * currentMM = NULL;

void initMM() {
    currentMM = memory;
}

void * allocateMM(size_t size) {

    if(currentMM == NULL){
        perror("Error memory not initialized");
        exit(1);
    }

    if(size>MEM_SIZE || memory+MEM_SIZE<currentMM+size){
        perror("Error allocating memory");
        exit(1);
    }

    void * aux = currentMM;
    currentMM += size;
    return aux;
}

int main() {

    initMM();

    char * p1 = allocateMM(200);
    // assert(p1 == NULL);
    char * p2 = allocateMM(500);
    // assert(p2 == NULL);
    //char * p3 = allocateMM(400);

    strcpy(p1, "Hello world");
    assert(strcmp(p1, "Hello world") == 0);

    strcpy(p2, p1);
    assert(strcmp(p1, p2) == 0);
    printf("Tests cleared\n");

    return 0;
}