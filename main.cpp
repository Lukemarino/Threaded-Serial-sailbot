/*
 Sailbot Hull 14
  L Marino and D Robinson,
  with D Evangelista, P Frontera, M Kutzer, A Laun
  2018

  Threaded version using mbed OS5
  Serial links between nodes
  Stepper motors with Pololu 2968 drivers (MP6500 based)
  Piher PST360 angle sensors
  Adafruit Absolute GPS
  Telemetry via ???Ubiquity???
*/

//Thread for logging time and Gps, heading and encoder values is mostly complete
//control thread is a work in progress

#include "mbed.h"
#include "MBed_Adafruit_GPS.h"
#include "BNO055.h"
#include "Spektrum.h"
#include "unity.h"

Serial pc(USBTX, USBRX, 115200);
Spektrum rx(p13, p14);
int i;
int j;
float now;
//***Stepper Motor universal variables***
AnalogIn   ain(p20);
PwmOut  motor( p25 );
DigitalOut   dir( p24 );
DigitalOut    I(p26);
DigitalOut slp(p29); //sleep
float d_ang = 180.0;
float pos = 180.0;
float RC_O;

//universal variables for Telemetry
DigitalOut heartbeat(LED1);
Serial * gps_Serial;
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

//**get position**
float posi(float in)
{
    float p1;
    float p2;
    float p3;
    p1 = (ain-.108)/.002466;
    p2 = (ain-.108)/.002466;
    p3 = (ain-.108)/.002466;
    return (p1+p2+p3)/3.0;
}

//*********read reads yaw from IMU **************
void yaw_read()
{
    bno.get_angles();
    data = bno.euler.yaw;
    myFloat.m_float = data;
    data = myFloat.m_float;   // get the float back from the union
    pc.printf("%.1f", data);
    Thread::wait(2);

}

//********concatenate time (not currently in use)**********
float conc(float hr, float min, float sec )
{
    return hr+(min/60.0)+(sec/3600.0);
}
//********Initialize IMU************

void bno_init(void)
{
    if(bno.check()) {
        //pc.printf("BNO055 connected\r\n");
        bno.reset();
        Thread::wait(30);
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


// *****threading*****
Thread telemetry_thread;
Thread control_thread;

void telemetry_callback(void);
void control_callback(void);

int main()
{

    telemetry_thread.start(callback(telemetry_callback));
    control_thread.start(callback(control_callback));
    while(1) {

        heartbeat = !heartbeat; // blink the heartbeat
        Thread::wait(300);


    } // while(1)
}//end main

void telemetry_callback(void)
{
    //uint64_t now;

    // setup
    //debug("telemetry_thread started\r\n");

    pc.baud(115200); //sets virtual COM serial communication to high rate; this is to allow more time to be spent on GPS retrieval


    gps_Serial = new Serial(p9,p10); //serial object for use w/ GPS
    Adafruit_GPS myGPS(gps_Serial); //object of Adafruit's GPS class


    myGPS.begin(9600);  //sets baud rate for GPS communication; note this may be changed via Adafruit_GPS::sendCommand(char *)
    //a list of GPS commands is available at http://www.adafruit.com/datasheets/PMTK_A08.pdf

    myGPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //these commands are defined in MBed_Adafruit_GPS.h; a link is provided there for command creation
    myGPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    myGPS.sendCommand(PGCMD_ANTENNA);

    //pc.printf("Connection established at 115200 baud...\n");
    bno_init();

    Thread::wait(1000);
    refresh_Timer.start();  //starts the clock on the timer

    while(true) {

        c = myGPS.read();   //queries the GPS
        bno.get_angles();
        //check if we recieved a new message from GPS, if so, attempt to parse it,
        if ( myGPS.newNMEAreceived() ) {
            if ( !myGPS.parse(myGPS.lastNMEA()) ) {
                continue;
            }//if ( !myGPS.parse..

            if (refresh_Timer.read_ms() >= refresh_Time) {
                refresh_Timer.reset();

                //send hr
                data = myGPS.hour;               // conc(myGPS.hour, myGPS.minute, myGPS.seconds );
                myFloat.m_float = data;
                data = myFloat.m_float;   // get the float back from the union
                pc.printf("%2.0f", data);
                Thread::wait(35);
                //send min
                data = myGPS.minute;               // conc(myGPS.hour, myGPS.minute, myGPS.seconds );
                myFloat.m_float = data;
                data = myFloat.m_float;   // get the float back from the union
                pc.printf("%2.0f", data);
                Thread::wait(35);
                //send sec
                data = myGPS.seconds;               // conc(myGPS.hour, myGPS.minute, myGPS.seconds );
                myFloat.m_float = data;
                data = myFloat.m_float;   // get the float back from the union
                pc.printf("%2.0f", data);
                Thread::wait(35);

                if (myGPS.fix) {
                    //rudder and mast position will log here
                    //send encoder angle
                    data = posi(ain);
                    myFloat.m_float = data;
                    data = myFloat.m_float;   // get the float back from the union
                    pc.printf("%.1f", data);
                    Thread::wait(35);
                    //send latitude
                    data = myGPS.latitude;
                    myFloat.m_float = data;
                    data = myFloat.m_float;   // get the float back from the union
                    pc.printf("%.2f", data);
                    Thread::wait(35);
                    //send logitude
                    data = myGPS.longitude;
                    myFloat.m_float = data;
                    data = myFloat.m_float;   // get the float back from the union
                    pc.printf("%.2f", data);
                    Thread::wait(35);
                    //send yaw
                    yaw_read();
                    //send transmittion
                    pc.printf("q");
                    Thread::wait(3000);
                }//if (myGPS.fix)
            }//if (refresh_Timer..

            //pc.printf("Location: %5.2f%c, %5.2f%c\n", myGPS.latitude, myGPS.lat, myGPS.longitude, myGPS.lon);
            //pc.printf("Speed: %5.2f knots\n", myGPS.speed,myGPS.angle, myGPS.altitude, myGPS.satellites);
            //pc.printf("yaw: %5.2f\n", bno.euler.yaw);
        }//if ( myGPS.newNMEAreceived() )
    }//while(true)

} // telemetry_callback()

void control_callback(void)
{
    //pc.printf("Spektrum library version ");
    //pc.printf(SPEKTRUM_VERSION);
    I=0;
    motor.period(.001);
    motor.pulsewidth(0);
    // loop
    while(1) {
        //now = rtos::Kernel::get_ms_count();
        //***JUST ADDED stepper code

        if (rx.valid) {
            RC_O = rx.channel[0];
            //pc.printf(" rc: %f\n",rx.channel[0]);
        } else {
            //pc.printf(" invalid\r\n");
            slp = 0;
        }

        d_ang = (RC_O/6.77)+39.19;

        pos = (ain-.108)/.002466;
        //pc.printf(" %.3f\n",);
        if((pos > (d_ang-3.0)) && (pos < (d_ang+3.0))) {
            motor.pulsewidth(0);
            slp = 0;
        }
        if( (pos > (d_ang+3.0)) && pos < 270.0) {
            slp = 1;
            while(pos > d_ang) {
                dir = 1; //left??
                motor.pulsewidth(.0005);
                //pc.printf("pos: %.3f\n",pos);
                pos = posi(ain);
            }//while pos
            motor.pulsewidth(0);
        }//if pos
        if((pos < (d_ang-3.0)) && pos > 90.0) {
            slp = 1;
            while(pos < d_ang) {
                dir = 0; //right??
                motor.pulsewidth(.0005);
                //pc.printf("pos: %.3f\n",pos);
                pos = posi(ain);
            }//while pos
            motor.pulsewidth(0);
        }
        //***END OF JUST ADDED
        ThisThread::sleep_until(121);
    }//while(1)
}//control callback
