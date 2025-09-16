#include "drawing.h"
#include <cmath>
#include <algorithm>

void drawLine(Image &img, double x0, double y0, double x1, double y1, u8 r, u8 g, u8 b) {
    double dx = x1-x0, dy = y1-y0;
    double steps = std::max(std::abs(dx), std::abs(dy));
    if (steps < 1) {
        img.setPixel((int)std::round(x0),(int)std::round(y0),r,g,b);
        return;
    }
    double sx = dx/steps, sy = dy/steps;
    double x = x0, y = y0;
    for (int i=0;i<= (int)steps;i++) {
        img.setPixel((int)std::round(x),(int)std::round(y),r,g,b);
        x += sx; y += sy;
    }
}

void drawCircle(Image &img, int cx, int cy, int radius, u8 r, u8 g, u8 b) {
    int steps = std::max(12, radius*6);
    for (int i=0;i<steps;i++) {
        double a1 = (2*M_PI*i)/steps;
        double a2 = (2*M_PI*(i+1))/steps;
        double x1 = cx + cos(a1)*radius;
        double y1 = cy + sin(a1)*radius;
        double x2 = cx + cos(a2)*radius;
        double y2 = cy + sin(a2)*radius;
        drawLine(img,x1,y1,x2,y2,r,g,b);
    }
}

void drawPointer(Image &img, int cx, int cy, double angleRad, double length, u8 r, u8 g, u8 b) {
    double x1 = cx + cos(angleRad) * length;
    double y1 = cy + sin(angleRad) * length;
    drawLine(img, cx, cy, x1, y1, r,g,b);
}

void fillRect(Image &img, int x, int y, int w, int h, u8 r, u8 g, u8 b) {
    for (int yy = y; yy < y + h; yy++) {
        for (int xx = x; xx < x + w; xx++) {
            img.setPixel(xx, yy, r, g, b);
        }
    }
}