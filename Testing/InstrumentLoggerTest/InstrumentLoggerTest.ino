#include "Instruments.h"
#include "Logger.h"

Instruments instruments;
Logger logger;
const uint8_t sdChipSelect = 10;

void setup() {
    Serial.begin(9600);
    instruments = Instruments(true);
    logger = Logger(sdChipSelect);
}

void loop() {
    float heading = instruments.getHeading();
    String logLine = "Heading = " + String(heading);
    Serial.println(logLine);
    logger.log(F("Instruments"), logLine);
}