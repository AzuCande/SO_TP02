#ifndef SEMAPHORES_USER_H
#define SEMAPHORES_USER_H

#include <syscalls_asm.h>
#include <stdint.h>
#include <stdio.h>

int sem_open(uint32_t id, uint32_t initValue);
int sem_wait(uint32_t id);
int sem_post(uint32_t id);
int sem_close(uint32_t id);
void sem_status();

#endif