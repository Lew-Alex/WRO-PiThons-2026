#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "SparkFunLSM6DS3.h"

class IMU {
public:
    IMU(uint8_t address = 0x6B);

    bool init();

    void calibrateGyro(int samples = 100, int delayMS = 5);

    void update();

    void reset();

    float getHeadingDeg() const;
    float getHeadingRad() const;
    float getDeltaHeadingRad() const;

    LSM6DS3 imu;

private:
    
    float headingAngle;      // integrated heading in degrees
    float prevHeadingAngle;  // previous heading for delta calculation
    float gyroZOffset;       // gyro bias
    unsigned long lastTime;  // millis() of last update
};