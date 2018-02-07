#ifndef QUADCOPTER_PROJECT_LOGGER_H
#define QUADCOPTER_PROJECT_LOGGER_H

class Motor {
public:
    Motor(uint8_t pwmPin, uint8_t in1Pin, uint8_t in2Pin);
    
    setActiveDirection(bool clockwise);
    
    disable();
    
private:

    uint8_t pwmPin;
    uint8_t in1Pin;
    uint8_t in2Pin;

    uint8_t speed;
};

#endif //QUADCOPTER_PROJECT_LOGGER_H
