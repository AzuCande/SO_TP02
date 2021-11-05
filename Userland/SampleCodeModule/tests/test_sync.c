// #include <stdint.h>
// #include <stdio.h>
// #include <processesUser.h>

// #define NULL (void*)0

// //TO BE INCLUDED
// static void endless_loop(){
//   while(1);
// }

// //my_create_process("inc", 0, 1, 1000000);

// //int createProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd
// static uint64_t my_create_process(void (*entryPoint)(int, char **), ){
//   char *argv[2];
//   argv[0] = name;
//   argv[1] = NULL;

//   int fds[2];
//   fds[0] = 0;
//   fds[1] = 0;

//   return createProcess(endless_loop, 0, argv, fds);
// }

// static uint64_t my_sem_open(int sem_id, uint64_t initialValue){
//   return (uint64_t) openSemaphoreSyscall(sem_id, initialValue);
// }

// static uint64_t my_sem_wait(int sem_id){
//   return (uint64_t) waitSemphoreSyscall(sem_id);
// }

// static uint64_t my_sem_post(int sem_id){
//   return (uint64_t) postSemaphoreSyscall(sem_id);
// }

// static uint64_t my_sem_close(int sem_id){
//   return (uint64_t) closeSemaphoreSyscall(sem_id);
// }

// #define TOTAL_PAIR_PROCESSES 2
// #define SEM_ID 101

// int64_t global;  //shared memory

// void slowInc(int64_t *p, int64_t inc){
//   int64_t aux = *p;
//   aux += inc;
//   yield();
//   *p = aux;
// }

// void inc(uint64_t sem, int64_t value, uint64_t N){
//   uint64_t i;

//   if (sem && !my_sem_open(SEM_ID, 1)){
//     printf("ERROR OPENING SEM\n");
//     return;
//   }
  
//   for (i = 0; i < N; i++){
//     if (sem) my_sem_wait(SEM_ID);
//     slowInc(&global, value);
//     if (sem) my_sem_post(SEM_ID);
//   }

//   if (sem) my_sem_close(SEM_ID);
  
//   printf("Final value: %d\n", global);
// }

// void test_sync(){
//   uint64_t i;

//   global = 0;

//   printf("CREATING PROCESSES...(WITH SEM)\n");

//   for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
//     my_create_process("inc", 1, 1, 1000000);
//     my_create_process("inc", 1, -1, 1000000);
//   }
// }

// void test_no_sync(){
//   uint64_t i;

//   global = 0;

//   printf("CREATING PROCESSES...(WITHOUT SEM)\n");

//   for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
//     my_create_process("inc", 0, 1, 1000000);
//     my_create_process("inc", 0, -1, 1000000);
//   }
// }
