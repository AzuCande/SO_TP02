#ifndef IRQ_DISPATCHER
#define IRQ_DISPATCHER

#include <naiveConsole.h>
#include <keyboard_driver.h>
#include <timer_driver.h>
#include <regi.h>
#include <syscalls.h>

static void int_21h(registerStruct *);
static void int_80h(registerStruct *);
static void int_20h();

void irqDispatcher(uint64_t irq, uint64_t * registers) {
  switch (irq) {
    case 0: int_20h(); _sendEOI();;
    break;
    case 1: int_21h((registerStruct *)registers); _sendEOI();
    break;
    case 80: _sti(); _sendEOI(); int_80h((registerStruct *)registers);
    break;
  }
}

static void int_20h() {
  timeHandler();
}

static void int_21h(registerStruct * registers) {
  keyboardHandler(registers);
};

static void int_80h(registerStruct * registers) {
  syscallHandler(registers);
}


#endif
