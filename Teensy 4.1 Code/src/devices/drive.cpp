#include "drive.hpp"
#include "../libraries/math.hpp"

MotorPoint Drive::sample_points[MOTOR_CURVE_SAMPLE_POINTS] = {{0, 0}, {60, 10}, {255, 100}};


Drive::Drive(uint8_t PWM1, uint8_t PWM2, uint8_t ENC1, uint8_t ENC2)
    : PWM1(PWM1), PWM2(PWM2), ENC1(ENC1), ENC2(ENC2), encoder(ENC1, ENC2), motor(PWM_PWM, PWM1, PWM2)
    {}


// Motor power Curve
uint8_t Drive::speedToPower(uint8_t speed){
    if (speed < sample_points[0].speed) return sample_points[0].speed;
    if (speed > sample_points[MOTOR_CURVE_SAMPLE_POINTS - 1].speed) return sample_points[MOTOR_CURVE_SAMPLE_POINTS - 1].speed;

    // Find correct segment
    for (int i = 0; i < MOTOR_CURVE_SAMPLE_POINTS - 1; i++) {
        MotorPoint p1 = sample_points[i];
        MotorPoint p2 = sample_points[i + 1];

        if (speed >= p1.speed && speed <= p2.speed) {
            // Linear interpolation
            float ratio = float(speed - p1.speed) / float(p2.speed - p1.speed);
            float power = p1.power + ratio * (p2.power - p1.power);

            return (uint8_t)power;
        }
    }

    return 0;
}

uint8_t Drive::powerToSpeed(uint8_t power){
    if (power < sample_points[0].power) return sample_points[0].speed;
    if (power > sample_points[MOTOR_CURVE_SAMPLE_POINTS - 1].power) return sample_points[MOTOR_CURVE_SAMPLE_POINTS - 1].speed;

    // Find correct segment
    for (int i = 0; i < MOTOR_CURVE_SAMPLE_POINTS - 1; i++) {
        MotorPoint p1 = sample_points[i];
        MotorPoint p2 = sample_points[i + 1];

        if (power >= p1.power && power <= p2.power) {
            // Linear interpolation
            float ratio = float(power - p1.power) / float(p2.power - p1.power);
            float speed = p1.speed + ratio * (p2.speed - p1.speed);

            return (uint8_t)speed;
        }
    }

    return 0;
}


void Drive::move(float speed) {
    int16_t power = speedToPower((uint8_t)fabs(speed)) * sgn(speed);
    motor.setSpeed(power);
}


void Drive::coast() {
    motor.setSpeed(0.0);
}


float Drive::getRotations(){
    float rotations = encoder.read() / (28.0 * 150.0);
    return rotations;
}