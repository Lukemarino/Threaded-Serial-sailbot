#ifndef SAILBOTTELEMETRY_H
#define SAILBOTTELEMETRY_H
#define SAILBOTTELEMETRY_VERSION ("14.0.0")

#include "mbed.h"

class SailbotTelemetry(){
 public:
  SailbotTelemetry(PinName tx, PinName rx);
  ~SailbotTelemetry();

 private:
  Serial _telemetry;

};

#endif


/*
Serial pc (USBTX, USBRX); // remnant of old
BNO055 bno(p28, p27);
Timer refresh_Timer; //sets up a timer for use in loop
char c; //when read via Adafruit_GPS::read(), the class returns string stored here
const int refresh_Time = 2000; //refresh time in ms
float pitch; //a multiuse variable used for sending things over GPS

union Float {       //slightly mysterious data type that we use to send data over serial
    float    m_float;
    uint8_t  m_bytes[sizeof(float)];
};
float        data;
uint8_t      bytes[sizeof(float)];
Float        myFloat;
*/
