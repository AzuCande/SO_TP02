#include <test_mm.h>

#define NULL (void *)0
#define MAX_BLOCKS 12
#define MAX_MEMORY 1024*1024 //Should be around 80% of memory managed by the MM

typedef struct MM_rq
{
  void *address;
  uint32_t size;
} mm_rq;

void test_mm(int argc, char **argv)
{
  void * test = myMalloc(10);
  if(test == NULL)
    printf("todo mal 1\n");
  else {
    memoryset(test,1,10);
    if(memcheck(test,1,10) == 0)
      printf("todo mal 2\n");
    else
      {
        myFree(test);
        printf("hizo free\n");
        test = myMalloc(10);
        memoryset(test,2,10);
        if (memcheck(test,2,10) == 0)
          printf("todo mal 3\n");
        else
          printf("anduvo 2\n");
      }
  }
  // mm_rq mm_rqs[MAX_BLOCKS];

  // printf("Comienza el test de memoria:\n");
  // while(1)
  // {
  //   uint8_t rq = 0;
  //   uint32_t total = 0;
  
  //   // Request as many blocks as we can
  //   while (rq < MAX_BLOCKS && total < MAX_MEMORY)
  //   {
  //     mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;

  //     mm_rqs[rq].address = myMalloc(mm_rqs[rq].size);
  //     if (mm_rqs[rq].address == NULL)
  //     {
  //       printf("No memory!\n");
  //       return;
  //     }
  //     total += mm_rqs[rq].size;
  //     rq++;
  //   }

    
  //   // Set
  //   uint32_t i;
  //   for (i = 0; i < rq; i++)
  //     if (mm_rqs[i].address != NULL)
  //       memoryset(mm_rqs[i].address, i, mm_rqs[i].size);

    
  //   // Check
  //   for (i = 0; i < rq; i++)
  //     if (mm_rqs[i].address != NULL)
  //       if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
  //         printf("error");
    
  //   // Free
  //   for (i = 0; i < rq; i++)
  //     if (mm_rqs[i].address != NULL)
  //       myFree(mm_rqs[i].address);
  // }
  
}


//------------------------------------------

// #include <stdio.h>
// #include <stdlib.h>
// #include <test_util.h>
// #include <syscalls_asm.h>
// #include <stddef.h>

// #define MAX_BLOCKS 128
// #define MAX_MEMORY 1024 //Should be around 80% of memory managed by the MM

// typedef struct MM_rq{
//   void *address;
//   uint32_t size;
// }mm_rq;

// void test_mm(){
//   mm_rq mm_rqs[MAX_BLOCKS];
//   uint8_t rq;
//   uint32_t total;

//   while (1){
//     rq = 0;
//     total = 0;

//     // Request as many blocks as we can
//     while(rq < MAX_BLOCKS && total < MAX_MEMORY){
//       mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
//       mm_rqs[rq].address = mallocSyscall(mm_rqs[rq].size); // TODO: Port this call as required
// //TODO: check if NULL
//       total += mm_rqs[rq].size;
//       rq++;
//     }

//     // Set
//     uint32_t i;
//     for (i = 0; i < rq; i++){
//       if (mm_rqs[i].address != NULL){
//         memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required
//         printf("SET\n");
//       }
//     }

//     // Check
//     for (i = 0; i < rq; i++)
//       if (mm_rqs[i].address != NULL)
//         if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
//           printf("ERROR!\n"); // TODO: Port this call as required

//     // Free
//     for (i = 0; i < rq; i++)
//       if (mm_rqs[i].address != NULL)
//         freeSyscall(mm_rqs[i].address);  // TODO: Port this call as required
//   } 
// }

// // int main(){
// //   test_mm();
// //   return 0;
// // }
