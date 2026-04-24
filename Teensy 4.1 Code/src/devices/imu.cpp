#include "imu.hpp"

IMU::IMU(uint8_t address)
    : imu(I2C_MODE, address),
      headingAngle(0.0),
      prevHeadingAngle(0.0),
      gyroZOffset(0.0),
      lastTime(0)
{}

bool IMU::init() {
    Wire.begin();
    if (imu.begin() != 0) {
        Serial.println("IMU initialization failed!");
        return false;
    }
    lastTime = micros();
    return true;
}

void IMU::calibrateGyro(int samples, int delayMS) {
    float sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += imu.readFloatGyroZ();
        delay(delayMS);
    }
    gyroZOffset = sum / samples;
}

void IMU::update() {
    unsigned long now = micros();
    float dt = (now - lastTime) / 1000000.0;
    lastTime = now;

    float gz_dps = imu.readFloatGyroZ() - gyroZOffset;

    prevHeadingAngle = headingAngle;
    headingAngle += gz_dps * dt;
}

void IMU::reset(){
    headingAngle = 0.0;
    prevHeadingAngle = 0.0;
}

float IMU::getHeadingDeg() const { return headingAngle; }
float IMU::getHeadingRad() const { return headingAngle * PI / 180.0; }
float IMU::getDeltaHeadingRad() const { return (headingAngle - prevHeadingAngle) * PI / 180.0; }