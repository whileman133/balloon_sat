/*

  MPL3115A2 altimeter, pressure, and temperature sensor example code.
  Based on that of A. Weiss.
  
  Author: W. Hileman, last modified 26 May 2014.
  MIT Lisense.
  
  Pin Mapping:
    Vdd = 3.3v
    SDA = A4
    SCL = A5
    INT1, INT2 = unconnected

*/

#include <Wire.h>
#include <MPL3115A2.h>

MPL3115A2 altimeter;

void setup()
{
  
  Wire.begin();
  Serial.begin(9600);
  
  if ( altimeter.begin( 7 ) ) {
    Serial.println("Altimeter Online!");
  } 
  else {
    Serial.println("Altimeter NACK");
    while(1);
  }
  
  Serial.println("Alt (m), Psr (Pa), Tmp (C)");
}


void loop()
{
  float psr = altimeter.pressure();
  
  Serial.println(psr, 2);
  
}

