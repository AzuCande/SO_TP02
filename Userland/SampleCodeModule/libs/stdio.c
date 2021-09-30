#ifndef STD_IO
#define STD_IO

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <syscalls_asm.h>
#include <stdGraphics.h>

#define PRINTF_BUFFER_SIZE 128
#define STD_BUFFER_SIZE 256

char std_in[STD_BUFFER_SIZE] = {0};
char std_out[STD_BUFFER_SIZE] = {0};
static int updateConsoleInitialized = 0;

void (*updateConsolePointer)(char *, int);

void scanf(char * buffer) {
    int aux = 0;
    int index = 0;
    while (aux != '\n') {
        if(aux) {
            buffer[index++] = aux;
            putChar(aux);
            aux = getChar();
        }
    }
    putChar('\n');
}

void printf(char * fmt, ...) {
    va_list vl;
  va_start(vl, fmt);
  char * auxPtr;
  char buffer[128] = {0};
  char tmp[20];
  int i = 0, j = 0;
  while (fmt && fmt[i]) {
    if (fmt[i] == '%') {
      i++;
      switch(fmt[i]) {
        case 'c':
        buffer[j++] = (char)va_arg( vl, int );
        break;
        case 'd':
        intToString(va_arg( vl, int ), tmp);
        strcpy(&buffer[j], tmp);
        j+=strlen(tmp);
        break;
        case 's':
        auxPtr = va_arg(vl, char*);
        strcpy(&buffer[j],auxPtr);
        j+=strlen(auxPtr);
        break;
        case 'x':
        intToBase(va_arg( vl, int ),16, tmp);
        strcpy(&buffer[j], tmp);
        j+=strlen(tmp);
        break;
        case 'X': //long hexa
        intToBase(va_arg( vl, uint64_t ),16, tmp);
        strcpy(&buffer[j], tmp);
        j+=strlen(tmp);
        break;
      }
    } else {
      buffer[j++] = fmt[i];
    }
    i++;
  }
  updateConsolePointer(buffer, j);
}

// int readKeyboard(char * buffer, int size) {
//     if(size == 0)
//         return 0;
//     uint64_t aux;
//     isKeyboardEmptySyscall(&aux);
//     if(aux) {
//         //readKeyboardSysCall(buffer, (uint8_t) size);
//         return 1;
//     }
//     return 0;
// }

void setConsoleUpdateFunction(void (*f)(char *, int)) {
  updateConsolePointer = f;
  updateConsoleInitialized = 1;
};

void putChar(char ch) {
  updateConsolePointer(&ch, 1);
}

char getChar() {
  char ch = 0;
  uint64_t count;
  readKeyboardSysCall(&ch, 1, &count);
  return ch;
}

void setFunctionKey(int index, void(*func)()) {
  setFunctionKeyMethodSyscall((uint64_t)index, func);
}

uint64_t getError() {
  uint64_t err = 32;
  readErrorSyscall(&err);
  return err;
}


#endif
