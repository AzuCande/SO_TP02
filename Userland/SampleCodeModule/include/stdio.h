#ifndef STD_IO_H
#define STD_IO_H value

#include <stdint.h>

void scanf(char * buffer);
void printf(char * fmt, ...);
void setConsoleUpdateFunction(void (*f)(char *, int));
void putChar(char ch);
void setFunctionKey(int index, void(*func)());
char getChar();
uint64_t getError();

#endif
