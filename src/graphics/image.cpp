#include "image.h"

Image::Image(int W, int H, u8 r, u8 g, u8 b) : w(W), h(H), data(W*H*3) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int i = (y*w + x) * 3;
            data[i+0] = r; 
            data[i+1] = g; 
            data[i+2] = b;
        }
    }
}

void Image::setPixel(int x, int y, u8 r, u8 g, u8 b) {
    if (x < 0 || x >= w || y < 0 || y >= h) return;
    int i = (y*w + x) * 3;
    data[i+0] = r; 
    data[i+1] = g; 
    data[i+2] = b;
}