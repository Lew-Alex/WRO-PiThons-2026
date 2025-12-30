#include "drive.hpp"

Drive::Drive(int pinA, int pinB, int pwmBits, int pwmFreq)
    : pinA(pinA), pinB(pinB), pwmBits(pwmBits), pwmFreq(pwmFreq) {}

void Drive::setup() {
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    analogWriteResolution(pwmBits);
    analogWriteFrequency(pinA, pwmFreq);
    analogWriteFrequency(pinB, pwmFreq);
    coast();
}

void Drive::move(float p) {
    p = p / 100.0;
    if (p > 1.0f) p = 1.0f;
    if (p < -1.0f) p = -1.0f;

    if (fabs(p) < 0.0001f) {
        brake();
        return;
    }

    int duty = (int)(fabs(p) * maxDuty());

    if (p > 0) {
        analogWrite(pinA, duty);
        digitalWrite(pinB, LOW);
    } else {
        digitalWrite(pinA, LOW);
        analogWrite(pinB, duty);
    }
}

void Drive::brake() {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
}

void Drive::coast() {
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
}