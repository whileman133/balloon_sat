// RTC_test.ino
//
// Written by Wes Hileman
// MIT License.

#include <Wire.h> 
#include <RTC.h>   

RTC clock;

void setup() 
{
  // Initialize I2C communication (clock is I2C-based)
  Wire.begin();
  // Initialize real-time clock
  clock.begin();
  // Start serial communication
  Serial.begin( 9600 );
} // end setup

void loop() 
{
  char timestamp[ TIMESTAMP_LENGTH ] = ""; // TIMESTAMP_LENGTH defined in RTC.h

  // Load timestamp into buffer
  clock.timestamp( &timestamp[ 0 ] );

  Serial.println( timestamp );
} // end loop
