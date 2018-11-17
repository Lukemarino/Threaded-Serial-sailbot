#include "mbed.h"
#include "SailbotTelemetry.h"

SailbotTelemetry::SailbotTelemetry(PinName tx, PinName rx):_telemetry(tx,rx){
  debug("SailbotTelemetry() constructor called\r\n");
}

SailbotTelemetry::~SailbotTelemetry(){
  debug("~SailbotTelemetry() destructor called\r\n");
}
