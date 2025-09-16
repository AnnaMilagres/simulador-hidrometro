// main.cpp - sha v2.0 - controlador simples
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <iomanip>

#include "hydrometer.h"
#include "renderer.h"

int main() {
    Hydrometer h(2.0, 1.0, 4.0); // max 2 L/s; pressure 1..4 bar
    Renderer renderer; // objeto responsável por gerar a imagem

    const double dt = 1.0;
    double lastSavedLitersFloor = std::floor(h.getTotalLiters());
    int counter = 0;

    std::cout << "sha v2.0 iniciado. gerando hydrometer.png quando total aumenta +1 l\n";

    while (true) {
        h.step(dt);
        double total = h.getTotalLiters();
        double lps = h.getInstantLitersPerSec();
        double pressure = h.getPressureBar();

        int totalLitersInt = static_cast<int>(total);
        int litersRest = totalLitersInt % 1000;
        int centenas = (litersRest / 100) % 10;
        int dezenas = (litersRest / 10) % 10;
        int unidades = litersRest % 10;
        int decimos = static_cast<int>((total - totalLitersInt) * 10.0);

        // console resumido
        std::cout << "t=" << counter << "s | inst=" << std::fixed << std::setprecision(2)
                  << lps << " L/s | total=" << std::fixed << std::setprecision(2)
                  << total << " L | press=" << std::fixed << std::setprecision(2)
                  << pressure << " bar\n";

        // salva/sobrescreve imagem quando cruza novo litro inteiro
        double currentFloor = std::floor(total);
        if (currentFloor >= lastSavedLitersFloor + 1.0) {
            lastSavedLitersFloor = currentFloor;
            renderer.renderHydrometerPNG("hydrometer.png",
                                         total, centenas, dezenas, unidades, decimos,
                                         lps, pressure);
            std::cout << "imagem atualizada: hydrometer.png (total L = " << total << ")\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds((int)(dt*1000)));
        counter++;
    }

    return 0;
}