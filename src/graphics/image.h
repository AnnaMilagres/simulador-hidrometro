#ifndef IMAGE_H
#define IMAGE_H

#include <vector>

using u8 = unsigned char;

// Estrutura simples para imagem RGB
struct Image {
    int w, h;
    std::vector<u8> data; // RGB

    Image(int W, int H, u8 r = 255, u8 g = 255, u8 b = 255);
    void setPixel(int x, int y, u8 r, u8 g, u8 b);
};

#endif // IMAGE_H