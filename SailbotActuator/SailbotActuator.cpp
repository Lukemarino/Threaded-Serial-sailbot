#include "mbed.h"
#include "SailbotActuator.h"

SailbotActuator::SailbotActuator(PinName step, PinName dir, PinName i1, PinName i2, PinName nsleep, PinName ain):_driver(step,dir,i1,i2,nsleep),_sensor(ain){
  int i;
  
  debug("SailbotActuator() constructor called\r\n");
  r = 0.0;
  y = 0.0;
  for (i=0; i<SAILBOTACTUATOR_SENSOR_BUFSIZE; i++){
    _sensor_buf[i] = 0.0; 
  }
  _sensor_i = 0; 
}

SailbotActuator::~SailbotActuator(){
  debug("~SailbotActuator() destructor called\r\n");
}
