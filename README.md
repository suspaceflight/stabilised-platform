## Logger

The Logger module is for SD writing. It has two main functions:

Firstly: `logger.log(String tag, String data)`

This takes a logger tag and some data to be logged and writes a log line to the SD card.

e.g. `[00:04.54][Instruments] Location: -1.01, -0.52, 0.92`

Secondly: `logger.log(String logLine)`

This takes a whole log line and writes it to the SD card.

Logger writes "FLIGHTX.LOG" files to the "logs" folder on the SD card. The flights are numerically ordered, so the first flight would be FLIGHT1.LOG.

The Logger will usually use sdChipSelect pin 10.


## Motor

The Motor module is for controlling a motor using an [L298N motor controller](https://tronixlabs.com.au/news/tutorial-l298n-dual-motor-controller-module-2a-and-arduino).

The module uses a `Motor` object to control each motor. `Motor` is initialized with three pins: PWM, IN1, IN2. PWM controls the motor speed and should be connected to EN1 on the L298N breakout board, IN1 and IN2 control the direction and should be connected to the similarly named pins on the breakout board.

To control the speed use `motor.setFloatSpeed(float speed)`. This takes a float between -1 and 1 where -1 is full speed anticlockwise and 1 is full speed clockwise.

To stop the motor use `motor.disable()`, this stops the motor from generating torque. It should be noted that this does not cause the motor to stop spinning.

See Testing/MotorSpeedTest/ for an example of Motor module usage.
