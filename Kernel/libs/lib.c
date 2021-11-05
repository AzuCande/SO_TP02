#include <lib.h>

char* reverse(char *buffer, int i, int j);
void swap(char *x, char *y);

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

char* strcpy(char * destination, const char * source) {
	int i;
	for(i=0; source[i]!=0; i++) {
		destination[i]=source[i];
	}
	destination[i]=0;
	return destination;
}

void strcat(char * destination, char * source, unsigned int *index) {
	for(int i = 0; source[i]; (*index)++, i++) {
		destination[*index] = source[i];
	}
}

// Function to swap two numbers
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

int strlen(char *str){
  int i =0;
  for ( i = 0; *(str+i); i++)
  {

  }
  return i;
}

/* ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
** ------------------------------------------------------------------------------------------------------------------
*/

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

int iabs(int num){
  if (num < 0){
   return -num;
  }
  return num;
}

void addSpace(char *dest, int* from, int strSize, int numSpace){
  int j;
  for (j=0; j < numSpace && *from < strSize; j++, (*from)++){
    dest[*from] = ' ';
  }
  return;
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

char* strCopy(char* str){
  int strSize = strlen(str);
  char *copy = (char*) mallocMemory(sizeof(char*)*strSize);
  if (copy == NULL) return 0;
  strcpy(copy, str);
  return copy;
}
