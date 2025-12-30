#pragma once
#include <Arduino.h>
#include <Encoder.h>

class TrackingWheel {
public:
    TrackingWheel(int pinA, int pinB, float cpr, float wheelDiameterMM);

    void init();

    void update();

    void reset();

    long getCounts() const;
    long getDeltaCounts() const;

    float getDistanceMM() const;
    float getDeltaDistanceMM() const;

private:
    Encoder encoder;

    const float CPR;
    const float WHEEL_DIAMETER_MM;
    float WHEEL_CIRCUM_MM;

    long prevCounts;
    long deltaCounts;

    float distanceMM;
    float deltaDistanceMM;
};