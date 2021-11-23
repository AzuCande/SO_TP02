#ifndef SHELLS_H
#define SHELLS_H

#include <stdGraphics.h>
#include <colors.h>
#include <stdint.h>
#include <stdio.h>
#include <commands.h>
#include <stdlib.h>
#include <syscalls_asm.h>

#define NO_BUILTIN_SIZE 10
#define ERROR -1

void drawShellLines();
void init_shell();
void clearAll();

#endif
