#ifndef IO_DRIVER_H
#define IO_DRIVER_H

#include <stdint.h>
#include <registers.h>
#include <scheduler.h>
#include <pipes.h>
#include <syscalls.h>
#include <keyboard_driver.h>

#define ERROR -1
#define IN 0
#define OUT 1

void saveRegisters(registerStruct * registers);
void getRegisters(uint64_t * arr);
void getBytesFromAddress(uint64_t address, uint64_t * target, uint8_t totalBytes);
void saveErrCode(uint64_t err);
uint64_t getErrCode();
int readFrom(char * buff, uint64_t size, uint64_t * count);
int writeTo(registerStruct *registers);

#endif
