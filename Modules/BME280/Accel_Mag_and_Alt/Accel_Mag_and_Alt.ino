#include "SparkFunBME280.h"
#include <L3G.h>
#include <LSM303.h>

#include "stdint.h"
#include <math.h>

#include "Wire.h"
#include "SPI.h"

//Sensor Definitions
BME280 altimeter;
L3G gyro;
LSM303 compass;

//Mag calibration script
//LSM303::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32768, -32768, -32768};

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  //Accel Mag setup (L3G and LSM303)
  if (!gyro.init()) {
    Serial.println("Failed to autodetect gyro type!");
    while(1);
  }
  if (!compass.init()) {
    Serial.println("Failed to autodetect compass!");
    while(1);
   
  gyro.enableDefault();
  compass.enableDefault();
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  compass.m_min = (LSM303::vector<int16_t>){-  805, - 1748, - 2332};
  compass.m_max = (LSM303::vector<int16_t>){+ 4515, + 4483, + 4265};
  }
  
  //Altimeter
  altimeter.settings.commInterface = I2C_MODE;
  altimeter.settings.I2CAddress = 0x76;
  altimeter.settings.runMode = 3; // 3, Normal mode
  altimeter.settings.tStandby = 0; // 0, 0.5ms
  altimeter.settings.filter = 0; // 0, filter off
  altimeter.settings.tempOverSample = 1; //  tempOverSample  *1
  altimeter.settings.pressOverSample = 1; // pressOverSample *1
  altimeter.settings.humidOverSample = 1; // humidOverSample *1
  Serial.print("Altimeter: 0x");
  Serial.println(altimeter.begin(), HEX);
}

void loop()
{
  //Gyro
  gyro.read();
  Serial.print("G ");
  Serial.print(" X: ");
  Serial.print((int)gyro.g.x);
  Serial.print(" Y: ");
  Serial.print((int)gyro.g.y);
  Serial.print(" Z: ");
  Serial.println((int)gyro.g.z);
  
  /*Compass*/
  compass.read();
  
//  Calibration script
//  running_min.x = min(running_min.x, compass.m.x);
//  running_min.y = min(running_min.y, compass.m.y);
//  running_min.z = min(running_min.z, compass.m.z);
//
//  running_max.x = max(running_max.x, compass.m.x);
//  running_max.y = max(running_max.y, compass.m.y);
//  running_max.z = max(running_max.z, compass.m.z);
  
  Serial.print("A ");
  Serial.print(" X: ");
  Serial.print((int)compass.a.x);
  Serial.print(" Y: ");
  Serial.print((int)compass.a.y);
  Serial.print(" Z: ");
  Serial.println((int)compass.a.z);
  
  /*
  When given no arguments, the heading() function returns the angular
  difference in the horizontal plane between a default vector and
  north, in degrees.
  
  The default vector is chosen by the library to point along the
  surface of the PCB, in the direction of the top of the text on the
  silkscreen. This is the +X axis on the Pololu LSM303D carrier and
  the -Y axis on the Pololu LSM303DLHC, LSM303DLM, and LSM303DLH
  carriers.
  
  To use a different vector as a reference, use the version of heading()
  that takes a vector argument; for example, use
  
    compass.heading((LSM303::vector<int>){0, 0, 1});
  
  to use the +Z axis as a reference.
  */
  float heading = compass.heading();
  Serial.print("Heading: ");
  Serial.println(heading);
  
  /*Magnetometer*/
  compass.read();
  Serial.print("M ");
  Serial.print(" X: ");
  Serial.print((int)compass.m.x);
  Serial.print(" Y: ");
  Serial.print((int)compass.m.y);
  Serial.print(" Z: ");
  Serial.println((int)compass.m.z);
  
  /*Altimeter and Temp*/
  Serial.print("Altitude /m: ");
  Serial.println(altimeter.readFloatAltitudeMeters(), 2);
  Serial.print("Temperature /degrees C: ");
  Serial.println(altimeter.readTempC(), 2);
  Serial.print("Pressure /Pa: ");
  Serial.println(altimeter.readFloatPressure(), 2);
  Serial.print("%RH: ");
  Serial.println(altimeter.readFloatHumidity(), 2);
  
  Serial.println();
  
  delay(1000);
}
