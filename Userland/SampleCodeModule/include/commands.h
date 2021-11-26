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
#include <tests.h>
#include <stddef.h>
#include <processesUser.h>
#include <phylo.h>

#define MAX_ARGS 10
#define MAX_ARG_LEN 32
#define MAX_LINES 20
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
// int testCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

//void sh(char args[MAX_ARGS][MAX_ARG_LEN]);
int help(char args[MAX_ARGS][MAX_ARG_LEN]);

int mem(char args[MAX_ARGS][MAX_ARG_LEN]);

int ps(char args[MAX_ARGS][MAX_ARG_LEN]);
int killCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
int niceCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
int blockCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
int unblockCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
int loopCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

int sem(char args[MAX_ARGS][MAX_ARG_LEN]);

// void cat(char args[MAX_ARGS][MAX_ARG_LEN]);
int catCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
int wcCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
int filterCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

int pipe(char args[MAX_ARGS][MAX_ARG_LEN]);
int phyloCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

int testMemCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
int testProcessesCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
int testPrioCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

int testSync(char args[MAX_ARGS][MAX_ARG_LEN]);
int testNoSync(char args[MAX_ARGS][MAX_ARG_LEN]);

#endif
