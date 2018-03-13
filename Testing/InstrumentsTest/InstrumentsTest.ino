#include <Arduino.h>
#include "Instruments.h"

Instruments instruments;

void setup() {
    Serial.begin(9600);
    Serial.println("Initializing...");
    instruments = Instruments(true);
}

void loop() {
    Serial.println("Heading = " + String(instruments.getHeading()) + ", Altitude = " + String(instruments.getAltitude()));
    delay(500);
}