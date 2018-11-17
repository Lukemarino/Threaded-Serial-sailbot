#include "mbed.h"
#include "BNO055.h"
#include "SailbotIMU.h"



/** Constructor for Sailbot IMU, BNO055
 \param sda I2C SDA pin for BNO055, normally p28
 \param scl I2C SCL pin for BNO055, normally p27
 \returns a SailbotIMU object
 */
SailbotIMU::SailbotIMU(PinName sda, PinName scl):_bno055(sda,scl){
  debug("SailbotIMU() constructor called\r\n");

  // IMU initialization from Marino's code
  if(_bno055.check()) {
    debug("SailbotIMU() BNO055 connected\r\n");
    _bno055.reset();
    ThisThread::sleep_for(50);
    
    _bno055.setmode(OPERATION_MODE_CONFIG);
    _bno055.SetExternalCrystal(1);
    //_bno055.set_orientation(1);
    _bno055.setmode(OPERATION_MODE_NDOF);  //Uses magnetometer
    //_bno055.setmode(OPERATION_MODE_NDOF_FMC_OFF);   //no magnetometer
    _bno055.set_angle_units(RADIANS);
  }
  else {
    MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION,MBED_ERROR_CODE_NO_RESPONSE),"Failed to connect to BNO055\r\n");
  }
} // SailbotIMU() constructor



/** Destructor for a SailbotIMU object */
SailbotIMU::~SailbotIMU(){
  debug("~SailbotIMU() destructor called\r\n");
} // ~SailbotIMU() destructor

/** Calls _bno055.get_angles() to update internal angle estimates */ 
void SailbotIMU::get_angles(void){
  _bno055.get_angles();
}

/** Returns _bno055.euler.yaw angle */ 
float SailbotIMU::get_yaw(void){
  return _bno055.euler.yaw;
}


//*********read reads yaw from IMU **************
/*void yaw_read()
{
    bno.get_angles();
    float pitch = bno.euler.yaw;
    myFloat.m_float = pitch;
    data = myFloat.m_float;   // get the float back from the union
    pc.printf("%.4f", pitch);
    Thread::wait(10);

}
*/
