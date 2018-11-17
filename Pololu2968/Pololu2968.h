#ifndef POLOLU2968_H
#define POLOLU2968_H

#define POLOLU2968_VERSION "14.0.0"
#include "mbed.h"

class Pololu2968{
 public:
  Pololu2968(PinName step, PinName dir, PinName i1, PinName i2, PinName nsleep);
  ~Pololu2968();

  void microstep(int dir);
  
 private:
  DigitalOut _step;
  DigitalOut _dir;
  DigitalOut _i1;
  DigitalOut _i2;
  DigitalOut _nsleep;
}

#endif
