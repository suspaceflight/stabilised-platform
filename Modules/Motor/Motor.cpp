#include "Motor.h"

/**
 * Allows for creation of Motor object without initialization.
 */
Motor::Motor() {}

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

Motor::setSpeed(uint8_t speed) {
    analogWrite(pwmPin, speed);
}

Motor::setFloatSpeed(float speed) {
    // TODO: Add logger and error message for these checks.
    if (speed > 1) {
        speed = 1;
    }
    if (speed < -1) {
        speed = -1;
    }
    setActiveDirection(!(speed < 0));
    setSpeed(floor(255 * fabs(speed)));
}

