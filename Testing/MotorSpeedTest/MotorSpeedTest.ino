#include <Arduino.h>
#include "Motor.h"

Motor motor;
uint8_t pwmPin = 10;
uint8_t in1Pin = 9;
uint8_t in2Pin = 8;

void setup() {
    motor = Motor(pwmPin, in1Pin, in2Pin);
}

void loop() {
    motor.setFloatSpeed(-0.5);
    delay(2500);
    motor.disable();
    delay(5000);
    motor.setFloatSpeed(0.5);
    delay(2500);
    motor.disable();
    delay(5000);
}