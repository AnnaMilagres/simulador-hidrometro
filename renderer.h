// renderer.h - interface para geração de imagem do hidrômetro
#ifndef RENDERER_H
#define RENDERER_H

#include <string>

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    // gera/salva a imagem do hidrômetro (sobrescreve o arquivo)
    // totalLiters: total em litros
    // centenas,dezenas,unidades,decimos: valores dos ponteiros
    // instantLps: consumo instantâneo (L/s)
    // pressure: pressão em bar
    void renderHydrometerPNG(const std::string &filename,
                             double totalLiters,
                             int centenas, int dezenas, int unidades, int decimos,
                             double instantLps, double pressure);
};

#endif // RENDERER_H
