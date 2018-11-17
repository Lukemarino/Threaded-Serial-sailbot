#include "mbed.h"
#include "PST360.h"

PST360::PST360(PinName ain):_ain(ain){
  debug("PST360() constructor called.");
  _m = 0.0;
  _b = 0.0;
  _th = 0.0;
}

PST360::~PST360(){
  debug("~PST360() destructor called.");
}
