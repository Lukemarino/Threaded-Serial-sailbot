//Thread for logging time and Gps, heading and eventually encoder values

#include "mbed.h"
#include "BNO055.h"
#include "MBed_Adafruit_GPS.h"

//#include "rtos.h" //rtos library has been causing issues

Serial * gps_Serial;
Serial pc (USBTX, USBRX);
BNO055 bno(p28, p27);
char c; //when read via Adafruit_GPS::read(), the class returns single character stored here
float info; //a multiuse variable used for sending things over GPS
Timer refresh_Timer; //sets up a timer for use in loop; how often do we print GPS info?
const int refresh_Time = 2000; //refresh time in ms

union Float {       //slightly mysterious data type that we use to send data over serial
    float    m_float;
    uint8_t  m_bytes[sizeof(float)];
};
float        data;
uint8_t      bytes[sizeof(float)];
Float        myFloat;

//*********read reads yaw from IMU **************
void yaw_read()
{
    bno.get_angles();
    data = bno.euler.yaw;
    myFloat.m_float = data;
    data = myFloat.m_float;   // get the float back from the union
    pc.printf("%.4f", data);
    wait(.02);

}

//********concatenate time**********
float conc(float hr, float min, float sec )
{
    return hr+(min/60.0)+(sec/3600.0);
}
//********Initialize IMU************

void bno_init(void)
{
    if(bno.check()) {
        pc.printf("BNO055 connected\r\n");
        bno.reset();
        wait(.5);
        bno.setmode(OPERATION_MODE_CONFIG);
        bno.SetExternalCrystal(1);
        //bno.set_orientation(1);
        bno.setmode(OPERATION_MODE_NDOF);  //Uses magnetometer
        //bno.setmode(OPERATION_MODE_NDOF_FMC_OFF);   //no magnetometer
        bno.set_angle_units(RADIANS);
    } else {
        pc.printf("BNO055 NOT connected\r\n Program Trap.");
        while(1);
    }
}



int main()
{

    pc.baud(115200); //sets virtual COM serial communication to high rate; this is to allow more time to be spent on GPS retrieval


    gps_Serial = new Serial(p9,p10); //serial object for use w/ GPS
    Adafruit_GPS myGPS(gps_Serial); //object of Adafruit's GPS class


    myGPS.begin(9600);  //sets baud rate for GPS communication; note this may be changed via Adafruit_GPS::sendCommand(char *)
    //a list of GPS commands is available at http://www.adafruit.com/datasheets/PMTK_A08.pdf

    myGPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //these commands are defined in MBed_Adafruit_GPS.h; a link is provided there for command creation
    myGPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    myGPS.sendCommand(PGCMD_ANTENNA);

    pc.printf("Connection established at 115200 baud...\n");
    bno_init();

    wait(1);
    refresh_Timer.start();  //starts the clock on the timer

    while(true) {

        c = myGPS.read();   //queries the GPS
        bno.get_angles();
        //check if we recieved a new message from GPS, if so, attempt to parse it,
        if ( myGPS.newNMEAreceived() ) {
            if ( !myGPS.parse(myGPS.lastNMEA()) ) {
                continue;
            }

            if (refresh_Timer.read_ms() >= refresh_Time) {
                refresh_Timer.reset();

                //send time
                data = conc(myGPS.hour, myGPS.minute, myGPS.seconds ) ;               // conc(myGPS.hour, myGPS.minute, myGPS.seconds );
                myFloat.m_float = data;
                data = myFloat.m_float;   // get the float back from the union
                pc.printf("%.5f", data);
                wait(.04);

                if (myGPS.fix) {
                    //rudder and mast position will log here
                    //not yet included because we have been testing GPS without powering the motors and encoders

                    //send latitude
                    data = myGPS.latitude;
                    myFloat.m_float = data;
                    data = myFloat.m_float;   // get the float back from the union
                    pc.printf("%.2f", data);
                    wait(.03);
                    //send logitude
                    data = myGPS.longitude;
                    myFloat.m_float = data;
                    data = myFloat.m_float;   // get the float back from the union
                    pc.printf("%.2f", data);
                    wait(.03);
                    //send yaw
                    yaw_read();
                    //send transmittion
                    pc.printf("q");
                    wait(2);
                }
            }
            //pc.printf("Location: %5.2f%c, %5.2f%c\n", myGPS.latitude, myGPS.lat, myGPS.longitude, myGPS.lon);
            //pc.printf("Speed: %5.2f knots\n", myGPS.speed,myGPS.angle, myGPS.altitude, myGPS.satellites);
            //pc.printf("yaw: %5.2f\n", bno.euler.yaw);
        }
    }}
