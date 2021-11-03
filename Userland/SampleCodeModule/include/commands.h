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

int dateTime(char args[MAX_ARGS][MAX_ARG_LEN]);
int infoReg(char args[MAX_ARGS][MAX_ARG_LEN]);
int clear(char args[MAX_ARGS][MAX_ARG_LEN]);
int echo(char args[MAX_ARGS][MAX_ARG_LEN]);
int printmem(char args[MAX_ARGS][MAX_ARG_LEN]);
int divzero(char args[MAX_ARGS][MAX_ARG_LEN]);
int invalidopcode(char args[MAX_ARGS][MAX_ARG_LEN]);

// void mallocCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
// void freeCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
int testCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

//void sh(char args[MAX_ARGS][MAX_ARG_LEN]);
int help(char args[MAX_ARGS][MAX_ARG_LEN]);

int mem(char args[MAX_ARGS][MAX_ARG_LEN]);

int ps(char args[MAX_ARGS][MAX_ARG_LEN]);
int kill(char args[MAX_ARGS][MAX_ARG_LEN]);
int nice(char args[MAX_ARGS][MAX_ARG_LEN]);
int block(char args[MAX_ARGS][MAX_ARG_LEN]);
int unblock(char args[MAX_ARGS][MAX_ARG_LEN]);
void loop(char args[MAX_ARGS][MAX_ARG_LEN]);
int loopCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

int sem(char args[MAX_ARGS][MAX_ARG_LEN]);

void cat(char args[MAX_ARGS][MAX_ARG_LEN]);
int catCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
void wc(char args[MAX_ARGS][MAX_ARG_LEN]);
int wcCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
void filter(char args[MAX_ARGS][MAX_ARG_LEN]);
int filterCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

int pipe(char args[MAX_ARGS][MAX_ARG_LEN]);
void phylo(char args[MAX_ARGS][MAX_ARG_LEN]);
int phyloCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

int testMemCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

#endif
