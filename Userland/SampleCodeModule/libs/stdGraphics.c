#ifndef STD_GRAPHICS
#define STD_GRAPHICS
#include <syscalls_asm.h>
#include <stdint.h>


void drawString(int x, int y, char * buffer, int dim, uint64_t fontColor, uint64_t backgroundColor, int fontSize, int alphaBackground) {
  drawStringSysCall(buffer, (uint64_t)dim, (uint64_t)x, (uint64_t)y, fontColor, backgroundColor, (uint64_t)fontSize, (uint64_t)alphaBackground);
}

void clearScreen(uint64_t color) {
  clearDisplaySyscall(color);
}

void drawRect(int xi, int yi, int width, int height, uint64_t color){
  drawRectSyscall((uint64_t) xi, (uint64_t) yi, (uint64_t) width, (uint64_t) height, color);
}
#endif
