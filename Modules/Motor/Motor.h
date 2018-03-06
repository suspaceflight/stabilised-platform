#ifndef QUADCOPTER_PROJECT_LOGGER_H
#define QUADCOPTER_PROJECT_LOGGER_H

#include <Arduino.h>
#include <math.h>

class Motor {
public:
    Motor();

    Motor(uint8_t pwmPin, uint8_t in1Pin, uint8_t in2Pin);
    
    /**
     * Sets the speed and direction of the motor based on a -1 to 1 value.
     * @param speed Speed of the motor. (-1 is full anticlockwise, 1 is full clockwise.
     * @return
     */
    setFloatSpeed(float speed);

    disable();
    
private:

    uint8_t pwmPin;
    uint8_t in1Pin;
    uint8_t in2Pin;

    setActiveDirection(bool clockwise);

    setSpeed(uint8_t speed);
};

#endif //QUADCOPTER_PROJECT_LOGGER_H
