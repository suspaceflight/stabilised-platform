#include "Instruments.h"
#include "Motor.h"

double lastTime;
double Input, Setpoint;
double errSum, lastErr;
double kp, ki, kd;
double pos;
double vel;
double accel;
double timestep;

Instruments instruments;
Motor motor;
uint8_t pwmPin = 10;
uint8_t in1Pin = 9;
uint8_t in2Pin = 8;

float headingTarget = 180;

void setup() {
    Serial.begin(9600);
    motor = Motor(pwmPin, in1Pin, in2Pin);

    instruments = Instruments(true);

    Input = 0;
    Setpoint = 1;

    errSum = 0;
    lastErr = 0;
    lastTime = 0;

    pos = 0;
    vel = 0;
    accel = 0;
    timestep = 0;

    SetTunings_PID(20, 5, 10);
}

void loop() {
    /*How long since we last calculated*/
    double now = micros() / 1000000.0;
    double timestep = (now - lastTime);
    float heading = instruments.getHeading();
    double Output = Compute_PID(headingTarget - heading, timestep) / 10000;
    motor.setFloatSpeed(Output);
    Serial.println("Output = " + String(Output) + ",\tHeading = " + String(heading));
}

double Compute_PID(double Input, double timeChange) {


    /*Compute all the working error variables*/
    double error = Setpoint - Input;
    errSum += (error * timeChange);
    double dErr = (error - lastErr) / timeChange;

    /*Compute PID Output*/
    double Output = kp * error + ki * errSum + kd * dErr;

    /*Remember some variables for next time*/
    lastErr = error;
    lastTime += timeChange;
    return (Output);
}

void SetTunings_PID(double Kp, double Ki, double Kd) {
    kp = Kp;
    ki = Ki;
    kd = Kd;
}
