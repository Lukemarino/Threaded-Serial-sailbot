# Threaded-Serial-sailbot
This is the repository for the threaded serial code for sailbot (2018)
The program currently runs two threads; one for telemetry and the other for manual control. Eventually telemetry will be turned into two threads so that data like rudder position and heading are updated at a different rate than GPS position.
The left and right motor functions work but are not great. They will probably be replaced with a pwm output rather than toggling a digital output
Some of the functions and code will be consolidated and cleaned up after thanksgiving. For example, the stepper motor functions will be turned into a class and the code will make two stepper motor objects rather than having two separate sets of stepper motor functions
