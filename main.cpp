
/*
  Sailbot Hull 14
  L Marino and D Robinson,
  with D Evangelista, P Frontera, M Kutzer, A Laun
  2018

  Threaded version using mbed OS5
  Three threads: Mast control, rudder control, Telemetry
  Serial links between nodes
  Stepper motors with Pololu 2968 drivers (MP6500 based)
  Piher PST360 angle sensors
  Adafruit Absolute GPS
  Telemetry via ???Ubiquity???
*/

#include "mbed.h"
#include "MBed_Adafruit_GPS.h"
#include "BNO055.h"
#include "Spektrum.h"
#include "unity.h"

Serial pc(USBTX, USBRX, 115200);
Spektrum rx(p13, p14); //radio controller
int i;
int j;
float now;
//***Mast universal variables***
AnalogIn   ain(p20);  //read mast encoder value
PwmOut  motor( p25 ); //mast motor
DigitalOut   dir( p24 ); //choose mast direction
DigitalOut    I(p26);   //limits current of motor driver to 1.5 amps
DigitalOut slp(p29); //when slp = 0, no power goes to motor driver
float d_ang;  //desired mast angle
float pos;  //current position of mast
float RC_O; // variable to store raw input from RC controller
//***rudder universal variables***
AnalogIn   r_ain(p18);  //read rudder encoder value
PwmOut  rudder( p22 );    //rudder motor
DigitalOut   r_dir( p21 ); //rudder direction
DigitalOut    r_I(p23);    //limits current of motor driver to 1.5 amps
DigitalOut r_slp(p30); //when slp = 0, no power goes to motor driver
float r_ang;   //desired rudder angle
float r_pos;   //current position of rudder
float RC_1;     // variable to store raw input from RC controller

//****universal variables for Telemetry****
DigitalOut heartbeat(LED1); //LED to blink for heartbeat
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
float posi(); //function that reads the encoder three times and averages the value to reduce noise
float posR();

//*********read reads yaw from IMU **************
void yaw_read()
{
    bno.get_angles();
    data = bno.euler.yaw;
    myFloat.m_float = data;
    data = myFloat.m_float;   // get the float back from the union
    pc.printf("%.1f", data);
    Thread::wait(35);

}

//********concatenate time**********  
float conc(float hr, float min, float sec ) //turns time into single value to reduce the amount of stuff sent over serial
{
    return hr+(min/60.0)+(sec/3600.0);
}
//********Initialize IMU************

void bno_init(void)     //initialization process for IMU
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
Thread mast_thread;
Thread rudder_thread;

void telemetry_callback(void);
void mast_callback(void);
void rudder_callback(void);

int main()
{
    //start all threads
    wait(10); //This holding wait is because when the Rpi boots up, It turns the mbed on and off twice before running
    telemetry_thread.start(callback(telemetry_callback));
    mast_thread.start(callback(mast_callback));
    rudder_thread.start(callback(rudder_callback));
    //could have a heart beat but I tried to reduce the number of threads fighting for time
}//end main

float posi()
{
    float p1;
    float p2;
    float p3;
    p1 = (ain-.108)/.002466;
    Thread::wait(3);
    p2 = (ain-.108)/.002466;
    Thread::wait(3);
    p3 = (ain-.108)/.002466;
    return (p1+p2+p3)/3.0;
}
float posr()
{
    float r1;
    float r2;
    float r3;
    r1 = (r_ain-.108)/.002466;
    Thread::wait(3);
    r2 = (r_ain-.108)/.002466;
    Thread::wait(3);
    r3 = (r_ain-.108)/.002466;
    return (r1+r2+r3)/3.0;
}

void telemetry_callback(void)
{
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

            if (refresh_Timer.read_ms() >= refresh_Time) {
                refresh_Timer.reset();
            
                data = conc(myGPS.hour, myGPS.minute, myGPS.seconds ); 
                myFloat.m_float = data;
                data = myFloat.m_float;   // get the float back from the union
                pc.printf("%2.0f", data);
                Thread::wait(35);
                /*  this part can send hr min second separately, commented to save thread space
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
                */
                if (myGPS.fix) {
                    //send mast angle
                    data = posi();
                    myFloat.m_float = data;
                    data = myFloat.m_float;   // get the float back from the union
                    pc.printf("%.1f", data);
                    Thread::wait(35);
                    //send rudder angle
                    data = posi();
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
        }//if ( myGPS.newNMEAreceived() )
    }//while(true)
} // telemetry_callback()

void mast_callback(void)
{

    I=0; //sets for 1.5amp output
    motor.period(.001); //sets the pulse width modulation period
    motor.pulsewidth(0); 
    slp = 0;
    r_slp = 0; //starts the current driver in off state
    Thread::wait(10);
    // loop
    while(1) { 
        if (rx.valid) { //check for a good value from RC controller
            RC_O = rx.channel[0];
        } else {
            //pc.printf(" invalid\r\n");
            slp = 0;  //ensure that stepper motor doesn't run if boat loses contact with RC controller
        }
        d_ang = (RC_O/6.77)+39.19;  //these equations convert the raw values into angles between 0-180
        pos = (ain-.108)/.002466;

        if((pos > (d_ang-3.0))&& (pos < (d_ang+3.0))) { //if motor is within 3 degrees of desired, stop motor
            motor.pulsewidth(0);
            slp = 0;
        }
        if( (pos > (d_ang+3.0))) { //turn motor
            slp = 1;
                dir = 1; //left??
                motor.pulsewidth(.0005);
                //pc.printf("pos: %.3f\n",pos);
                Thread::wait(10);
                pos = posi();
        }//if pos
        if((pos < (d_ang-3.0))) { //turn motor
            slp = 1;
                dir = 0; //right??
                motor.pulsewidth(.0005);
                //pc.printf("pos: %.3f\n",pos);
                Thread::wait(10);
                pos = posi();
        }
        ThisThread::sleep_until(121); //update rate is slow because I wanted to make more time for all of the threads
    }//while(1)
}//mast callback

void rudder_callback(void)  //this thread is nearly identical to mast thread
{

    r_I=0; //sets for 1.5amp output
    rudder.period(.001); //sets the pulse width modulation period
    rudder.pulsewidth(0); 
    slp = 0;
    r_slp = 0; //sstarts the current driver in off state
    Thread::wait(10);
    
    // loop
    while(1) {
        
        if (rx.valid) {
            RC_1 = rx.channel[1];
        } else {
            //pc.printf(" invalid\r\n");
            r_slp = 0;
        }
        //39.19
        r_ang = (RC_1/6.77)-5.0;
        r_pos = (r_ain-.108)/.002466;

        if((r_pos > (r_ang-3.0)) && (r_pos < (r_ang+3.0))) {
            rudder.pulsewidth(0);
            r_slp = 0;
        }
        if( (r_pos > (r_ang+3.0)) ) {  //&& r_pos < 235.0
            r_slp = 1;
                r_dir = 1; //left??
                rudder.pulsewidth(.0005);
                Thread::wait(10);
                r_pos = posr();
        }//if pos
        if((r_pos < (r_ang-3.0)) ) {   // && r_pos > 55.0
            r_slp = 1;
                r_dir = 0; //right??
                rudder.pulsewidth(.0005);
                Thread::wait(10);
                r_pos = posr();
        }
        ThisThread::sleep_until(121);
    }//while(1)
}//rudder callback
