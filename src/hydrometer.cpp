#include "hydrometer.h"
#include <chrono>

Hydrometer::Hydrometer(double liters_per_second_max, double press_min, double press_max)
    : gen((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count()),
      consumoDist(0.0, liters_per_second_max),
      pressaoDist(press_min, press_max),
      instant_lps(0.0),
      pressure_bar(press_min),
      total_liters(0.0)
{}

void Hydrometer::step(double dt_seconds) {
    // gera novos valores aleatórios
    instant_lps = consumoDist(gen);
    pressure_bar = pressaoDist(gen);

    // acumula volume
    total_liters += instant_lps * dt_seconds;
}

double Hydrometer::getInstantLitersPerSec() const { return instant_lps; }
double Hydrometer::getPressureBar() const { return pressure_bar; }
double Hydrometer::getTotalLiters() const { return total_liters; }