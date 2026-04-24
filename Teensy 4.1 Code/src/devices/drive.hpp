#pragma once
#include <Arduino.h>
#include <Encoder.h>
#include "CytronMotorDriver.h"


#define MOTOR_CURVE_SAMPLE_POINTS 3

struct MotorPoint{
    uint8_t power; // 0 - 255
    uint8_t speed; // RPM (0 - 200)
};

class Drive {
private:
    // Motor power curve
    static MotorPoint sample_points[MOTOR_CURVE_SAMPLE_POINTS];

    uint8_t speedToPower(uint8_t speed);
    uint8_t powerToSpeed(uint8_t power);

public:
    Drive(uint8_t PWM1, uint8_t PWM2, uint8_t ENC1 = UINT8_MAX, uint8_t ENC2 = UINT8_MAX);

    // Movement Code
    void move(float speed);
    void coast();

    // Sensor Code
    float getRotations();

private:
    uint8_t PWM1, PWM2, ENC1, ENC2;
    CytronMD motor;
    Encoder encoder;
};
