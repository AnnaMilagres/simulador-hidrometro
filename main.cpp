#include "hydrometer.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>

int main() {
    Hydrometer h(50.0); // cada 50 ml conta 1 pulso

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> flowDist(0.0, 120.0);

    std::normal_distribution<> pressureDist(1.0, 0.1);

    const double dt_s = 1.0;
    long long time_s = 0;

    std::cout << "time(s)\tflow(ml/h)\tpressure\tcumulative_ml\tpulses\n";

    while (true) {
        double newFlow = flowDist(gen);
        double newPressure = pressureDist(gen);

        h.setFlow(newFlow);
        h.setPressure(newPressure);

        h.step(dt_s);

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

        std::this_thread::sleep_for(std::chrono::seconds(1));
        time_s += 1;
    }

    return 0;
}