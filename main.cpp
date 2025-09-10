#include "hydrometer.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>

int main() {
    Hydrometer h(50.0); // cada 50 ml conta 1 pulso

    // geradores de aleatoriedade
    std::random_device rd;
    std::mt19937 gen(rd());

    // intervalo de vazão (ml/h)
    std::uniform_real_distribution<> flowDist(0.0, 120.0);

    // pressão média 1.0 com variação pequena
    std::normal_distribution<> pressureDist(1.0, 0.1);

    const double dt_s = 1.0; // atualização a cada 1 segundo
    long long time_s = 0;

    std::cout << "time(s)\tflow(ml/h)\tpressure\tcumulative_ml\tpulses\n";

    while (true) {
        double newFlow = flowDist(gen);
        double newPressure = pressureDist(gen);

        // define novos valores no hidrômetro
        h.setFlow(newFlow);
        h.setPressure(newPressure);

        // avança o tempo da simulação
        h.step(dt_s);

        // checa se há fluxo de água
        if (h.getFlow() < 0.1) {
            std::cout << time_s << "\tSEM FLUXO DE AGUA\n";
        } else {
            std::cout << time_s << "\t"
                      << std::fixed << std::setprecision(2)
                      << h.getFlow() << "\t\t"
                      << h.getPressure() << "\t\t"
                      << h.getTotalMl() << "\t"
                      << h.getPulses() << "\n";
        }

        // avança tempo real
        std::this_thread::sleep_for(std::chrono::seconds(1));
        time_s += 1;
    }

    return 0;
}