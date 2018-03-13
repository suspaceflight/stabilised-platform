//
// Created by Elijah on 13/03/2018.
//

#include "Instruments.h"

Instruments::Instruments() {}

Instruments::Instruments(bool initialize) {
    if (!initialize) return;
    Wire.begin();

    // Gyro initialization.
    gyro.init();
    gyro.enableDefault();

    // Accelerometer and Magnetometer initialization.
    compass.init();
    compass.enableDefault();

    compass.m_min = (LSM303::vector<int16_t>){-  805, - 1748, - 2332};
    compass.m_max = (LSM303::vector<int16_t>){+ 4515, + 4483, + 4265};

    // Altimeter initialization.
    altimeter.settings.commInterface = I2C_MODE;
    altimeter.settings.I2CAddress = 0x76;
    altimeter.settings.runMode = 3; // 3, Normal mode
    altimeter.settings.tStandby = 0; // 0, 0.5ms
    altimeter.settings.filter = 0; // 0, filter off
    altimeter.settings.tempOverSample = 1; //  tempOverSample  *1
    altimeter.settings.pressOverSample = 1; // pressOverSample *1
    altimeter.settings.humidOverSample = 1; // humidOverSample *1
    altimeter.begin();
}

float Instruments::getHeading() {
    compass.read();
    return compass.heading();
}

float Instruments::getAltitude() {
    // All included to make reading work.
    altimeter.readTempC();
    altimeter.readFloatPressure();
    altimeter.readFloatHumidity();
    return altimeter.readFloatAltitudeMeters();
}