#include <stdint.h>
#include <stdio.h>
#include <processesUser.h>
#include <stdlib.h>
#include <stddef.h>

// #define NULL (void*)0

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID 101

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  yield();
  *p = aux;
}

void inc(int argc, char *argv[]){
  uint64_t i;
  int semId;
  int sem = atoi(argv[1]);
  int value = atoi(argv[2]);
  int n = atoi(argv[3]);

  if (sem && (semId = sem_open(SEM_ID, 1)) == -1){
    printf("ERROR OPENING SEM\n");
    return;
  }
  
  for (i = 0; i < n; i++){
    if (sem) sem_wait(semId);
    slowInc(&global, value);
    if (sem) sem_post(semId);
  }
  if (sem) sem_close(semId);
  
  printf("Final value: %d\n", global);
  processKiller();
}

void test_sync(int argc, char *argv[]) {
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITH SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char *argv1[5] = {"inc+", "1", "1", "1000", NULL};
    createProcess(inc, 0, argv1, NULL);
    char *argv2[5] = {"inc-", "1", "-1", "1000", NULL};
    createProcess(inc, 0, argv2, NULL);
  }

  processKiller();
}

void test_no_sync(int argc, char *argv[]) {
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITHOUT SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char *argv1[5] = {"inc+", "0", "1", "1000", NULL};
    createProcess(inc, 0, argv1, NULL);
    char *argv2[5] = {"inc-", "0", "-1", "1000", NULL};
    createProcess(inc, 0, argv2, NULL);
  }

  processKiller();
}
