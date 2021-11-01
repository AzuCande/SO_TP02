#ifndef SYSCALLS
#define SYSCALLS

#include <stdint.h>
#include <registers.h>
#include <keyboard_driver.h>
#include <video_driver.h>
#include <timer_driver.h>
#include <date_driver.h>
#include <font.h>
#include <IO_driver.h>
#include <exceptions.h>
#include <memManager.h>
#include <scheduler.h>
#include <pipes.h>

void writeStr(registerStruct * registers);
void getDateInfo(uint8_t mode, uint8_t * target);

void syscallHandler(registerStruct * registers) {
  uint64_t option = registers->rax;

  switch(option) {
    //READ KEYBOARD
    case 0:
    //rdi -> puntero a buffer
    //rsi -> uint8_t size
    //rdx -> putero a uint64_t count
    readKeyboard((char *)registers->rdi, (uint8_t) registers->rsi, (uint64_t*)registers->rdx);
    break;

    //WRITE STR
    case 1:
    //rdi -> puntero a buffer
    //rsi -> size
    //rdx -> x
    //rcx -> y
    //r8 -> fontColor
    //r9 -> backgroundColor
    //r10 -> fontSize
    //r11 -> alphaBackground
    writeStr(registers);
    break;

    case 2:
    //rdi color
      clearDisplay((uint64_t) registers->rdi);
      break;
    case 3:
    // rdi xstart , rsi ystart, rdx xend, rcx yend, r8 color
      drawLine((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t) registers->rdx,(uint64_t) registers->rcx,(uint64_t) registers->r8);
      break;
    case 4:
    // r9 xi, r8 yi, rsi color
      drawPixel((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t) registers->rdx);
      break;
    case 5:
    // rdi xi, rsi yi , rdx width, rc8 height , r8 color
      drawRectangle((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t) registers->rdx,(uint64_t) registers->rcx,(uint64_t) registers->r8);
      break;
    case 6:
    // rdi xi, rsi yi, rdx puntero a matriz, rcx width, r8 height , r9 size
      drawMatrix((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t *) registers->rdx,(uint64_t) registers->rcx,(uint64_t) registers->r8,(uint64_t) registers->r9);
      break;
    case 7:
      * ((uint64_t *)registers->rdi) = getTicks();
      break;
    case 8:
    //rdi -> mode
    //rsi -> puntero a entero
    getDateInfo((uint8_t) registers->rdi, (uint8_t *) registers->rsi);
    break;

    case 9: //Obtener los registros
    //rdi -> puntero a vector de uint64_t para guardar los valores
    getRegisters((uint64_t*)registers->rdi);
    break;

    case 10:
    //rdi -> direccion de la que se desea leer
    //rsi -> direccion del buffer para guardar
    //rdx -> totalBytes
    getBytesFromAddress(registers->rdi, (uint64_t*)registers->rsi, (uint8_t)registers->rdx);
    break;

    case 11:
    //rdi -> puntero a int para devolver si hay algo
    bufferEmpty((uint64_t *) registers->rdi);
    break;

    case 12: //readError
    //rdi -> puntero a int para que devuelva el Error
    readError((uint64_t*)registers->rdi);
    break;

    case 13:
    //rdi -> indice de la tecla de funcion (de 1 (F1) a 10 (F10))
    //rsi -> puntero a la funcion tipo void foo()
    setFunctionKeyMethod(registers->rdi, (void (*)())registers->rsi);
    break;

    case 14:
    //rdi -> tamaño de memoria pedido
    mallocMemory(registers->rdi);
    break;

    case 15:
    //rdi -> puntero al bloque a liberar
    freeMemory((void *) registers->rdi);
    break;
    
    case 16:
    getPid();
    break;

    case 17:
    //rdi -> buffer
    printProcessList(registers->rdi);
    break;

    case 18:
    // rdi -> pid
    // rsi -> new priority
    changeProcessPriority(registers->rdi, registers->rsi);
    break;

    case 19:
    // rdi -> pid
    blockProcess(registers->rdi);
    break;

    case 20:
    // rdi -> entry point
    // rsi -> number of arguments
    // rdx -> arguments
    // rcx -> foreground
    createProcess(registers->rdi, registers->rsi, registers->rdx, registers->rcx);
    break;

    case 21:
    // rdi -> pid
    killProcess(registers->rdi);
    break;

    case 22:
    resignCPU();
    break;
  
    case 23:
    exitProcess();
    break;

    case 24:
    // rdi -> id
    pipeOpen(registers->rdi);
    break;
    
    case 25:
    // rdi -> id
    pipeClose(registers->rdi);
    break;

    case 26:
    // rdi -> id
    pipeRead(registers->rdi);
    break;

    case 27:
    // rdi -> id
    // rsi -> string
    pipeWrite(registers->rdi,registers->rsi);
    break;

    case 28:
    // rdi -> id
    createPipe(registers->rdi);
    break;

    case 29:
    // rdi -> buffer
    printPipes(registers->rdi);
    break;

    case 30:
    // rdi -> id
    // rsi -> initValue
    openSemaphore(registers->rdi,registers->rsi);
    break;

    case 31:
    // rdi -> id
    waitSemphore(registers->rdi);
    break;

    case 32:
    // rdi -> id
    postSemaphore(registers->rdi);
    break;

    case 33:
    // rdi -> id
    closeSemaphore(registers->rdi);
    break;

    case 34:
    // rdi -> buffer
    printSemaphore(registers->rdi);
    break;

  }
}

void getDateInfo(uint8_t mode, uint8_t * target) {
  switch(mode) {
    case 0: *target = getSeconds();
    break;
    case 1: *target = getMinutes();
    break;
    case 2: *target = getHours();
    break;
    case 3: *target = getDay();
    break;
    case 4: *target = getMonth();
    break;
    case 5: *target = getYear();
    break;
  }
}

void writeStr(registerStruct * registers) {
  uint64_t xOffset = 0;
  char * buffer = (char *)registers->rdi;
  for (uint64_t i = 0; i < registers->rsi && buffer[i] != 0; i++) {
    char ch = ((char *)registers->rdi)[i];
    drawChar(registers->rdx + xOffset, registers->rcx, ch, registers->r10, registers->r8, registers->r9, registers->r11);
    xOffset += CHAR_WIDTH * registers->r10;
  }
  //drawChar(0, 0, 'A',1, 0xFFFFFF, 0, 0);
}

#endif
