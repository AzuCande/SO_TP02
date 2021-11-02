#ifndef MEMORY_LIB_H
#define MEMORY_LIB_H

#include <stddef.h>
#include <stdint.h>
#include <syscalls_asm.h>

void * myMalloc(long int size);
void myFree(void * dir);
void* sbrk(long int size);

#endif