// TestTermistor.ino
//
// Written by Wes Hileman on 07 Feb. 2015
//
// Purpose: To retrieve temperature values from
// an NTC termistor via ADC.

#include <Thermistor.h>

#define R 9800.0 // Resistance of series resistor
#define B 3950.0 // B-parameter
#define To 297.6 // Ambient temperature
#define Ro 10730.0 // Them. resistance at To

#define ADC_IN A5

Thermistor thm( ADC_IN, R, B, To, Ro );

void setup()
{
  analogReference( EXTERNAL );
  Serial.begin( 9600 ); 
  thm.begin();
} // end setup

void loop()
{
  // Sample and print temperature (in degrees C)
  Serial.println( thm.temp() );
  delay( 500 );
} // end loop
