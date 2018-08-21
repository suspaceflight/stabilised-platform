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

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  
  // Gyro initialization
  gyro.init();
  gyro.enableDefault();
  
  //Accelerometer and Magnetometer initialization
  compass.init();
  compass.enableDefault();
  
  compass.m_min = (LSM303::vector<int16_t>){-  805, - 1748, - 2332};
  compass.m_max = (LSM303::vector<int16_t>){+ 4515, + 4483, + 4265};
  
  //Altimeter initialization
  altimeter.settings.commInterface = I2C_MODE;
  altimeter.settings.I2CAddress = 0x76;
  altimeter.settings.runMode = 3; // 3, Normal mode
  altimeter.settings.tStandby = 0; // 0, 0.5ms
  altimeter.settings.filter = 0; // 0, filter off
  altimeter.settings.tempOverSample = 1; //  tempOverSample  *1
  altimeter.settings.pressOverSample = 1; // pressOverSample *1
  altimeter.settings.humidOverSample = 1; // humidOverSample *1
  altimeter.begin();
  
  //SD initialisation
  Serial.println("Initializing SD card...");
  pinMode(10, OUTPUT);

  

  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

  String header = "Mag_x\tMag_y\tMag_z\tHeading\t";
  write_sd(header);
  header = "Acc_x\tAcc_y\tAcc_z\t";
  write_sd(header);
  header = "Gyro_x\tGyro_y\t_Gyro_z\t";
  write_sd(header);
  header = "Height\tTemp\tPressure\tHumidity\n";
  write_sd(header);
  Serial.println("##write");

  delay(50);
}

void loop()
{
  String dataString = "";
  compass.read();
  
  // Magnetometer
  uint8_t c_x = int(compass.m.x);
  uint8_t c_y = int(compass.m.y);
  uint8_t c_z = int(compass.m.z);
  float head = compass.heading(); 

  Serial.print("M  X: ");
  Serial.print(c_x);
  dataString += String(c_x) + "\t";
  
  Serial.print(" Y: ");
  Serial.print(c_y);
  dataString += String(c_y) + "\t";
  
  Serial.print(" Z: ");
  Serial.print(c_z);
  dataString += String(c_z) + "\t";
  
  Serial.print(" Heading: ");
  Serial.print(head);
  dataString += String(head) + "\t";
  
  //Accelerometer
  uint8_t a_x = int(compass.a.x);
  uint8_t a_y = int(compass.a.y);
  uint8_t a_z = int(compass.a.z);

  Serial.print("  \t|\t X: ");
  Serial.print(a_x);
  dataString += String(a_x) + "\t";
  
  Serial.print(" Y: ");
  Serial.print(a_y);
  dataString += String(a_y) + "\t";
  
  Serial.print(" Z: ");
  Serial.print(a_z);
  dataString += String(a_z) + "\t";
  
  //Gyro
  uint8_t g_x = int(gyro.g.x);
  uint8_t g_y = int(gyro.g.y);
  uint8_t g_z = int(gyro.g.z);

  gyro.read();
  Serial.print("  \t|\t X: ");
  Serial.print(g_x);
  dataString += String(g_x) + "\t";
  
  Serial.print(" Y: ");
  Serial.print(g_y);
  dataString += String(g_y) + "\t";
  
  Serial.print(" Z: ");
  Serial.print(g_z);
  dataString += String(g_z) + "\t";
  
  //Altimeter
  double height = altimeter.readFloatAltitudeMeters();
  double temp = altimeter.readTempC();
  double pressure = altimeter.readFloatPressure();
  double humidity = altimeter.readFloatHumidity();

  Serial.print("  \t|\tAlt m: ");
  Serial.print(height);
  dataString += String(height) + "\t";
  
  Serial.print(" Temp C: ");
  Serial.print(temp);
  dataString += String(temp) + "\t";
  
  Serial.print(" Pres Pa: ");
  Serial.print(pressure);
  dataString += String(pressure) + "\t";
  
  Serial.print(" Humidity: ");
  Serial.print(humidity);
  dataString += String(humidity) + "\n";

  Serial.println();

  //SD Logging
  
  write_sd(dataString);

  delay(50);
}

void write_sd(String input)
{
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile)
  {
    dataFile.print(input);
    dataFile.close();
    Serial.println("##write");
  }
  else
  {
    Serial.println("error opening datalog.txt");
  }
}

