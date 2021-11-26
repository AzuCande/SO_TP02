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
  int isNegative = 1;
  if (str[0] == '-') {
    isNegative = -1;
    str++;
  }
  while (*str!=0){
    aux = aux*10 + (*str)-'0';
    str++;
  }
  return aux * isNegative;
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
  int i;
  for ( i = 0; source[i] != 0; i++) {
    destination[i] = source[i];
  }
  destination[i] = 0;
  return destination;
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

void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Function to reverse `buffer[i…j]`
char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}

// Iterative function to implement `itoa()` function in C
char* itoa(uint64_t value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // consider the absolute value of the number
    int n = (value >= 0)? value : value * (-1);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

void strcat(char * destination, char * source, unsigned int *index) {
	for(int i = 0; source[i]; (*index)++, i++) {
		destination[*index] = source[i];
	}
}

#endif
