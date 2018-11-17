#include "mbed.h"
#include "rtos.h"

// global definitions here
DigitalOut heartbeat(LED1);  // heartbeat indication in main thread
Thread manual_control_thread; 
Thread telemetry_thread;
void manual_control_callback(void);
void telemetry_callback(void); 





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

  return 0; 
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
