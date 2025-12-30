#include <QuadratureEncoder.h>
// Must also have enableInterrupt.h library

// Use any 2 pins for interrupts (digital pins recommended)
Encoders leftEncoder(10, 11);  // Create an Encoder object

// Set your encoder CPR here
const float CPR = 4096.0 * 4;  // quadrature counts per revolution

// Wheel diameter in mm
const float WHEEL_DIAMETER_MM = 38.0;
const float INCHES_PER_MM = 1.0 / 25.4;
const float WHEEL_CIRCUMFERENCE_IN = WHEEL_DIAMETER_MM * 3.14159265 * INCHES_PER_MM;

unsigned long lastMilli = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (millis() - lastMilli > 50) {
    long counts = leftEncoder.getEncoderCount();

    // Convert counts to degrees
    float angle = (counts / CPR) * 360.0;
    angle = fmod(angle, 360.0);
    if (angle < 0) angle += 360.0;

    // Convert counts to linear inches
    float inches = (counts / CPR) * WHEEL_CIRCUMFERENCE_IN;

    Serial.print("Counts: "); Serial.print(counts);
    Serial.print(" | Degrees: "); Serial.print(angle, 2);
    Serial.print(" | Inches: "); Serial.println(inches, 3);

    lastMilli = millis();
  }
}