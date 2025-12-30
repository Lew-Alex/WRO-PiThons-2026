#include "trackingWheel.hpp"

TrackingWheel::TrackingWheel(int pinA, int pinB, float cpr, float wheelDiameterMM)
    : encoder(pinA, pinB),
      CPR(cpr * 4.0),
      WHEEL_DIAMETER_MM(wheelDiameterMM)
{
    WHEEL_CIRCUM_MM = WHEEL_DIAMETER_MM * PI;
    prevCounts = 0;
    distanceMM = 0;
    deltaDistanceMM = 0;
    deltaCounts = 0;
}

void TrackingWheel::init() {
    encoder.write(0);
    prevCounts = 0;
    distanceMM = 0;
    deltaDistanceMM = 0;
    deltaCounts = 0;
}

void TrackingWheel::update() {
    long newCounts = encoder.read();
    deltaCounts = newCounts - prevCounts;

    distanceMM = (newCounts / CPR) * WHEEL_CIRCUM_MM;
    deltaDistanceMM = (deltaCounts / CPR) * WHEEL_CIRCUM_MM;

    prevCounts = newCounts;
}

void TrackingWheel::reset() {
    encoder.write(0);
    prevCounts = 0;
    distanceMM = 0;
    deltaDistanceMM = 0;
    deltaCounts = 0;
}

long TrackingWheel::getCounts() const { return prevCounts; }
long TrackingWheel::getDeltaCounts() const { return deltaCounts; }

float TrackingWheel::getDistanceMM() const { return distanceMM; }
float TrackingWheel::getDeltaDistanceMM() const { return deltaDistanceMM; }