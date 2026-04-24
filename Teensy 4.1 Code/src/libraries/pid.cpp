#include "pid.hpp"
#include <cmath>

PID::PID(double kP, double kI, double kD,
         double bias,
         bool integralResetOnSignChange,
         double integralDeadZone,
         double integralLimit,
         double outputLimit)
: kP(kP), kI(kI), kD(kD),
  bias(bias),
  integralResetOnSignChange(integralResetOnSignChange),
  integralDeadZone(integralDeadZone),
  integralLimit(integralLimit),
  outputLimit(outputLimit),
  timer("PID", true, TimingUnit::MICROS)
{}

double PID::compute(double input, double target) {
    double dt = timer.getTime() / 1000000.0; // microseconds → seconds
    timer.reset(true);

    if (dt <= 0) return output;

    error = target - input;

    // P
    proportional = kP * error;

    // I
    if (kI != 0) {
        if (integralResetOnSignChange &&
            ((error > 0 && lastError < 0) || (error < 0 && lastError > 0))) {
            integral = 0;
        }

        if (fabs(error) > integralDeadZone) {
            integral += error * dt * kI;

            if (integral > integralLimit) integral = integralLimit;
            if (integral < -integralLimit) integral = -integralLimit;
        }
    }

    // D
    derivative = (error - lastError) / dt * kD;

    lastError = error;

    output = proportional + integral + derivative + bias;

    // clamp output
    if (output > outputLimit) output = outputLimit;
    if (output < -outputLimit) output = -outputLimit;

    return output;
}

void PID::reset() {
    integral = 0;
    output = 0;
    lastError = 0;
    timer.reset(true);
}

double PID::getError() const { return error; }
double PID::getOutput() const { return output; }