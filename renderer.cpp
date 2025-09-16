// renderer.cpp - implementação do renderer (inclui stb_image_write)
#include "renderer.h"
#include <vector>
#include <cmath>
#include <string>
#include <cstring>

// stb imagem writer (definição apenas aqui)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using u8 = unsigned char;

struct Image {
    int w,h;
    std::vector<u8> data;
    Image(int W,int H, u8 r=255,u8 g=255,u8 b=255): w(W), h(H), data(W*H*3) {
        for (int y=0;y<h;y++) for (int x=0;x<w;x++) {
            int i=(y*w+x)*3;
            data[i+0]=r; data[i+1]=g; data[i+2]=b;
        }
    }
    void setPixel(int x,int y,u8 r,u8 g,u8 b) {
        if (x<0||x>=w||y<0||y>=h) return;
        int i=(y*w+x)*3;
        data[i+0]=r; data[i+1]=g; data[i+2]=b;
    }
};

// desenho básico (linhas, circulos, ponteiros, sete-seg) - adaptado do código anterior
void drawLine(Image &img, double x0,double y0,double x1,double y1, u8 r,u8 g,u8 b) {
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
void drawCircle(Image &img, int cx,int cy,int radius, u8 r,u8 g,u8 b) {
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
void drawPointer(Image &img, int cx,int cy, double angleRad, double length, u8 r,u8 g,u8 b) {
    double x1 = cx + cos(angleRad) * length;
    double y1 = cy + sin(angleRad) * length;
    drawLine(img, cx, cy, x1, y1, r,g,b);
}
void drawSevenSegDigit(Image &img, int ox,int oy, int seg_w,int seg_h, int digit, u8 r,u8 g,u8 b) {
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
void drawOdometer(Image &img, int ox,int oy, int digits, int seg_w, int seg_h, int spacing, int totalLiters) {
    for (int y=oy-5;y<oy+seg_h*3 + seg_w*2 + 5; y++) for (int x=ox-5;x<ox + digits*(seg_w+seg_h+spacing)+10;x++)
        img.setPixel(x,y,230,230,230);
    int place = 1;
    for (int i=0;i<digits-1;i++) place *= 10;
    int val = totalLiters;
    for (int d=0; d<digits; d++) {
        int digit = (val / place) % 10;
        int px = ox + d * (seg_w + seg_h + spacing);
        drawSevenSegDigit(img, px, oy, seg_w, seg_h, digit, 10,10,10);
        if (place>1) place /= 10;
    }
}

/* font bitmap 5x7 (space, 0-9, A-Z, ( ) . :) */
static const unsigned char font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // space
    {0x3E,0x45,0x49,0x51,0x3E}, //0
    {0x00,0x21,0x7F,0x01,0x00}, //1
    {0x21,0x43,0x45,0x49,0x31}, //2
    {0x42,0x41,0x51,0x69,0x46}, //3
    {0x0C,0x14,0x24,0x7F,0x04}, //4
    {0x72,0x51,0x51,0x51,0x4E}, //5
    {0x3E,0x49,0x49,0x49,0x26}, //6
    {0x40,0x47,0x48,0x50,0x60}, //7
    {0x36,0x49,0x49,0x49,0x36}, //8
    {0x32,0x49,0x49,0x49,0x3E}, //9
    {0x7C,0x12,0x11,0x12,0x7C}, //A
    {0x7F,0x49,0x49,0x49,0x36}, //B
    {0x3E,0x41,0x41,0x41,0x22}, //C
    {0x7F,0x41,0x41,0x22,0x1C}, //D
    {0x7F,0x49,0x49,0x49,0x41}, //E
    {0x7F,0x09,0x09,0x09,0x01}, //F
    {0x3E,0x41,0x49,0x49,0x7A}, //G
    {0x7F,0x08,0x08,0x08,0x7F}, //H
    {0x00,0x41,0x7F,0x41,0x00}, //I
    {0x20,0x40,0x41,0x3F,0x01}, //J
    {0x7F,0x08,0x14,0x22,0x41}, //K
    {0x7F,0x40,0x40,0x40,0x40}, //L
    {0x7F,0x02,0x0C,0x02,0x7F}, //M
    {0x7F,0x04,0x08,0x10,0x7F}, //N
    {0x3E,0x41,0x41,0x41,0x3E}, //O
    {0x7F,0x09,0x09,0x09,0x06}, //P
    {0x3E,0x41,0x51,0x21,0x5E}, //Q
    {0x7F,0x09,0x19,0x29,0x46}, //R
    {0x46,0x49,0x49,0x49,0x31}, //S
    {0x01,0x01,0x7F,0x01,0x01}, //T
    {0x3F,0x40,0x40,0x40,0x3F}, //U
    {0x1F,0x20,0x40,0x20,0x1F}, //V
    {0x7F,0x20,0x18,0x20,0x7F}, //W
    {0x63,0x14,0x08,0x14,0x63}, //X
    {0x07,0x08,0x70,0x08,0x07}, //Y
    {0x61,0x51,0x49,0x45,0x43}, //Z
    {0x00,0x1C,0x22,0x41,0x00}, //(
    {0x00,0x41,0x22,0x1C,0x00}, //)
    {0x00,0x60,0x60,0x00,0x00}, //.
    {0x00,0x08,0x08,0x00,0x00}  //:
};

int charIndex(char c) {
    if (c == ' ') return 0;
    if (c >= '0' && c <= '9') return 1 + (c - '0');
    if (c >= 'A' && c <= 'Z') return 11 + (c - 'A');
    if (c == '(') return 11 + 26;
    if (c == ')') return 12 + 26;
    if (c == '.') return 13 + 26;
    if (c == ':') return 14 + 26;
    return 0;
}

void drawChar(Image &img, int x, int y, char ch, u8 r,u8 g,u8 b) {
    if (ch >= 'a' && ch <= 'z') ch = ch - 'a' + 'A';
    int idx = charIndex(ch);
    const unsigned char *bmp = font5x7[idx];
    for (int col=0; col<5; ++col) {
        unsigned char coldata = bmp[col];
        for (int row=0; row<7; ++row) {
            if (coldata & (1 << row)) {
                img.setPixel(x + col, y + row, r,g,b);
            }
        }
    }
}
void drawText(Image &img, int x, int y, const std::string &text, u8 r=0,u8 g=0,u8 b=0) {
    int cx = x;
    for (char ch : text) {
        drawChar(img, cx, y, ch, r,g,b);
        cx += 6;
    }
}

// substitua pela nova implementação abaixo (cole no renderer.cpp)
void Renderer::renderHydrometerPNG(const std::string &filename, double totalLiters,
                                   int centenas, int dezenas, int litrosUnit, int decimos,
                                   double instantLps, double pressure) {
    // canvas maior
    const int W = 1100;
    const int H = 520;
    Image img(W,H,255,255,255);

    // ---- layout constants (ajuste à vontade) ----
    const int margin = 40;

    // mostrador principal (esquerda)
    const int cx = 240;
    const int cy = 240;
    const int R = 180;

    // pequenos mostradores (direita)
    const int baseX = 540;
    const int baseY = 60;
    const int spacingX = 150;
    const int dR = 52; // radius dos pequenos mostradores

    // odometro (abaixo do mostrador principal)
    const int odox = cx + 40;
    const int odoy = cy + 120;

    // barra de pressao (direita, com margem)
    const int pbar_w = 22;
    const int pbar_x = W - margin - pbar_w; // margem direita
    const int pbar_y = H - 60;
    const int pbar_h = 260;

    // top instant consumption box (centralizada)
    const int ibox_w = 700;
    const int ibox_h = 30;
    const int ibox_x = (W - ibox_w) / 2;
    const int ibox_y = 18;

    // ---- funções auxiliares rápidas locais ----
    auto fillRect = [&](int x0,int y0,int w,int h, u8 r,u8 g,u8 b){
        int x1 = x0 + w;
        int y1 = y0 + h;
        if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0;
        if (x1 > img.w) x1 = img.w;
        if (y1 > img.h) y1 = img.h;
        for (int y=y0;y<y1;y++) for (int x=x0;x<x1;x++) img.setPixel(x,y,r,g,b);
    };

    // ponteiro "grosso": desenha 3 linhas paralelas para dar espessura
    auto drawThickPointer = [&](int cxp,int cyp,double angleRad,double length,u8 r,u8 g,u8 b){
        // direção perpendicular
        double px = -sin(angleRad);
        double py = cos(angleRad);
        double offsets[3] = {-1.4, 0.0, 1.4};
        for (int i=0;i<3;i++) {
            double ox = px * offsets[i];
            double oy = py * offsets[i];
            drawLine(img, cxp + ox, cyp + oy, cxp + cos(angleRad)*length + ox, cyp + sin(angleRad)*length + oy, r,g,b);
        }
    };

    // ---- desenha mostrador principal ----
    drawCircle(img, cx, cy, R, 40,40,40);

    // ponteiro principal (fracao 0..999 -> -90..270 graus)
    int litersRest = static_cast<int>(std::fmod(totalLiters, 1000.0));
    double angle = (-M_PI/2.0) + ((double)litersRest / 1000.0) * (2*M_PI);
    drawThickPointer(cx, cy, angle, R - 30, 180, 0, 0);

    // rótulo dentro do mostrador (com fundo leve para não ficar perdido)
    drawText(img, cx - 18, cy - 6, "LITROS");

    // ---- pequenos mostradores (com espaçamento aumentado) ----
    int cx1 = baseX;
    int cx2 = baseX + spacingX;
    int cx3 = baseX + spacingX * 2;
    int cx4 = baseX + spacingX * 3;
    int cySmall = baseY + dR;

    auto drawSmall = [&](int cx_s, int cy_s, int value) {
        drawCircle(img, cx_s, cy_s, dR, 40,40,40);
        double a = (-M_PI/2.0) + ((double)value / 10.0) * (2*M_PI);
        drawPointer(img, cx_s, cy_s, a, dR - 14, 0,0,180);
    };
    drawSmall(cx1, cySmall, centenas);
    drawSmall(cx2, cySmall, dezenas);
    drawSmall(cx3, cySmall, litrosUnit);
    drawSmall(cx4, cySmall, decimos);

    // labels pequenos com fundo
    fillRect(cx1 - 32, cySmall + dR + 8, 90, 12, 240,240,240);
    drawText(img, cx1 - 28, cySmall + dR + 8, "CENTENAS");
    fillRect(cx2 - 32, cySmall + dR + 8, 90, 12, 240,240,240);
    drawText(img, cx2 - 26, cySmall + dR + 8, "DEZENAS");
    fillRect(cx3 - 24, cySmall + dR + 8, 80, 12, 240,240,240);
    drawText(img, cx3 - 20, cySmall + dR + 8, "UNIDADES");
    fillRect(cx4 - 32, cySmall + dR + 8, 110, 12, 240,240,240);
    drawText(img, cx4 - 26, cySmall + dR + 8, "DECIMOS");

    // ---- odometro (sete-seg) com fundo e borda para legibilidade ----
    int totalLitersInt = static_cast<int>(totalLiters);
    int odow = 6 * (12 + 5 + 6) + 10; // aproximado width
    int odox_box = odox - 8;
    int odoy_box = odoy - 10;
    fillRect(odox_box - 6, odoy_box - 6, odow + 20, 46, 230,230,230);
    drawOdometer(img, odox, odoy, 6, 12, 5, 6, totalLitersInt);
    drawText(img, odox - 40, odoy + 40, "TOTAL (L)");

    // ---- barra de pressao (visual) - com margem e fundo ----
    fillRect(pbar_x - 26, pbar_y - pbar_h - 6, 74, pbar_h + 12, 240,240,240);
    double pct = (pressure - 1.0) / (4.0 - 1.0);
    if (pct < 0) pct = 0; if (pct > 1) pct = 1;
    int fillh = static_cast<int>(pbar_h * pct);
    for (int y = pbar_y - fillh; y <= pbar_y; ++y)
        for (int x = pbar_x; x < pbar_x + pbar_w; ++x)
            img.setPixel(x,y,40,160,40);
    drawText(img, pbar_x - 36, pbar_y + 8, "PRESSAO (BAR)");

    // ---- barra de consumo instantaneo (top central) com label e fundo ----
    fillRect(ibox_x, ibox_y, ibox_w, ibox_h, 245,245,245);
    double maxLps = 2.0;
    double prop = instantLps / maxLps; if (prop<0) prop=0; if (prop>1) prop=1;
    int barW = static_cast<int>(ibox_w * prop);
    fillRect(ibox_x + 4, ibox_y + 4, barW, ibox_h - 8, 200,50,50);
    drawText(img, ibox_x + 4, ibox_y + ibox_h + 6, "CONSUMO (L/S)");

    // ---- small numeric overlays: instant consumption (left) and pressure (near bar) ----
    int inst_int = static_cast<int>(instantLps * 100.0); // centesimos
    drawOdometer(img, 40, 50, 4, 8, 3, 3, inst_int);
    int press_int = static_cast<int>(pressure * 10.0); // decimos
    drawOdometer(img, pbar_x - 46, 40, 3, 8, 3, 3, press_int);

    // ---- salvar png ----
    stbi_write_png(filename.c_str(), img.w, img.h, 3, img.data.data(), img.w*3);
}
