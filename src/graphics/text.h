#ifndef TEXT_H
#define TEXT_H

#include "image.h"
#include <string>

// Renderização de texto
void drawChar(Image &img, int x, int y, char ch, u8 r = 0, u8 g = 0, u8 b = 0);
void drawText(Image &img, int x, int y, const std::string &text, u8 r = 0, u8 g = 0, u8 b = 0);

// Helper interno
int charIndex(char c);

#endif // TEXT_H