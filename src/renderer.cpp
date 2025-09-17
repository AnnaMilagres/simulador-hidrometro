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

void Renderer::renderHydrometerPNG(const std::string &filename,
                                   double totalLiters,
                                   int centenas, int dezenas, int unidades, int decimos,
                                   double instantLps, double pressure) {
    const int W = 600, H = 400;
    Image img(W, H, 255, 255, 255);

    // --- caixa do hidrômetro ---
    fillRect(img, 20, 20, W-40, H-40, 245, 245, 245);
    fillRect(img, 20, 20, W-40, 2, 30, 30, 30);           // topo
    fillRect(img, 20, H-22, W-40, 2, 30, 30, 30);         // baixo
    fillRect(img, 20, 20, 2, H-40, 30, 30, 30);           // esquerda
    fillRect(img, W-22, 20, 2, H-40, 30, 30, 30);

    // --- odômetro (parte de cima) ---
    int odox = 200, odoy = 60;
    int totalInt = static_cast<int>(totalLiters);
    drawOdometer(img, odox, odoy, 6, 16, 6, 8, totalInt);
    drawText(img, odox + 120, odoy + 25, "m³");

    // --- mostrador central ---
    int cx = W/2 - 100, cy = H/2 + 40, R = 80;
    drawCircle(img, cx, cy, R, 0, 0, 0);
    int litersRest = static_cast<int>(std::fmod(totalLiters, 10.0));
    double angle = (-M_PI/2.0) + (litersRest/10.0) * 2*M_PI;
    drawPointer(img, cx, cy, angle, R-15, 200, 0, 0);
    drawText(img, cx-20, cy+R+20, "Litros");

    // --- pequenos mostradores embaixo ---
    int baseX = W/2 + 40, baseY = H/2;
    int dR = 40;
    auto drawSmall = [&](int cx, int cy, int value, const std::string &label) {
        drawCircle(img, cx, cy, dR, 0, 0, 0);
        double a = (-M_PI/2.0) + ((double)value/10.0)*2*M_PI;
        drawPointer(img, cx, cy, a, dR-12, 0, 0, 200);
        drawText(img, cx-15, cy+dR+15, label);
    };
    drawSmall(baseX, baseY, centenas, "x0.1");
    drawSmall(baseX+90, baseY, dezenas, "x0.01");
    drawSmall(baseX, baseY+100, unidades, "x0.001");
    drawSmall(baseX+90, baseY+100, decimos, "x0.0001");

    // --- salvar ---
    stbi_write_png(filename.c_str(), img.w, img.h, 3, img.data.data(), img.w*3);
}
