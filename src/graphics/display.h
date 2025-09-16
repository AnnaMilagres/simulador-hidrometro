#ifndef DISPLAY_H
#define DISPLAY_H

#include "image.h"

// Displays digitais e odômetros
void drawSevenSegDigit(Image &img, int ox, int oy, int seg_w, int seg_h, int digit, u8 r, u8 g, u8 b);
void drawOdometer(Image &img, int ox, int oy, int digits, int seg_w, int seg_h, int spacing, int totalValue);

#endif // DISPLAY_H