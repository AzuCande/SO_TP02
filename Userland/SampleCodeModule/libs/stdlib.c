#ifndef STD_LIB_C
#define STD_LIB_C value
#include <stdlib.h>

int atoi(char * str);
int intToString(unsigned long long num, char * buffer);
int strlen(char *str);
int strcmp(char * str1, char * str2);
int intToBase(unsigned long long num, int base, char*buffer);
int iabs(int num);
int isAlfaNum(char c);

int atoi(char * str){
  int aux = 0;
  while (*str!=0){
    aux = aux*10 + (*str)+'0';
    str++;
  }
  return aux;
}

int atohex(char * str) {
  int aux = 0;
  while (*str!= 0) {
    int value = 0;
    if (*str >= '0' && *str <= '9') {
      value = *str - '0';
    } else if (*str >= 'A' && *str <= 'F') {
      value = 10 + *str - 'A';
    } else if (*str >= 'a' && *str <= 'f') {
      value = 10 + *str - 'a';
    } else {
      return -1;
    }
    str++;
    aux *= 16;
    aux += value;
  }
  return aux;
}
char toLower(char letter){
  if (letter<='Z' && letter>='A'){
    return letter+'a'-'A';
  }
  return letter;
}

char toUpper(char letter){
  if (letter<='z' && letter>='a'){
    return letter+'A'-'a';
  }
  return letter;
}

int intToBase(unsigned long long num, int base, char*buffer){
    if (num == 0) {
      buffer[0] = '0';
      buffer[1] = 0;
      return 2;
    }
    char stack[11];
    int c = 0;
    int i=0;
    int remainder = 0;
    if(num==0) stack[i++]='0';
    while(num!=0){
        remainder = num % base;
        stack[i]=remainder>=10? remainder+'A'-10:remainder+'0';
        num = num/base;
        i++;
    }
    c=i;
    i--;
    while(i>=0){
        *buffer=stack[i];
        buffer++;
        i--;
    }
    *buffer=0;
    return c;
}

int iabs(int num){
  if (num < 0){
   return -num;
  }
  return num;
}


int intToString(unsigned long long num, char * buffer){
  return intToBase(num,10,buffer);
}

int strlen(char *str){
  int i =0;
  for ( i = 0; *(str+i); i++) {
    ;
  }
  return i;
}

int strcmp(char * str1, char * str2){
    int i =0;
    while(str1[i] != 0 && str2[i] != 0 ){
        if(str1[i] != str2[i]){
            return str1[i]-str2[i];
        }
        i++;
    }
    return str1[i]-str2[i];
}

char * strcpy(char * destination, const char * source) {
  // return if no memory is allocated to the destination
  // if (destination == NULL) {
  //   return NULL;
  // }

  // take a pointer pointing to the beginning of the destination string
  char *ptr = destination;

  // copy the C-string pointed by source into the array
  // pointed by destination
  while (*source != '\0')
  {
    *destination = *source;
    destination++;
    source++;
  }

  // include the terminating null character
  *destination = '\0';

  // the destination is returned by standard `strcpy()`
  return ptr;
}

int isVowel(char letter) {
    letter = toLower(letter);

    switch(letter) {
      case 'a':
      case 'e':
      case 'i':
      case 'o':
      case 'u':
      return 1;
      break;
      
      default:
      return 0;
    }
}

int isAlfaNum(char c) {
  if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9')) {
    return 1;
  }

  return 0;
}

#endif
