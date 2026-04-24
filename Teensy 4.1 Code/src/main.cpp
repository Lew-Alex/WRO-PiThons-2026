#include <Arduino.h>
#include "config.hpp"
#include "run.hpp"
#include "localization.hpp"
#include "libraries/timer.hpp"
#include "libraries/pid.hpp"
#include "libraries/util.hpp"
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(
    TCS34725_INTEGRATIONTIME_50MS,
    TCS34725_GAIN_4X
);

const int ledPin = 14;


void setup() {
    Serial.begin(115200);
    delay(500); // allow time to open serial monitor
    positioning.setup();
    
    tcs.begin();

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
}

bool running = true;
void loop() {

    uint16_t r, g, b, c;

    tcs.getRawData(&r, &g, &b, &c);

    Serial.print("R: "); Serial.print(r);

    Serial.print(" G: "); Serial.print(g);

    Serial.print(" B: "); Serial.print(b);

    Serial.print(" C: "); Serial.println(c);

    // if (running){
    //     positioning.localizationCycle();
    //     autoCycle();

    // }


    
    // Pose pos = positioning.getPose();

    delay(100);

}