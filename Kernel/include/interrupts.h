#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

void _cli();
void _sti();
void _hlt();
void picSlaveMask(uint8_t mask);
void picMasterMask(uint8_t mask);
void _irq00Handler(void);
void _irq01Handler(void);
void _irq80Handler(void);
void _exception00Handler(void);
void _exception06Handler(void);
void saveInitialConditions(void* address);
void _sendEOI(void);

#endif
