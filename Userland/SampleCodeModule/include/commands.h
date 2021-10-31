#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdGraphics.h>
#include <time.h>
#include <inforeg.h>
#include <shells.h>
#include <syscalls_asm.h>
#include <exceptions_asm.h>
#include <stddef.h> // TODO: Borrar despues

#define MAX_ARGS 10
#define MAX_ARG_LEN 32

void dateTime(char args[MAX_ARGS][MAX_ARG_LEN]);
void infoReg(char args[MAX_ARGS][MAX_ARG_LEN]);
void help(char args[MAX_ARGS][MAX_ARG_LEN]);
void clear(char args[MAX_ARGS][MAX_ARG_LEN]);
void echo(char args[MAX_ARGS][MAX_ARG_LEN]);
void printmem(char args[MAX_ARGS][MAX_ARG_LEN]);
void divzero(char args[MAX_ARGS][MAX_ARG_LEN]);
void invalidopcode(char args[MAX_ARGS][MAX_ARG_LEN]);

// void mallocCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
// void freeCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
void testCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

void mem(char args[MAX_ARGS][MAX_ARG_LEN]);

void ps(char args[MAX_ARGS][MAX_ARG_LEN]);
void kill(char args[MAX_ARGS][MAX_ARG_LEN]);
void nice(char args[MAX_ARGS][MAX_ARG_LEN]);
void block(char args[MAX_ARGS][MAX_ARG_LEN]);
void sem(char args[MAX_ARGS][MAX_ARG_LEN]);
void pipe(char args[MAX_ARGS][MAX_ARG_LEN]);
void loop(char args[MAX_ARGS][MAX_ARG_LEN]);

#endif
