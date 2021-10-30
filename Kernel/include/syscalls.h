#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
#include <registers.h>

void syscallHandler(registerStruct * registers);

#endif
