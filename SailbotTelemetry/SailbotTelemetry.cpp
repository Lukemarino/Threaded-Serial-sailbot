#include "mbed.h"
#include "SailbotTelemetry.h"

SailbotTelemetry::SailbotTelemetry(PinName tx, PinName rx, int baud):_telemetry(tx,rx,baud){
  debug("SailbotTelemetry() constructor called\r\n");
}

SailbotTelemetry::~SailbotTelemetry(){
  debug("~SailbotTelemetry() destructor called\r\n");
}
