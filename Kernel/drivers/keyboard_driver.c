#ifndef KEYBOARD_DRIVER
#define KEYBOARD_DRIVER
#include <keyboard_asm.h>
#include <keyboard_driver.h>
#include <regi.h>
#include <stdint.h>
#include <IO_driver.h>
#define BUFFER_SIZE 64

static uint8_t shiftL = 0, shiftR = 0;
static char pressCodes[][2] = {{0, 0}, {0, 0}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'},
{'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {'\b', '\b'}, {'\t', '\t'}, {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'},
{'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'}, {'\n', '\n'}, {0, 0}, {'a', 'A'},
{'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\"'}, {'`', '~'},
{0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'},
{'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {' ', ' '}, {0, 0}};

static char BUFFER[BUFFER_SIZE] = {0}; //Buffer circular
static uint64_t startIndex = 0; //Indice del comienzo para el read
static uint64_t endIndex = 0; //Indice del final para el read
static void (*functionKeysMethods[10])(void);
static uint8_t functionKeysMethodsInitialized[10];

void keyboardHandler(registerStruct * registers) {
  uint8_t keyCode = getKeyCode();

  if (keyCode == SHIFT_L) {
    shiftL = 1;
  } else if (keyCode == SHIFT_R) {
    shiftR = 1;
  } else if (keyCode == (SHIFT_L + KEY_RELEASED)) {
    shiftL = 0;
  } else if (keyCode == (SHIFT_R + KEY_RELEASED)) {
    shiftR = 0;
  } else if (keyCode == F12) {
    saveRegisters(registers);
  } else if ( keyCode >= 59 && keyCode < 69) {
    int index = keyCode - F1;
    if (functionKeysMethodsInitialized[index]) {
      (*functionKeysMethods[index])();
    }
  }

  if (keyCode < KEYS) {
    uint8_t mayus = (shiftL || shiftR);
    BUFFER[(endIndex++)%BUFFER_SIZE] = pressCodes[keyCode][mayus];
  }
}

void readKeyboard(char * buff, uint64_t size, uint64_t * count) {
  int i = 0;
  while (endIndex<=startIndex) {
    _hlt();
  }
	for(i = 0; i<(endIndex-startIndex) && i<size; i++){
		buff[i] = BUFFER[(startIndex++)%BUFFER_SIZE];
	}
	*count = i;
}

void bufferEmpty(uint64_t * target) {
  *target = (endIndex - startIndex);
}
void isMayus(uint64_t * target) {
  *target = (shiftL || shiftR);
}

void initializeFunctionKeys() {
  for (uint8_t i = 0; i < 10; i++) {
    functionKeysMethods[i] = 0;
    functionKeysMethodsInitialized[i] = 0;
  }
}

void setFunctionKeyMethod(uint64_t index, void (*function)()) {
  if (index > 0 && index <= 10) {
    functionKeysMethodsInitialized[index-1] = 1;
    functionKeysMethods[index-1] = function;
  }
}

#endif
