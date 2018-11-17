#include "mbed.h"
#include "SailbotGPS.h"

SailbotGPS::SailbotGPS(PinName tx, PinName rx):_gps(tx,rx){
  debug("SailbotGPS() constructor called\r\n");
} // SailbotGPS constructor

SailbotGPS::~SailbotGPS(){
  debug("~SailbotGPS() destructor called\r\n");
} // ~SailbotGPS destructor
