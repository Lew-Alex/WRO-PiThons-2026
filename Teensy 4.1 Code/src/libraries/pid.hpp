#pragma once
#include <Arduino.h>
#include "timer.hpp"

class PID {
public:
    PID(double kP, double kI, double kD,
        double bias = 0,
        bool integralResetOnSignChange = false,
        double integralDeadZone = 0,
        double integralLimit = 1000,
        double outputLimit = 255);

    double compute(double input, double target);
    void reset();

    double getError() const;
    double getOutput() const;

private:
    double kP, kI, kD, bias;

    double error = 0;
    double lastError = 0;

    double proportional = 0;
    double integral = 0;
    double derivative = 0;
    double output = 0;

    bool integralResetOnSignChange;
    double integralDeadZone;
    double integralLimit;
    double outputLimit;

    Timer timer;
};