#include <memorylib.h>

void* myMalloc(long int size){
  void *result = NULL; 
  mallocSyscall(size, &result);
  return (void*) (result);
}

void myFree(void *ptr){
  freeSyscall(ptr);
}

void* sbrk(long int size){
  void *result = NULL; 
  sbrkSyscall(size, &result);
  return (void*) (result);
}