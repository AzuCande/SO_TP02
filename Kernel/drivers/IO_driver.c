#ifndef IO_DRIVER
#define IO_DRIVER
#include <IO_driver.h>

static registerStruct registerSnapshot;
static uint64_t errNum = 32;

void saveRegisters(registerStruct * registers) {
  registerSnapshot.r15 = registers->r15;
  registerSnapshot.r14 = registers->r14;
  registerSnapshot.r13 = registers->r13;
  registerSnapshot.r12 = registers->r12;
  registerSnapshot.r11 = registers->r11;
  registerSnapshot.r10 = registers->r10;
  registerSnapshot.r9 = registers->r9;
  registerSnapshot.r8 = registers->r8;
  registerSnapshot.rsi = registers->rsi;
  registerSnapshot.rdi = registers->rdi;
  registerSnapshot.rbp = registers->rbp;
  registerSnapshot.rdx = registers->rdx;
  registerSnapshot.rcx = registers->rcx;
  registerSnapshot.rbx = registers->rbx;
  registerSnapshot.rax = registers->rax;
  registerSnapshot.rip = registers->rip;
  registerSnapshot.cs = registers->cs;
  registerSnapshot.flags = registers->flags;
  registerSnapshot.rsp = registers->rsp;
  registerSnapshot.ss = registers->ss;
}

void getRegisters(uint64_t * arr) {
  uint64_t * regArr = (uint64_t *)&registerSnapshot;
  for (int i = 0; i < TOTAL_REGISTERS; i++) {
    arr[i] = regArr[TOTAL_REGISTERS - 1 - i];
  }
}

void getBytesFromAddress(uint64_t address, uint64_t * target, uint8_t totalBytes) {
  uint8_t * pos = (uint8_t *) address;
  for (uint8_t i = 0; i < totalBytes; i++) {
    target[i] = (uint64_t)pos[i];
  }
}

void saveErrCode(uint64_t err) {
  errNum = err;
}

uint64_t getErrCode() {
  return errNum;
}

int readFrom(char * buff, uint64_t size, uint64_t * count) {
  int readFd = currentReadFd();
  if(readFd == IN) {
    if(isCurrentFg()) {
      readKeyboard(buff, size, count);  // TODO: chequear count
      return 1;
    } else {
      return -1;
    }
  }
  int toReturn;
  pipeRead(readFd, &toReturn);
  return toReturn;
}

void writeTo(registerStruct *registers) {
  int writeFd = currentWriteFd();
  if(writeFd == OUT) {
    writeStr(registers);
  } else {
    int toReturn;
    pipeWrite(writeFd, (char *) registers->rdi, &toReturn);
  }
}

#endif
