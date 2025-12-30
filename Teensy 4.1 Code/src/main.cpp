#include <Arduino.h>
#include "chassis/odom.hpp"
#include "devices/drive.hpp"

Drive drive_l(10, 11);
Drive drive_r(12, 13);

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  delay(100 ); 

  drive_l.setup();
  drive_r.setup();
  odom.start();
}

void loop() {
  while (true){
    Pose pos = odom.getPose();
    Serial.printf("Odom | %8.3f %8.3f %8.3f\n", pos.x, pos.y, radToDeg(pos.a));

    delay(10);
  }
}