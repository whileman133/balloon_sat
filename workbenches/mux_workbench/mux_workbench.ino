// mux_workbench.ino
//
// Written by Wes Hileman on 7 March 2015
// MIT License.
// 
// Purpose: To sequentially illuminate 16 LEDs with a 16:1 MUX.

#include <MUX.h>

// Select pins
int pins[ 4 ] = { A1, A2, A3, A4 };
// MUX object
MUX mux( pins, 4 );

void setup() 
{
  Serial.begin( 9600 );
}

void loop() 
{ 
  static char muxPin = 0;
  static bool countUp = true;
  
  mux.select( muxPin );
  
  if( muxPin >= 15 )
  {
    countUp = false;
  } // end if
  else if( muxPin <= 0 )
  { 
    countUp = true;
  } // end else
  
  if( countUp )
  {
    ++muxPin;
  } // end if
  else
  {
    --muxPin;
  } // end else
  
  delay( 50 );
}
