#include "mbed.h"
#include "SailbotActuator.h"

SailbotActuator::SailbotActuator(PinName step, PinName dir, PinName i1, PinName i2, PinName nsleep, PinName ain):_driver(step,dir,i1,i2,nsleep),_sensor(ain){
  debug("SailbotActuator() constructor called\r\n");
  command = 0.0; 
}

SailbotActuator::~SailbotActuator(){
  debug("~SailbotActuator() destructor called\r\n");
}
