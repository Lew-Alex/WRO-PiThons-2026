#include <Arduino.h>


class Drive {
public:
    Drive(int pinA, int pinB, int pwmBits = 12, int pwmFreq = 20000);

    void setup();
    void move(float power);
    void brake();
    void coast();

private:
    int pinA, pinB;
    int pwmBits, pwmFreq;

    inline int maxDuty() const { return (1 << pwmBits) - 1; }
};
