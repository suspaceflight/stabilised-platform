## Logger

The Logger module is for SD writing. It has two main functions:

Firstly: `logger.log(String tag, String data)`

This takes a logger tag and some data to be logged and writes a log line to the SD card.

e.g. `[00:04.54][Instruments] Location: -1.01, -0.52, 0.92`

Secondly: `logger.log(String logLine)`

This takes a whole log line and writes it to the SD card.

Logger writes "FLIGHTX.LOG" files to the "logs" folder on the SD card. The flights are numerically ordered, so the first flight would be FLIGHT1.LOG.

The Logger will usually use sdChipSelect pin 10.