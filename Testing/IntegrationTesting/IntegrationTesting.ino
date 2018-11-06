#include "Instruments.h"
#include "Logger.h"
//#include "Motor.h"

Instruments instruments;
Logger logger;
const int sdChipSelect = 10;
//Motor motor;
uint8_t pwmPin = 9;
uint8_t in1Pin = 8;
uint8_t in2Pin = 7;

float headingTarget = 180;

void setup() {
    Serial.begin(9600);
//    motor = Motor(pwmPin, in1Pin, in2Pin);
    instruments = Instruments(true);
    logger = Logger(sdChipSelect);
}

void loop() {
//    motor.setFloatSpeed(0.5);
    float heading = instruments.getHeading();
    String logLine = "Heading = " + String(heading);
    Serial.println(logLine);
    logger.log("Instruments", logLine);
}