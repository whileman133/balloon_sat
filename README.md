# BalloonSat

BalloonSat is a collection of C++ classes for interfacing Arduino microcontrollers to sensors and actuators that may be present on a high altitude balloon payload. This library also includes "workbench" sketches to demonstrate the classes' functionality and a program used in an actual BalloonSat implementation (Pikes Peak Community College DemoSat team Spring 2015). This repository includes:

- _MPL3115A2_: Class for communicating with a MPL3115A2 altimeter, pressure, and temperature sensor chip.
- _ML8511_: Class for processing data from a ML8511 ultraviolet light sensor.
- _ADXL335_: Class for processing data from a ADXL335 triple-axis accelerometer chip.
- _ITG3200_: Triple-axis gyro class.
- _MHC5883L_: Triple-axis magnetometer class.
- _MUX_: Class for controlling multiplexers.
- _Heater_: Class for controlling a heater as a thermostat.
- _Thermistor_: Class for working with NTC thermistors.
