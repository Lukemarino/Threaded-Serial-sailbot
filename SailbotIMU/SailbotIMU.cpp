#include "mbed.h"
#include "BNO055.h"
#include "SailbotIMU.h"

SailbotIMU::SailbotIMU(PinName SDA, PinName SCL):_bno055(SDA,SCL){
  debug("SailbotIMU() constructor called\r\n");
}

SailbotIMU::~SailbotIMU(){
  debug("~SailbotIMU() destructor called\r\n");
}




//*********IMU initialization**************
/*void bno_init(void)
{
    if(bno.check()) {
        //pc.printf("BNO055 connected\r\n");
        bno.reset();
        Thread::wait(50);
        bno.setmode(OPERATION_MODE_CONFIG);
        bno.SetExternalCrystal(1);
        //bno.set_orientation(1);
        bno.setmode(OPERATION_MODE_NDOF);  //Uses magnetometer
        //bno.setmode(OPERATION_MODE_NDOF_FMC_OFF);   //no magnetometer
        bno.set_angle_units(RADIANS);
    } else {
        //pc.printf("BNO055 NOT connected\r\n Program Trap.");
        while(1);
    }
}
*/

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
