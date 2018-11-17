#ifndef SAILBOTIMU_H
#define SAILBOTIMU_H
#define SAILBOTIMU_VERSION ("14.0.0")

#include "mbed.h"
#include "BNO055.h"

// TODO change it to inherit from BNO055 rather than contain? 
class SailbotIMU{
 public:
  SailbotIMU(PinName SDA, PinName SCL);
  ~SailbotIMU();
  void get_angles(void); 
  float get_yaw(void);
  
 private:
  BNO055 _bno055;
};

#endif
