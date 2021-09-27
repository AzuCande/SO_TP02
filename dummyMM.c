// To compile: gcc -std=c99 -pedantic -Wall dummyMM.c -o dummyMM
// To execute: ./dummyMM

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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
    char * p2 = allocateMM(500);
    //char * p3 = allocateMM(400);

    return 0;
}