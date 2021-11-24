#include <syscalls.h>

void getDateInfo(uint8_t mode, uint8_t * target);

int syscallHandler(registerStruct * registers) {
  uint64_t option = registers->rax;

  switch(option) {
    //READ KEYBOARD
    case 0:
    //rdi -> puntero a buffer
    //rsi -> uint8_t size
    //rdx -> puntero a uint64_t count
    //rcx -> valor de retorno
    return readFrom((char *)registers->rdi, (uint8_t) registers->rsi, (uint64_t*)registers->rdx);
    // readKeyboard((char *)registers->rdi, (uint8_t) registers->rsi, (uint64_t*)registers->rdx);
    // break;

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
    return writeTo(registers);
    // writeStr(registers);
    // break;

    case 2:
    //rdi color
      clearDisplay((uint64_t) registers->rdi);
      return 1;
      // break;

    case 3:
    // rdi xstart , rsi ystart, rdx xend, rcx yend, r8 color
      drawLine((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t) registers->rdx,(uint64_t) registers->rcx,(uint64_t) registers->r8);
      return 1;
      // break;

    case 4:
    // r9 xi, r8 yi, rsi color
      drawPixel((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t) registers->rdx);
      return 1;
      // break;

    case 5:
    // rdi xi, rsi yi , rdx width, rc8 height , r8 color
      drawRectangle((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t) registers->rdx,(uint64_t) registers->rcx,(uint64_t) registers->r8);
      return 1;
      // break;

    case 6:
    // rdi xi, rsi yi, rdx puntero a matriz, rcx width, r8 height , r9 size
      drawMatrix((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t *) registers->rdx,(uint64_t) registers->rcx,(uint64_t) registers->r8,(uint64_t) registers->r9);
      return 1;
      // break;

    case 7:
      * ((uint64_t *)registers->rdi) = getTicks();
      return 1;
      // break;

    case 8:
    //rdi -> mode
    //rsi -> puntero a entero
    getDateInfo((uint8_t) registers->rdi, (uint8_t *) registers->rsi);
    return 1;
    // break;

    case 9: //Obtener los registros
    //rdi -> puntero a vector de uint64_t para guardar los valores
    getRegisters((uint64_t*)registers->rdi);
    return 1;
    // break;

    case 10:
    //rdi -> direccion de la que se desea leer
    //rsi -> direccion del buffer para guardar
    //rdx -> totalBytes
    getBytesFromAddress(registers->rdi, (uint64_t*)registers->rsi, (uint8_t)registers->rdx);
    return 1;
    // break;

    case 11:
    //rdi -> puntero a int para devolver si hay algo
    bufferEmpty((uint64_t *) registers->rdi);
    return 1;
    // break;

    case 12: //readError
    //rdi -> puntero a int para que devuelva el Error
    readError((uint64_t*)registers->rdi);
    return 1;
    // break;

    case 13:
    //rdi -> indice de la tecla de funcion (de 1 (F1) a 10 (F10))
    //rsi -> puntero a la funcion tipo void foo()
    setFunctionKeyMethod(registers->rdi, (void (*)())registers->rsi);
    return 1;
    // break;

    case 14:
    // rdi -> tamaño de memoria pedido
    // rsi -> result
    mallocWrapper((uint64_t) registers->rdi, (void**) registers->rsi);
    return 1;
    // break;

    case 15:
    //rdi -> puntero al bloque a liberar
    freeMemory((void *) registers->rdi);
    return 1;
    // break;
    
    case 16:
    getPid((uint64_t *) registers->rdi);
    return 1;
    // break;

    case 17:
    //rdi -> buffer
    ps((char *)registers->rdi);
    return 1;
    // break;

    case 18:
    // rdi -> pid
    // rsi -> new priority
    // rdx -> result
    nice((uint64_t) registers->rdi, (uint64_t) registers->rsi,(int *) registers->rdx);
    return 1;
    // break;

    case 19:
    // rdi -> pid
    // rsi -> result
    blockProcess((uint64_t) registers->rdi,(int *) registers->rsi);
    return 1;
    // break;

    case 20:
    // rdi -> entry point
    // rsi -> number of arguments
    // rdx -> arguments
    // rcx -> foreground
    // r8 -> fds
    createProcess((void (*)()) registers->rdi, (int) registers->rsi, (char **) registers->rdx,(uint64_t *) registers->rcx, (int *) registers->r8);
    return 1;
    // break;
    //

    case 21:
    // rdi -> pid
    // rsi -> result
    endProcess((uint64_t) registers->rdi, (int *) registers->rsi);
    return 1;
    // break;

    case 22:
    yield();
    return 1;
    // break;
  
    case 23:
    // rdi -> pid
    // rsi -> result
    unlockProcess((uint64_t) registers->rdi,(int *) registers->rsi);
    return 1;
    // break;

    case 24:
    // rdi -> id
    // rsi -> toReturn
    pipeOpen((uint32_t) registers->rdi, (int *) registers->rsi);
    return 1;
    // break;
    
    case 25:
    // rdi -> id
    // rsi -> toReturn
    pipeClose((uint32_t) registers->rdi, (int *) registers->rsi);
    return 1;
    // break;

    case 26:
    // rdi -> id
    // rsi -> toReturn
    pipeRead((uint32_t) registers->rdi, (char *) registers->rsi, (int *) registers->rdx);
    return 1;
    // break;

    case 27:
    // rdi -> id
    // rsi -> string
    // rdx -> toReturn
    pipeWrite((uint32_t) registers->rdi,(char *) registers->rsi, (int *) registers->rdx);
    return 1;
    // break;

    case 28:
    // rdi -> buffer
    // rsi -> size
    printMem((char *)registers->rdi, (int)registers->rsi);
    return 1;
    // break;

    case 29:
    // rdi -> buffer
    printPipes((char *) registers->rdi);
    return 1;
    // break;

    case 30:
    // rdi -> id
    // rsi -> initValue
    // rdx -> *toReturn
    openSemaphore((uint32_t) registers->rdi, (uint32_t) registers->rsi, (int*) registers->rdx);
    return 1;
    // break;

    case 31:
    // rdi -> id
    // rsi -> *toReturn
    waitSemaphore((uint32_t) registers->rdi, (int*) registers->rsi);
    return 1;
    // break;

    case 32:
    // rdi -> id
    // rsi -> *toReturn
    postSemaphore((uint32_t) registers->rdi, (int*) registers->rsi);
    return 1;
    // break;

    case 33:
    // rdi -> id
    // rsi -> *toReturn
    closeSemaphore((uint32_t) registers->rdi, (int*) registers->rsi);
    return 1;
    // break;

    case 34:
    // rdi -> buffer
    printSemaphore((char *) registers->rdi);
    return 1;
    // break;

    case 35:
    //rdi -> trae el size a pedir  
    //rsi -> puntero a la memoria 
    sbrSyscall((uint64_t) registers->rdi, (void**) registers->rsi);
    return 1;
    // break;

  }
  return -1;
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
