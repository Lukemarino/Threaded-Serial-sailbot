#include "mbed.h"
#include "SailbotGPS.h"

SailbotGPS::SailbotGPS(PinName tx, PinName rx):
  _gps(tx,rx),
  _adafruit(&_gps){
  
  debug("SailbotGPS() constructor called\r\n");

  _adafruit.begin(9600); //sets baud rate for GPS communication; note this may be changed via Adafruit_GPS::sendCommand(char *)
  //a list of GPS commands is available at http://www.adafruit.com/datasheets/PMTK_A08.pdf
  _adafruit.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //these commands are defined in MBed_Adafruit_GPS.h; a link is provided there for command creation
  _adafruit.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  _adafruit.sendCommand(PGCMD_ANTENNA);
} // SailbotGPS constructor

SailbotGPS::~SailbotGPS(){
  debug("~SailbotGPS() destructor called\r\n");
} // ~SailbotGPS destructor




/*
   gps_Serial = new Serial(p9,p10); //serial object for use w/ GPS
    Adafruit_GPS myGPS(gps_Serial); //object of Adafruit's GPS class


    myGPS.begin(9600);  //sets baud rate for GPS communication; note this may be changed via Adafruit_GPS::sendCommand(char *)
    //a list of GPS commands is available at http://www.adafruit.com/datasheets/PMTK_A08.pdf

    myGPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //these commands are defined in MBed_Adafruit_GPS.h; a link is provided there for command creation
    myGPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    myGPS.sendCommand(PGCMD_ANTENNA);
*/
