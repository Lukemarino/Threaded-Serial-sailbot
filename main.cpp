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
#include "mbed.h"
#include "rtos.h"
#include "Spektrum.h"
#include "SailbotActuator.h"

// global definitions here
DigitalOut heartbeat(LED1);  // heartbeat indication in main thread
Thread manual_control_thread; 
Thread telemetry_thread;
void manual_control_callback(void);
void telemetry_callback(void); 

Spektrum rx(p13,p14); 
SailbotActuator mainsail(p25,p24,p26,NC,NC,p18);
SailbotActuator rudder(p22,p21,p23,NC,NC,p20);


int main(void){
  // setup
  // start up the boat

  // loop
  while(1){
    heartbeat = ~heartbeat; // blink the heartbeat
    ThisThread::sleep_for(1000);
    // let the control thread do its job...
    // let the telemetry thread do its job... 
  } // while(1)

  //return 0; 
}// main()






void manual_control_callback(void){
  // setup

  // loop
  // every X approximately
  // check the Spektrum radio commands
  // set the rudder and mainsail. 
}





void telemetry_callback(void){
  // setup

  // loop
  // every X
  // assemble and send telemetry information
}
