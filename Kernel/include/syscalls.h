#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <registers.h>
#include <processes.h>
#include <pipes.h>
#include <semaphores.h>
#include <keyboard_driver.h>
#include <video_driver.h>
#include <timer_driver.h>
#include <date_driver.h>
#include <font.h>
#include <IO_driver.h>
#include <exceptions.h>
#include <memManager.h>
#include <scheduler.h>
#include <memoryDriver.h>
#include <lib.h>

int syscallHandler(registerStruct * registers);
void writeStr(registerStruct * registers);

#endif
