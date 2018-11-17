#include "mbed.h"
#include "Pololu2968.h"

Pololu2968::Pololu2968(PinName step, PinName dir, PinName i1, PinName i2, PinName nsleep):
  _step(step),_dir(dir),_i1(i1),_i2(i2),_nsleep(nsleep){
  debug("Pololu2968() constructor called\r\n");
}

Pololu2968::~Pololu2968(){
  debug("~Pololu2968() destructor called\r\n");
}
