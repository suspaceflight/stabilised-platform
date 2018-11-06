#include "Motor.h"
#include "Logger.h"

Logger logger;
const int sdChipSelect = 10;
Motor motor;
uint8_t pwmPin = 9;
uint8_t in1Pin = 8;
uint8_t in2Pin = 7;

float headingTarget = 180;

void setup() {
    Serial.begin(9600);
    motor = Motor(pwmPin, in1Pin, in2Pin);
    logger = Logger(sdChipSelect);
}

void loop() {
    motor.setFloatSpeed(0.5);
    String logLine = "A line to log... y'know";
    Serial.println(logLine);
    logger.log("That log line", logLine);
}