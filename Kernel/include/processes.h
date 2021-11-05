#ifndef PROCESSES_H
#define PROCESSES_H

#include <scheduler.h>
#include <memManager.h>
#include <lib.h>

void createProcess(void (*function)(),int foreground,char **argv, uint64_t *pid, int *fds);
void exceptionProcess();

#endif