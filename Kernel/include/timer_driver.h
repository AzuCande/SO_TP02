#ifndef TIME_DRIVER_H
#define TIME_DRIVER_H

#include <stdint.h>

void timeHandler();
uint64_t getTicks();
int secondsElapsed();

#endif
