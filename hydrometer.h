#ifndef HYDROMETER_H
#define HYDROMETER_H

#include <cstddef>

class Hydrometer {
private:
    double flow_ml_per_h;     // vazão instantânea
    double pressure;          // pressão da água
    double cumulative_ml;     // volume total acumulado
    double ml_per_pulse;      // quantos ml contam como 1 pulso
    size_t pulses;            // contador de pulsos

public:
    Hydrometer(double ml_per_pulse = 100.0);

    void setFlow(double ml_per_h);
    void setPressure(double p);

    void step(double dt_seconds);

    double getFlow() const;
    double getPressure() const;
    double getTotalMl() const;
    size_t getPulses() const;
};

#endif