#ifndef HYDROMETER_H
#define HYDROMETER_H

#include <random>

class Hydrometer {
public:
    Hydrometer(double liters_per_second_max = 2.0, double press_min = 1.0, double press_max = 4.0);

    void step(double dt_seconds);

    double getInstantLitersPerSec() const;
    double getPressureBar() const;
    double getTotalLiters() const;

private:
    std::mt19937 gen;
    std::uniform_real_distribution<> consumoDist;
    std::uniform_real_distribution<> pressaoDist;

    // estado
    double instant_lps;
    double pressure_bar;
    double total_liters;
};

#endif