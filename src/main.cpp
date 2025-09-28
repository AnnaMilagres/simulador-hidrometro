// main.cpp - Versao MULTITHREAD (requer GCC 8+ e -pthread)
#include <iostream>
#include <cmath> 
#include <chrono> 
#include <iomanip>
#include <vector>
#include <string>
#include <thread>
#include <tuple>
#include <algorithm> 

#include "hydrometer.h"
#include "renderer.h"

// O Renderer é compartilhado, mas como cada thread renderiza em um arquivo diferente,
// o risco de conflito é menor. Idealmente seria protegido por um mutex para garantir a atomicidade
// da chamada da funcao de renderizacao.
Renderer globalRenderer; 

/**
 * @brief Funcao de simulacao para um unico hidrometro, rodando em sua propria thread.
 * * @param id ID do hidrometro (1 a 5)
 * @param max_lps 
 * @param min_pressao 
 * @param max_pressao 
 */

void simular_hidrometro(int id, double max_lps, double min_pressao, double max_pressao) {
    Hydrometer h(max_lps, min_pressao, max_pressao);
    
    std::string filename = "hidrometro_" + std::to_string(id) + ".png";
    
    double lastSavedLitersFloor = std::floor(h.getTotalLiters());
    
    const double dt = 0.1; 
    const auto step_duration = std::chrono::milliseconds((int)(dt * 1000));
    
    std::cout << "Thread H#" << id << " iniciada." << std::endl;

    // --- Loop Principal de Simulação ---
    while (true) {
        h.step(dt);
        double total = h.getTotalLiters();
        double lps = h.getInstantLitersPerSec();
        double pressure = h.getPressureBar();

        int totalLitersInt = static_cast<int>(std::floor(total));
        
        int litersRest = totalLitersInt % 1000;
        
        int centenas = (litersRest / 100) % 10;
        int dezenas = (litersRest / 10) % 10;
        int unidades = litersRest % 10;
        
        int decimos = static_cast<int>((total - totalLitersInt) * 10.0);

        double currentFloor = std::floor(total);
        if (currentFloor >= lastSavedLitersFloor + 1.0) {
            
            globalRenderer.renderHydrometerPNG(filename,
                                         total, centenas, dezenas, unidades, decimos,
                                         lps, pressure);
            
            lastSavedLitersFloor = currentFloor;

            std::cout << "H#" << id << " IMAGEM ATUALIZADA: " << filename 
                      << " (total L = " << std::fixed << std::setprecision(2) << total << ") - LPS: " 
                      << std::fixed << std::setprecision(2) << lps << std::endl;
        }

        std::this_thread::sleep_for(step_duration);
    }
}


int main() {
    std::vector<std::tuple<int, double, double, double>> configs = {
        {1, 2.0, 1.0, 4.0}, // Hidrometro 1: Padrao
        {2, 1.5, 0.5, 3.0}, // Hidrometro 2: Vazao um pouco menor e pressao mais baixa
        {3, 3.5, 2.0, 5.0}, // Hidrometro 3: Vazao alta e pressao alta
        {4, 1.0, 1.5, 2.5}, // Hidrometro 4: Vazao e pressao estaveis/baixas
        {5, 5.0, 0.1, 6.0}  // Hidrometro 5: Vazao muito alta e pressao instavel
    };
    
    std::vector<std::thread> threads;

    std::cout << "Iniciando simulacao MULTITHREAD para " << configs.size() << " hidrometros..." << std::endl;
    std::cout << "Pressione Ctrl + C para encerrar todas as simulacoes." << std::endl;

    // Lanca as threads
    for (const auto& config : configs) {
        int id = std::get<0>(config);
        double max_lps = std::get<1>(config);
        double min_pressao = std::get<2>(config);
        double max_pressao = std::get<3>(config);
        
        threads.emplace_back(simular_hidrometro, id, max_lps, min_pressao, max_pressao);
    }
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}
