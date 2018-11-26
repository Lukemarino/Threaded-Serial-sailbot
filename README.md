# Threaded-Serial-sailbot
### L Marino and D Robinson, with D Evangelista, A Laun, M Kutzer and P Frontera

This is the repository for the threaded serial code for sailbot (2018) and is intended for use in Hull 14 with ship control system based on SY202 boards and RS232 serial interfacing between boards. This variant is intended for launch in Nov 2018 - Jan 2019 timeframe. 

The code has two main issues:
1. the compiler registers an error on "Thread thread;" and says Error: No default constructor exists for class "rtos::Thread" 
2. When thread is commented out, the program will compile but the mbed will output the error: Mbed OS error : pinmap not found for peripheral
We have double checked our pin asignments for the GPS and imu 

The left and right motor functions work but are not great. They will probably be replaced with a pwm output rather than toggling a digital output
Some of the functions and code will be consolidated and cleaned up after thanksgiving. For example, the stepper motor functions will be turned into a class and the code will make two stepper motor objects rather than having two separate sets of stepper motor functions

## ``main.cpp`` main code
``main.cpp`` contains a threaded version of the code using mbed OS5, RS232 serial links between nodes, stepper motors with Pololu 2968 drivers (MP6500 based), Piher PST360 angle sensors, Adafruit Absolute GPS, and telemetry via Ubiquiti wireless access points. The code is currently under construction, and currently contains a heartbeat thread, a thread for accomplishing manual r/c control via a Spektrum receiver, and a thread for sending back telemetry. 

## ``TESTS`` directory 
Subsystem and integrated system tests are included in the TESTS directory as well as in relevant libraries. 

## Revision control using git
```bash
git clone https://github.com/Lukemarino/Threaded-Serial-sailbot.git
git pull
make changes
git commit -m "I made some changes"
git push
```

## Contributors
MIDN 2/c Luke Marino and Drew Robinson created the code. Asst Prof D Evangelista and CDR P Frontera have assisted. 

## Acknowledgements
Fill in
