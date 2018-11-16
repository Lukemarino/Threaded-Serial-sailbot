# Threaded-Serial-sailbot
This is the repository for the threaded serial code for sailbot (2018)
The code has two main issues:
1. the compiler registers an error on "Thread thread;" and says Error: No default constructor exists for class "rtos::Thread" 
2. When thread is commented out, the program will compile but the mbed will output the error: Mbed OS error : pinmap not found for peripheral
We have double checked our pin asignments for the GPS and imu 
