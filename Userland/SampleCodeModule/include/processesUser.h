#ifndef PROCESSES_USER_H
#define PROCESSES_USER_H

#include <syscalls_asm.h>
#include <stdint.h>

uint64_t createProcess(void (*function)(),int fg,char **argv, int *fds);
int nice(uint64_t pid, uint64_t prio);
int block(uint64_t pid);
int unblock(uint64_t pid);
int kill(uint64_t pid);
uint64_t getPid();
void yield();
void processKiller();

#endif