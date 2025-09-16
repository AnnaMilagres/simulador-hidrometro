#ifndef DRAWING_H
#define DRAWING_H

#include "image.h"

// Funções básicas de desenho
void drawLine(Image &img, double x0, double y0, double x1, double y1, u8 r, u8 g, u8 b);
void drawCircle(Image &img, int cx, int cy, int radius, u8 r, u8 g, u8 b);
void drawPointer(Image &img, int cx, int cy, double angleRad, double length, u8 r, u8 g, u8 b);
void fillRect(Image &img, int x, int y, int w, int h, u8 r, u8 g, u8 b);

#endif // DRAWING_H