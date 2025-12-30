#include "SparkFunLSM6DS3.h"
#include "Wire.h"

LSM6DS3 myIMU(I2C_MODE, 0x6A); // Try 0x6A first. If it fails, change to 0x6B.

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (myIMU.begin() != 0) {
    Serial.println("Failed to initialize LSM6DS3!");
    while (1);
  }

  Serial.println("LSM6DS3 initialized!");
}

void loop() {
  Serial.print("Gyro X: ");
  Serial.print(myIMU.readFloatGyroX());
  Serial.print(" Y: ");
  Serial.print(myIMU.readFloatGyroY());
  Serial.print(" Z: ");
  Serial.println(myIMU.readFloatGyroZ());
  
  delay(100);
}