#include <memorylib.h>

void * myMalloc(size_t size) 
{
    return (void * ) mallocSyscall(size);
}

void myFree(void * dir)
{
    freeSyscall(dir);
}