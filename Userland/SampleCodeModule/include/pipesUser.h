#ifndef PIPES_USER_H
#define PIPES_USER_H

#include <syscalls_asm.h>
#include <stdint.h>

int pipe_open(uint32_t id);
int pipe_close(uint32_t id);
int pipe_read(uint32_t id);
int pipe_write(uint32_t id);

#endif