#ifndef SAILBOTGPS_H
#define SAILBOTGPS_H
#define SAILBOTPS_VERSION ("14.0.0")

#include "mbed.h"
#include "MBed_Adafruit_GPS.h"

class SailbotGPS{
 public:
  SailbotGPS(PinName tx, PinName rx);
  ~SailbotGPS();

 private:
  Serial _gps;
};

#endif
