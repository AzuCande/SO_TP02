#ifndef COMMANDS_H
#define COMMANDS_H

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
void mallocCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
void freeCommand(char args[MAX_ARGS][MAX_ARG_LEN]);
void testCommand(char args[MAX_ARGS][MAX_ARG_LEN]);

#endif
