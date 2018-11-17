#include "mbed.h"
#include "BNO055.h"
#include "SailbotIMU.h"

SailbotIMU::SailbotIMU(PinName SDA, PinName SCL):_bno055(SDA,SCL){
  debug("SailbotIMU() constructor called\r\n");
}

SailbotIMU::~SailbotIMU(){
  debug("~SailbotIMU() destructor called\r\n");
}

