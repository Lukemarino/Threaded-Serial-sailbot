#ifndef SAILBOT_ACTUATOR_H
#define SAILBOT_ACTUATOR_H
#define SAILBOT_ACTUATOR_VERSION ("14.0.0")

#include "mbed.h"
#include "Pololu2968.h"
#include "PST360.h"

class SailbotActuator{
 public:
  SailbotActuator(PinName step, PinName dir, PinName i1, PinName i2, PinName nsleep, PinName ain);
  ~SailbotActuator;

  float command; 

 private:
  Pololu2968 _driver;
  PST360 _sensor;
}

#endif
