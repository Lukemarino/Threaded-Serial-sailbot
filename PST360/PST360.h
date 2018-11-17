#ifndef PST360_H
#define PST360_H
#define PST360_VERSION ("14.0.0")

#include "mbed.h"

class PST360{
  // TODO make it inherit from AnalogIn rather than just contain one?
  
 public:
  PST360(PinName ain);
  ~PST360();
  //float angle();
  //float set_calibration(float m, float b);
  //float set_transform(float th); 
  
 private:
  AnalogIn _ain; 
  float _m;
  float _b;
  float _th; 
}

#endif
