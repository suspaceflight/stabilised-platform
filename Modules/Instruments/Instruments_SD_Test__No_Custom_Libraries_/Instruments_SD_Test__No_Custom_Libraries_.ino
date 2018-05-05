#include "SparkFunBME280.h" 
#include <L3G.h> 
#include <LSM303.h> 
#include <SD.h>

#include <Wire.h> 
#include <SPI.h> 
 
BME280 altimeter; 
L3G gyro; 
LSM303 compass; 

const int chipSelect = 10;

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
  
  // SD initialisation
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
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
  Serial.print(heading); 
  
  // Accelerometer 
  Serial.print("  \t|\t"); 
  Serial.print(" X: "); 
  Serial.print((int)compass.a.x); 
  Serial.print(" Y: "); 
  Serial.print((int)compass.a.y); 
  Serial.print(" Z: "); 
  Serial.print((int)compass.a.z); 
   
  // Gyro 
  gyro.read(); 
  Serial.print("  \t|\t"); 
  Serial.print(" X: "); 
  Serial.print((int)gyro.g.x); 
  Serial.print(" Y: "); 
  Serial.print((int)gyro.g.y); 
  Serial.print(" Z: "); 
  Serial.print((int)gyro.g.z); 
   
  // Altimeter
  Serial.print("  \t|\tAlt m: "); 
  Serial.print(altimeter.readFloatAltitudeMeters(), 2); 
  Serial.print(" Temp C: "); 
  Serial.print(altimeter.readTempC(), 2); 
  Serial.print(" Pres Pa: "); 
  Serial.print(altimeter.readFloatPressure(), 2); 
  /*Serial.print("%RH: "); 
  Serial.print(altimeter.readFloatHumidity(), 2);*/
  
  Serial.println();
  
  // SD Logging
  String dataString = "";

  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ","; 
    }
  }

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  else {
    Serial.println("error opening datalog.txt");
  }

  delay(50); 
} 

