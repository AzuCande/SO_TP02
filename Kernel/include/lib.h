#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void strcpy(char * destination, const char * source);
void strcat(char * destination, char * source, int *index);

char *cpuVendor(char *result);

uint8_t RTC(uint8_t mode);

#endif
