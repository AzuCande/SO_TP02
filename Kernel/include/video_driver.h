#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H
#include <stdint.h>

void initVideoDriver();
void clearDisplay(uint64_t color);
void drawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t col);
void drawLine(uint64_t xStart, uint64_t yStart, uint64_t xEnd, uint64_t yEnd, uint64_t col);
void drawPixel(uint64_t x, uint64_t y, uint64_t col);
void drawChar(uint64_t x, uint64_t y, uint8_t character, uint64_t fontSize, uint64_t fontColor, uint64_t backgroundColor, uint8_t alphaBackground);
uint64_t getScreenWidthRes();
uint64_t getScreenHeightRes();
void drawMatrix(uint64_t x, uint64_t y, uint64_t *mat, uint64_t width, uint64_t height, uint64_t size);
#endif
