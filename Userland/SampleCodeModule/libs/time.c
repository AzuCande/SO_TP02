#include <syscalls_asm.h>
#include <stdint.h>
#include <time.h>

int getSeconds() {
    uint8_t target;
    getTimeSyscall(0, &target);
    return target;
}

int getMinutes() {
    uint8_t target;
    getTimeSyscall(1, &target);
    return target;
}

int getHours() {
    uint8_t target;
    getTimeSyscall(2, &target);
    return target;
}

int getDays() {
    uint8_t target;
    getTimeSyscall(3, &target);
    return target;
}

int getMonth() {
    uint8_t target;
    getTimeSyscall(4, &target);
    return target;
}

int getYear() {
    uint8_t target;
    getTimeSyscall(5, &target);
    return target;
}

int getTicks() {
    uint64_t ticks;
    // getTicksSyscall(&ticks);
    getTicksSyscall(&ticks);
    return ticks;
}
