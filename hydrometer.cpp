#include "hydrometer.h"
#include <cmath>

Hydrometer::Hydrometer(double ml_per_pulse)
    : flow_ml_per_h(0.0),
      pressure(1.0),
      cumulative_ml(0.0),
      ml_per_pulse(ml_per_pulse),
      pulses(0) {}

void Hydrometer::setFlow(double ml_per_h) {
    flow_ml_per_h = ml_per_h;
}

void Hydrometer::setPressure(double p) {
    pressure = p;
}

void Hydrometer::step(double dt_seconds) {
    double ml_passados = flow_ml_per_h * (dt_seconds / 3600.0);
    if (ml_passados < 0) ml_passados = 0;

    cumulative_ml += ml_passados;

    size_t expected_pulses = static_cast<size_t>(cumulative_ml / ml_per_pulse);
    if (expected_pulses > pulses) {
        pulses = expected_pulses;
    }
}

double Hydrometer::getFlow() const {
    return flow_ml_per_h;
}

double Hydrometer::getPressure() const {
    return pressure;
}

double Hydrometer::getTotalMl() const {
    return cumulative_ml;
}

size_t Hydrometer::getPulses() const {
    return pulses;
}