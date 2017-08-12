// thermal_subsystem_testbench.ino
//
// Written by Wes Hileman on 22 March 2015
// MIT License.
// 
// Purpose: To test the thermal subsystem (heaters & thermistors).

#include <ML8511.h>
#include <MUX.h>
#include <Thermistor.h>
#include <Heater.h>

#define NUM_MUX_SELECT_PINS 4

// MUX pin-aliases
enum MuxPin { M0, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15 };
// Select pins
int pins[ NUM_MUX_SELECT_PINS ] = { A1, A2, A3, A4 };
// MUX object
MUX mux( pins, NUM_MUX_SELECT_PINS );
// UV sensor
ML8511 uvSensor(A5, 3.3);

// Thermistor objects (all thermistors are connected
// to the same input pin by the MUX)
Thermistor thm0i( A0, 9650.0, 3950.0, 292.1, 13630.0, &mux, M0 );
Thermistor thm1i( A0, 9990.0, 3950.0, 292.1, 13630.0, &mux, M1 );
Thermistor thm2i( A0, 9880.0, 3950.0, 292.1, 13430.0, &mux, M2 );
Thermistor thm3i( A0, 9900.0, 3950.0, 292.1, 13250.0, &mux, M3 );
Thermistor thm4e( A0, 9940.0, 3950.0, 292.1, 13570.0, &mux, M4 );
Thermistor thm5e( A0, 9950.0, 3950.0, 292.1, 13280.0, &mux, M5 );
Thermistor thm6e( A0, 9960.0, 3950.0, 292.1, 13350.0, &mux, M6 );
Thermistor thm7e( A0, 10040.0, 3950.0, 292.1, 13370.0, &mux, M7 );

// Heater objects
Heater h1( 4, 29.0, 1.0, &thm0i );
Heater h2( 5, 29.0, 1.0, &thm1i );
Heater h3( 6, 29.0, 1.0, &thm2i );
Heater h4( 7, 29.0, 1.0, &thm3i );

void setup() 
{
  Serial.begin( 9600 );
  uvSensor.begin();
  // Thermistors are on the same pin A0, so only one call
  // to begin() is needed.
  thm0i.begin();
  h1.begin();
  h2.begin();
  h3.begin();
  h4.begin();
  // Thermistors are placed in a voltage divider with Vcc = 3.3V, so
  // use 3.3V as the external analog reference.
  analogReference( EXTERNAL );
  Serial.println( "Therm 0, Therm 1, Therm 2, Therm 3, Therm 4, Therm 5, Therm 6, Therm 7, UV" );
}

void loop() 
{ 
  delay( 2000 );
  
  float v = uvSensor.intensity();
  
  Serial.print( thm0i.temp() );
  Serial.print( ", " );
  Serial.print( thm1i.temp() );
  Serial.print( ", " );
  Serial.print( thm2i.temp() );
  Serial.print( ", " );
  Serial.print( thm3i.temp() );
  Serial.print( ", " );
  Serial.print( thm4e.temp() );
  Serial.print( ", " );
  Serial.print( thm5e.temp() );
  Serial.print( ", " );
  Serial.print( thm6e.temp() );
  Serial.print( ", " );
  Serial.print( thm7e.temp() );
  Serial.print(", ");
  Serial.println( v, 5 );
  
  h1.cycle();
  h2.cycle();
  h3.cycle();
  h4.cycle();
}
