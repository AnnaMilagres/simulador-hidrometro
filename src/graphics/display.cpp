#include "display.h"
#include "drawing.h"

void drawSevenSegDigit(Image &img, int ox, int oy, int seg_w, int seg_h, int digit, u8 r, u8 g, u8 b) {
    auto rect = [&](int x,int y,int w,int h){
        for (int yy=y;yy<y+h;yy++) for (int xx=x;xx<x+w;xx++) img.setPixel(xx,yy,r,g,b);
    };
    const bool segmap[10][7] = {
        {1,1,1,1,1,1,0}, //0
        {0,1,1,0,0,0,0}, //1
        {1,1,0,1,1,0,1}, //2
        {1,1,1,1,0,0,1}, //3
        {0,1,1,0,0,1,1}, //4
        {1,0,1,1,0,1,1}, //5
        {1,0,1,1,1,1,1}, //6
        {1,1,1,0,0,0,0}, //7
        {1,1,1,1,1,1,1}, //8
        {1,1,1,1,0,1,1}  //9
    };
    bool seg[7] = {0};
    if (digit<0||digit>9) digit=0;
    for (int i=0;i<7;i++) seg[i]=segmap[digit][i];
    if (seg[0]) rect(ox+seg_h, oy, seg_w, seg_h);
    if (seg[1]) rect(ox+seg_h+seg_w, oy+seg_h, seg_h, seg_w);
    if (seg[2]) rect(ox+seg_h+seg_w, oy+seg_h+seg_w+seg_h, seg_h, seg_w);
    if (seg[3]) rect(ox+seg_h, oy+2*seg_h+2*seg_w, seg_w, seg_h);
    if (seg[4]) rect(ox, oy+seg_h+seg_w+seg_h, seg_h, seg_w);
    if (seg[5]) rect(ox, oy+seg_h, seg_h, seg_w);
    if (seg[6]) rect(ox+seg_h, oy+seg_h+seg_w, seg_w, seg_h);
}

void drawOdometer(Image &img, int ox, int oy, int digits, int seg_w, int seg_h, int spacing, int totalValue) {
    for (int y=oy-5;y<oy+seg_h*3 + seg_w*2 + 5; y++) for (int x=ox-5;x<ox + digits*(seg_w+seg_h+spacing)+10;x++)
        img.setPixel(x,y,230,230,230);
    int place = 1;
    for (int i=0;i<digits-1;i++) place *= 10;
    int val = totalValue;
    for (int d=0; d<digits; d++) {
        int digit = (val / place) % 10;
        int px = ox + d * (seg_w + seg_h + spacing);
        drawSevenSegDigit(img, px, oy, seg_w, seg_h, digit, 10,10,10);
        if (place>1) place /= 10;
    }
}