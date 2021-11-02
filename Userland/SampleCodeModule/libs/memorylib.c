#include <memorylib.h>

void* myMalloc(long int size){
  void *result = NULL; 
  result = mallocSyscall(size);
  return (void*) (result);
}

void myFree(void *ptr){
  freeSyscall(ptr);
}

void* sbrk(long int size){
  void * result = NULL; 
  
  sbrkSyscall((uint64_t) size, &result);
  return (void*) (result);
}