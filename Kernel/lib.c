#include <stdint.h>

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

void strcpy(char * destination, const char * source) {
	int i;
	for(i=0; source[i]!=0; i++) {
		destination[i]=source[i];
	}
	destination[i]=0;
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
    if(number == 0) {
        dest[0] = '0';
        dest[1] = '\0';
        return;
    }

    int negative = 0;
    
    if(number<0) {
        negative = 1;
        number = number * (-1);
    }

    int i;
    for(i=0; number > 0; i++) {
        int aux = number%10;
        dest[i] = aux + '0';
        number = number/10;
    }

    if(negative) {
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