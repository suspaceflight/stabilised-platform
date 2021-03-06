//
// Created by Elijah on 14/02/2017.
//

#ifndef QUADCOPTER_PROJECT_LOGGER_H
#define QUADCOPTER_PROJECT_LOGGER_H

#include <Arduino.h>
#include <SD.h>

/**
 * Allows for easy logging of information to the SD card and to the Serial monitor using log().
 *
 * The Logger writes "FLIGHTX.QFL" (Quadcopter Flight Log) files to the logDir folder on the SD card.
 *
 * The flights are numerically ordered, so the first flight would be FLIGHT1.QFL.
 *
 * \see Logger::log() for logging data.
 *
 * @author Elijah Andrews
 */
class Logger {
public:
    Logger(int sdChipSelect);

    Logger();

    void log(String tag, String data);

    void log(String logLine);

    void checkFlush(uint16_t logLineLength);

    void checkFlush();

    String parseMillis(uint32_t millis);

private:

    File logFile;

    uint16_t bytesWritten;

    uint32_t lastFlushed;

    String getNextName();

};

#endif //QUADCOPTER_PROJECT_LOGGER_H
