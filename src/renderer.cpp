// renderer.cpp - implementação do renderer usando módulos organizados
#include "renderer.h"
#include "graphics/image.h"
#include "graphics/drawing.h"
#include "graphics/text.h" 
#include "graphics/display.h"
#include <cmath>

// stb imagem writer (definição apenas aqui)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "external/stb_image_write.h"

void Renderer::renderHydrometerPNG(const std::string &filename, double totalLiters,
                                   int centenas, int dezenas, int unidades, int decimos,
                                   double instantLps, double pressure) {
    const int W = 700, H = 420;
    Image img(W, H, 255, 255, 255);

    // ---- mostrador principal (mostrador analógico central) ----
    int cx = 170, cy = 210, R = 140;
    drawCircle(img, cx, cy, R, 30, 30, 30);

    // ponteiro principal (mapeia litrosRest 0..999 para volta completa)
    int litersRest = static_cast<int>(std::fmod(totalLiters, 1000.0));
    double angle = (-M_PI/2.0) + ((double)litersRest / 1000.0) * (2*M_PI);
    drawPointer(img, cx, cy, angle, R-25, 200, 0, 0);

    // ---- pequenos mostradores (centenas, dezenas, unidades, décimos) ----
    int baseX = 380, baseY = 40;
    int dR = 44;
    int cx1 = baseX, cy1 = baseY + dR;
    int cx2 = baseX + 110, cy2 = baseY + dR;
    int cx3 = baseX + 220, cy3 = baseY + dR;
    int cx4 = baseX + 330, cy4 = baseY + dR;

    auto drawSmall = [&](int cx, int cy, int value) {
        drawCircle(img, cx, cy, dR, 30, 30, 30);
        double a = (-M_PI/2.0) + ((double)value / 10.0) * (2*M_PI);
        drawPointer(img, cx, cy, a, dR-12, 0, 0, 200);
    };
    
    drawSmall(cx1, cy1, centenas);
    drawSmall(cx2, cy2, dezenas);
    drawSmall(cx3, cy3, unidades);
    drawSmall(cx4, cy4, decimos);

    // ---- odômetro sete-segmentos (caixa com fundo) ----
    int totalLitersInt = static_cast<int>(totalLiters);
    int odox = 320, odoy = 220;
    int odow = 6 * (12 + 5 + 6); // largura estimada
    int odox_box = odox - 6, odoy_box = odoy - 6;
    
    fillRect(img, odox_box - 6, odoy_box - 6, odow + 20, 46, 230, 230, 230);
    drawOdometer(img, odox, odoy, 6, 12, 5, 6, totalLitersInt);
    drawText(img, odox - 40, odoy + 40, "TOTAL (L)");

    // ---- barra de pressão (visual) - com margem e fundo ----
    int pbar_x = 560, pbar_y = 320, pbar_h = 220, pbar_w = 18;
    fillRect(img, pbar_x - 26, pbar_y - pbar_h - 6, 74, pbar_h + 12, 240, 240, 240);
    double pct = (pressure - 1.0) / (4.0 - 1.0);
    if (pct < 0) pct = 0; if (pct > 1) pct = 1;
    int fillh = static_cast<int>(pbar_h * pct);
    
    for (int y = pbar_y - fillh; y <= pbar_y; ++y) {
        for (int x = pbar_x; x < pbar_x + pbar_w; ++x) {
            img.setPixel(x, y, 40, 160, 40);
        }
    }
    drawText(img, pbar_x - 36, pbar_y + 8, "PRESSAO (BAR)");

    // ---- barra de consumo instantâneo (top central) com label e fundo ----
    int ibox_x = 60, ibox_y = 20, ibox_w = 520, ibox_h = 28;
    fillRect(img, ibox_x, ibox_y, ibox_w, ibox_h, 245, 245, 245);
    double maxLps = 2.0;
    double prop = instantLps / maxLps; 
    if (prop < 0) prop = 0; if (prop > 1) prop = 1;
    int barW = static_cast<int>(ibox_w * prop);
    fillRect(img, ibox_x + 4, ibox_y + 4, barW, ibox_h - 8, 200, 50, 50);
    drawText(img, ibox_x + 4, ibox_y + ibox_h + 6, "CONSUMO (L/S)");

    // ---- small numeric overlays: instant consumption (left) and pressure (near bar) ----
    int inst_int = static_cast<int>(instantLps * 100.0); // centésimos
    drawOdometer(img, 40, 50, 4, 8, 3, 3, inst_int);
    int press_int = static_cast<int>(pressure * 10.0); // décimos
    drawOdometer(img, pbar_x - 46, 40, 3, 8, 3, 3, press_int);

    // ---- salvar png ----
    stbi_write_png(filename.c_str(), img.w, img.h, 3, img.data.data(), img.w*3);
}