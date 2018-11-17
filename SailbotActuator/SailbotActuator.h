#ifndef SAILBOT_ACTUATOR_H
#define SAILBOT_ACTUATOR_H
#define SAILBOT_ACTUATOR_VERSION ("14.0.0")

#include "mbed.h"
#include "Pololu2968.h"
#include "PST360.h"

class SailbotActuator{
 public:
  SailbotActuator(PinName step, PinName dir, PinName i1, PinName i2, PinName nsleep, PinName ain);
  ~SailbotActuator();

  float command; 

 private:
  Pololu2968 _driver;
  PST360 _sensor;
};

#endif

/*
// initialize variables for stepper motor rudder functions
float r_ang; //desired angle - user imput or nav software's specified angle
DigitalOut   r_motor( p25 ); // motor for rudder
DigitalOut   r_dir( p24 ); // specifies which direction stepper turns
DigitalOut    r_I(p26); //makes motor driver limit current to 1.5A, set to 0 and leave it
float p1; //p1-3 save the encoder position for an average
float p2; // to reduce noise
float p3;
float pos; //another variable for position
AnalogIn   r_ain(p18); //ain is raw encoder data

// initialize variables for stepper mast
float m_ang; //desired angle - user imput or nav software's specified angle
DigitalOut   m_motor( p22 ); // motor for rudder
DigitalOut   m_dir( p21 ); // specifies which direction stepper turns
DigitalOut    m_I(p23); //makes motor driver limit current to 1.5A, set to 0 and leave it
float m1; //m1-3 save the encoder position for an average
float m2; // to reduce noise
float m3;
float m_pos; //another variable for position
AnalogIn   m_ain(p20); //ain is raw encoder data
*/
