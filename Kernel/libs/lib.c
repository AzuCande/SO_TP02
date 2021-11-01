#include <lib.h>

void swap(char *a, char * b); // TODO: delete if not used
void reverse(char str[], int length); // TODO: delete if not used

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

void reveseString(char * string, int length) {
	for(int i=0; i<length/2; i++) {
		char aux = string[i];
		string[i] = string[length-i];
		string[length-i] = aux;
	}
}

void intToString(char * dest, int number) {
	int i = 0;
	int isNegative = 0;

    if(number == 0) {
        dest[i++] = '0';
        dest[i] = '\0';
        return i;
    }
    
    if(number<0) {
        isNegative = 1;
        number = number * (-1);
    }

    for(i = 0; number > 0; i++) {
        int aux = number % 10;
        dest[i] = aux + '0';
        number /= 10;
    }

    if(isNegative) {
        dest[i++] = '-';
    }

    dest[i] = '\0';

    reveseString(dest,i);
}

void intToBaseString(int base, char * dest, uint64_t number) {

    if(number == 0) {
        dest[0] = '0';
        dest[1] = '\0';
        return;
    }

	int i;
	for(i=0; number>0; i++) {
		int aux = number%base;
		int add = aux<10? '0': 'A'-10;
		dest[i] = aux + add;

		number = number/base;
	}

	dest[i] = '\0';

	reveseString(dest,i);
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

void swap(char *a, char * b) {
	if(!a || !b)
		return;

	char temp = *(a);
	*(a) = *(b);
	*(b) = temp;
}

void reverse(char str[], int length) {
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap((str+start), (str+end));
        start++;
        end--;
    }
}

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

char* strCopy(char* str){
  int strSize = strlen(str);
  char *copy = (char*) mallocMemory(sizeof(char*)*strSize);
  if (copy == NULL) return 0;
  strcpy(copy, str);
  return copy;
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
