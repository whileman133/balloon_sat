// openlog_testbench.ino
//
// Written by Wes Hileman on 27 March 2015
//
// Purpose: To test the operation of a SparkFun Openlog
// data logger via SoftwareSerial.

#include <Wire.h>
#include <RTC.h>

#define BAUD 9600

//SoftwareSerial logger( RX, TX );
RTC rtc;

char t[ TIMESTAMP_LENGTH ] = "";

void setup() 
{
  Wire.begin();
  rtc.begin();
  Serial.begin( BAUD );
}

void loop() 
{ 
  rtc.timestamp( t );
  
  Serial.println( t );
  
  delay( 1000 );
}
