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


//GPS and IMU variables
/*
Serial * gps_Serial;
Serial pc (USBTX, USBRX); // remnant of old
BNO055 bno(p28, p27);
Timer refresh_Timer; //sets up a timer for use in loop
char c; //when read via Adafruit_GPS::read(), the class returns string stored here
const int refresh_Time = 2000; //refresh time in ms
float pitch; //a multiuse variable used for sending things over GPS
*/
