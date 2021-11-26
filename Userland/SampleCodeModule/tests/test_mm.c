#include <tests.h>
#include <stddef.h>
#include <processesUser.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY 1024*1024*32 //Should be around 80% of memory managed by the MM

typedef struct MM_rq
{
  void *address;
  uint32_t size;
} mm_rq;

void test_mm() {
  mm_rq mm_rqs[MAX_BLOCKS];

  printf("Comienza el test de memoria:\n");
  for (int i = 0; i < 50; i++) { //while (1)
    uint8_t rq = 0;
    uint32_t total = 0;
  
    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < MAX_MEMORY)
    {
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;

      mm_rqs[rq].address = myMalloc(mm_rqs[rq].size);
      if (mm_rqs[rq].address == NULL)
      {
        printf("No memory!\n");
        return;
      }
      total += mm_rqs[rq].size;
      rq++;
    }

    
    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memoryset(mm_rqs[i].address, i, mm_rqs[i].size);

    
    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          printf("error");
    
    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        myFree(mm_rqs[i].address);
  }
  
  printf("termina el test de memoria\n");
  processKiller();
}