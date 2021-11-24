#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

#define EOF -1

int atoi(char * str);
int intToString(unsigned long long num, char * buffer);
int strlen(char *str);
int strcmp(char * str1, char * str2);
int intToBase(unsigned long long num, int base, char*buffer);
int iabs(int num);
char * strcpy(char * dest, const char * src);
int atohex(char * str);
char toUpper(char letter);
char toLower(char letter);
int isVowel(char letter);
int isAlfaNum(char c);
char* itoa(uint64_t value, char* buffer, int base);

#endif
