#ifndef SYSCALLS_ASM_H
#define SYSCALLS_ASM_H

#include <stdint.h>

#define MAX_ARG_LEN 32

void drawStringSysCall(char * buff, uint64_t size, uint64_t x, uint64_t y, uint64_t fontColor, uint64_t backgroundColor, uint64_t fontSize, uint64_t alphaBackground);
void isKeyboardEmptySyscall(uint64_t * target);
void readKeyboardSysCall(char * buffer, uint64_t size, uint64_t * count);
void clearDisplaySyscall(uint64_t color);
void drawRectSyscall(uint64_t xi, uint64_t yi, uint64_t width, uint64_t height, uint64_t color);
void setFunctionKeyMethodSyscall(uint64_t index, void(*)());

void getTimeSyscall(uint8_t mode, uint8_t *target);
void getTicksSyscall(uint64_t *ticks);
void getRegistersSyscall(uint64_t * registers);
void getMemSyscall(uint64_t, uint64_t*, uint8_t);
void readErrorSyscall(uint64_t * err);

void * mallocSyscall(uint64_t size, void ** result);
void freeSyscall(void * free);
void sbrkSyscall(uint64_t size, void** result);
void memSyscall(char *str, int strSize);

void createProcessSyscall(void (*)(),int fg,char **,uint64_t *newPid, int *fds);
void killSyscall(uint64_t pid, int *result);
void getPidSyscall(uint64_t *);
void psSyscall(char *);
void blockSyscall(uint64_t pid, int *result);
void unblockSyscall(uint64_t pid, int *result);
void niceSyscall(uint64_t pid, uint64_t priority, int *result);
void yieldSyscall();

void pipeOpenSyscall(uint32_t id, int *toReturn);
void pipeCloseSyscall(uint32_t id, int *toReturn);
void pipeReadSyscall(uint32_t id, int *toReturn);
void pipeWriteSyscall(uint32_t id, char *string, int *toReturn);
int createPipeSyscall(uint32_t id);
void pipeSyscall(char * buffer);

void openSemaphoreSyscall(uint32_t sem_id, uint32_t initialValue, int *toReturn);
void waitSemaphoreSyscall(uint32_t id, int *toReturn);
void postSemaphoreSyscall(uint32_t id, int *toReturn);
void closeSemaphoreSyscall(uint32_t id, int *toReturn);
void semSyscall(char * buffer);

#endif