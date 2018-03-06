double lastTime;
double Input, Setpoint;
double errSum, lastErr;
double kp, ki, kd;
double pos;
double vel;
double accel;
double timestep;

void setup()
{
  Input = 0;
  Setpoint = 1;

  errSum = 0;
  lastErr = 0;
  lastTime = 0;

  pos = 0;
  vel = 0;
  accel = 0;
  timestep = 0;

  SetTunings_PID(30, 10, 10);
  Serial.begin(4800);
}

void loop()
{
  /*How long since we last calculated*/
  double now = micros() / 1000000.0;
  double timestep = (now - lastTime);

  // System
  accel = Compute_PID(pos, timestep);
  /*if (accel > 0.1) {
    accel = 0.1;
  }*/
  vel += accel * timestep;
  pos += vel * timestep;
  Serial.println(pos);
}

double Compute_PID(double Input, double timeChange)
{


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

void SetTunings_PID(double Kp, double Ki, double Kd)
{
  kp = Kp;
  ki = Ki;
  kd = Kd;
}
