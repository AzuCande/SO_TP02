#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
#include <registers.h>
#include <processes.h>

void syscallHandler(registerStruct * registers);
void writeStr(registerStruct * registers);

#endif
