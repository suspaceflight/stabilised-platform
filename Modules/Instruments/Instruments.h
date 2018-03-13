//
// Created by Elijah on 13/03/2018.
//

#ifndef STABILISED_PLATFORM_INSTRUMENTS_H
#define STABILISED_PLATFORM_INSTRUMENTS_H

#include <Arduino.h>
#include "SparkFunBME280.h"
#include <L3G.h>
#include <LSM303.h>

#include <Wire.h>
#include <SPI.h>

class Instruments {
public:
    Instruments();

    Instruments(bool initialize);

    float getHeading();
    float getAltitude();

private:
    BME280 altimeter;
    L3G gyro;
    LSM303 compass;
};

#endif //STABILISED_PLATFORM_INSTRUMENTS_H
