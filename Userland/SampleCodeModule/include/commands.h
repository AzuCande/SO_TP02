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
#include <test_mm.h>
#include <stddef.h>

#define MAX_ARGS 10
#define MAX_ARG_LEN 32
#define BUFFER_SIZE 1024

void dateTime(char args[MAX_ARGS][MAX_ARG_LEN]);
void infoReg(char args[MAX_ARGS][MAX_ARG_LEN]);
void clear(char args[MAX_ARGS][MAX_ARG_LEN]);
void echo(char args[MAX_ARGS][MAX_ARG_LEN]);
void printmem(char args[MAX_ARGS][MAX_ARG_LEN]);
void divzero(char args[MAX_ARGS][MAX_ARG_LEN]);
void invalidopcode(char args[MAX_ARGS][MAX_ARG_LEN]);

// void mallocCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
// void freeCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
void testCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

//void sh(char args[MAX_ARGS][MAX_ARG_LEN]);
void help(char args[MAX_ARGS][MAX_ARG_LEN]);

void mem(char args[MAX_ARGS][MAX_ARG_LEN]);

void ps(char args[MAX_ARGS][MAX_ARG_LEN]);
void kill(char args[MAX_ARGS][MAX_ARG_LEN]);
void nice(char args[MAX_ARGS][MAX_ARG_LEN]);
void block(char args[MAX_ARGS][MAX_ARG_LEN]);
void unblock(char args[MAX_ARGS][MAX_ARG_LEN]);
void loop(char args[MAX_ARGS][MAX_ARG_LEN]);
void loopCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

void sem(char args[MAX_ARGS][MAX_ARG_LEN]);

void cat(char args[MAX_ARGS][MAX_ARG_LEN]);
void catCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
void wc(char args[MAX_ARGS][MAX_ARG_LEN]);
void wcCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
void filter(char args[MAX_ARGS][MAX_ARG_LEN]);
void filterCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

void pipe(char args[MAX_ARGS][MAX_ARG_LEN]);
void phylo(char args[MAX_ARGS][MAX_ARG_LEN]);
void phyloCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

void testMemCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

#endif
