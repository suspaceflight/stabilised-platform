#include "SparkFunBME280.h"
#include <L3G.h>
#include <LSM303.h>

#include <Wire.h>
#include <SPI.h>

BME280 altimeter;
L3G gyro;
LSM303 compass;

void setup() {
  Serial.begin(9600);
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

void loop() {  
  compass.read();
  
  // Magnetometer
  /*Serial.print("M ");
  Serial.print(" X: ");
  Serial.print((int)compass.m.x);
  Serial.print(" Y: ");
  Serial.print((int)compass.m.y);
  Serial.print(" Z: ");
  Serial.println((int)compass.m.z);
  */
  float heading = compass.heading();
  Serial.print("Heading: ");
  Serial.println(heading);
  /*
  // Accelerometer
  Serial.print("A ");
  Serial.print(" X: ");
  Serial.print((int)compass.a.x);
  Serial.print(" Y: ");
  Serial.print((int)compass.a.y);
  Serial.print(" Z: ");
  Serial.println((int)compass.a.z);
  
  // Gyro
  gyro.read();
  Serial.print("G ");
  Serial.print(" X: ");
  Serial.print((int)gyro.g.x);
  Serial.print(" Y: ");
  Serial.print((int)gyro.g.y);
  Serial.print(" Z: ");
  Serial.println((int)gyro.g.z);
  
  // Altimeter
  Serial.print("Altitude /m: ");
  Serial.println(altimeter.readFloatAltitudeMeters(), 2);
  Serial.print("Temperature /degrees C: ");
  Serial.println(altimeter.readTempC(), 2);
  Serial.print("Pressure /Pa: ");
  Serial.println(altimeter.readFloatPressure(), 2);
  Serial.print("%RH: ");
  Serial.println(altimeter.readFloatHumidity(), 2);
  */
  delay(1000);
}
