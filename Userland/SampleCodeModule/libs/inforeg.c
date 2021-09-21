#include <inforeg.h>
#include <stdint.h>
#include <syscalls_asm.h>

void getRegisters(uint64_t * registers){
    getRegistersSyscall(registers);
}
