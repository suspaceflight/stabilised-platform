#include "Motor.h"

Motor::Motor(uint8_t pwmPin, uint8_t in1Pin, uint8_t in2Pin) {
    this->pwmPin = pwmPin;
    this->in1Pin = in1Pin;
    this->in2Pin = in2Pin;
    
    pinMode(pwmPin, OUTPUT);
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    
    // Turn everything off.
    disable();
}

Motor::setActiveDirection(bool clockwise) {
    if (clockwise) {
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);
    } else {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, HIGH);
    }
}

Motor::disable() {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    analogWrite(pwmPin, 0);    
}
