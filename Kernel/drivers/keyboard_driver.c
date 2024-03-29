#ifndef KEYBOARD_DRIVER
#define KEYBOARD_DRIVER
#include <keyboard_asm.h>
#include <keyboard_driver.h>
#include <registers.h>
#include <stdint.h>
#include <IO_driver.h>
#include <interrupts.h>
#define BUFFER_SIZE 64

static uint8_t shiftL = 0, shiftR = 0, ctrlL = 0;
static char pressCodes[][3] = {{0, 0, 0}, {0, 0, 0}, {'1', '!', 0}, {'2', '@', 0}, {'3', '#', 0}, {'4', '$', 0}, {'5', '%', 0}, {'6', '^', 0}, {'7', '&', 0},
{'8', '*', 0}, {'9', '(', 0}, {'0', ')', 0}, {'-', '_', 0}, {'=', '+', 0}, {'\b', '\b', 0}, {'\t', '\t', 0}, {'q', 'Q', 0}, {'w', 'W', 0}, {'e', 'E', 0}, {'r', 'R', 0},
{'t', 'T', 0}, {'y', 'Y', 0}, {'u', 'U', 0}, {'i', 'I', 0}, {'o', 'O', 0}, {'p', 'P', 0}, {'[', '{', 0}, {']', '}', 0}, {'\n', '\n', 0}, {0, 0, 0}, {'a', 'A', 0},
{'s', 'S', 0}, {'d', 'D', 0}, {'f', 'F', 0}, {'g', 'G', 0}, {'h', 'H', 0}, {'j', 'J', 0}, {'k', 'K', 0}, {'l', 'L', 0}, {';', ':', 0}, {'\'', '\"', 0}, {'`', '~', 0},
{0, 0, 0}, {'\\', '|', 0}, {'z', 'Z', 0}, {'x', 'X', 0}, {'c', 'C', -1}, {'v', 'V', 0}, {'b', 'B', 0}, {'n', 'N', 0}, {'m', 'M', 0}, {',', '<', 0}, {'.', '>', 0},
{'/', '?', 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {' ', ' ', 0}, {0, 0, 0}};

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
  } else if (keyCode == CTRL_L) {
    ctrlL = 2;
  } else if (keyCode == (SHIFT_L + KEY_RELEASED)) {
    shiftL = 0;
  } else if (keyCode == (SHIFT_R + KEY_RELEASED)) {
    shiftR = 0;
  } else if (keyCode == (CTRL_L + KEY_RELEASED)) {
    ctrlL = 0;
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
    if (mayus == 0 && ctrlL != 0) {
      mayus = 2;
    }
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
