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
    lastTime = millis();
    return true;
}

void IMU::calibrateGyro(int samples, int delayMS) {
    Serial.println("Calibrating gyro... keep IMU still.");
    long sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += imu.readRawGyroZ();
        delay(delayMS);
    }
    gyroZOffset = sum / (float)samples;
    Serial.print("Gyro Z offset (raw LSB): ");
    Serial.println(gyroZOffset, 3);
}

void IMU::update() {
    unsigned long now = millis();
    float dt = (now - lastTime) / 1000.0;
    lastTime = now;

    float gz_raw = imu.readRawGyroZ();
    float gz_dps = (gz_raw - gyroZOffset) * 0.07; // convert LSB -> deg/sec
    headingAngle += gz_dps * dt;

    prevHeadingAngle = headingAngle - gz_dps * dt; // store previous for delta
}

void IMU::reset(){
    headingAngle = 0.0;
    prevHeadingAngle = 0.0;
}

float IMU::getHeadingDeg() const { return headingAngle; }
float IMU::getHeadingRad() const { return headingAngle * PI / 180.0; }
float IMU::getDeltaHeadingRad() const { return (headingAngle - prevHeadingAngle) * PI / 180.0; }