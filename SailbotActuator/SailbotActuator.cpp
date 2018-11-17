#include "mbed.h"
#include "Pololu2968.h"
#include "PST360.h"

SailbotActuator::SailbotActuator(PinName step, PinName dir, PinName i1, PinName i2, PinName nsleep, PinName ain):_driver(step,dir,i1,i2,nsleep),_sensor(ain){
  debug("SailbotActuator() constructor called\r\n");
}

SailbotActuator::~SailbotActuator(){
  debug("~SailbotActuator() destructor called\r\n");
}
