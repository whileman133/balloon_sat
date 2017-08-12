// sensor_testbench.ino
//
// Written by Wes Hileman on 3 April 2015
//
// Purpose: To test all of the Apogee payload's sensors.

#include <MUX.h>
#include <Wire.h>
#include <MPL3115A2.h>
#include <ML8511.h>
#include <RTC.h>
#include <HMC5883L.h>
#include <ADXL335.h>
#include <SendOnlySoftwareSerial.h>


// Altimeter
MPL3115A2 altimeter;
// Uv sensor
ML8511 uvSensor(A5);
RTC clock;
HMC5883L compass( AV8, F3HZ, G1370, CONTINUOUS );

int pins[ 4 ] = { A1, A2, A3, A4 };
MUX mux( pins, 4 );
//             aref M13 M14 M15
ADXL335 accel( 3.3, 13, 14, 15, &mux, A0 );
SendOnlySoftwareSerial openlog( 12 );

void setup()
{
  
  Wire.begin();
  Serial.begin(9600);
  
  uvSensor.begin();
  altimeter.begin( 7 );
  altimeter.barometer();
  clock.begin();
  compass.begin();
  accel.begin();
  openlog.begin( 9600 );
  
  openlog.println( "Time (ms), Timestamp, Pressure (Pa), UV (V), Theta, Phi, Mag (gauss), X ACCEL (g), Y ACCEL (g), Z ACCEL (g)" );
  
}

void loop()
{
  
  float psr = altimeter.pressure();
  float v = uvSensor.voltage();
  char buffer[ TIMESTAMP_LENGTH ] = "";
  float theta, phi, mag;
  
  clock.timestamp( &buffer[ 0 ] );
  compass.data( &theta, &phi, &mag );
  
  openlog.print( millis() );
  openlog.print(", ");
  openlog.print( buffer );
  openlog.print(", ");
  openlog.print(psr, 2);
  openlog.print(", ");
  openlog.print(v, 5);
  openlog.print(", ");
  openlog.print( theta );
  openlog.print(", ");
  openlog.print( phi );
  openlog.print(", ");
  openlog.print( mag );
  openlog.print(", ");
  openlog.print( accel.accelerationX() );
  openlog.print(", ");
  openlog.print( accel.accelerationY() );
  openlog.print(", ");
  openlog.println( accel.accelerationZ() );
  
  delay(1000);
  
}


