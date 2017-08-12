#include <SoftwareSerial.h>

SoftwareSerial LCD( 5,4 );

void setup()
{
  
  // Start serial communication with
  // the lcd at 9600 baud.
  LCD.begin( 9600 );
  // Send calibration code to LCD.
  LCD.print("?f?c0?G420?f");
  
} // end setup

void loop()
{
  
  // Clear the screen with the escape sequence ?f
  LCD.print( "?f" );
  
  // The escape sequence ?n gives a newline
  LCD.print( "PWR:14.5V      ?n" );
  LCD.print( "THM:b=25C HEAT 0001?n" );
  LCD.print( "SEN:G1-5 G2-10 ?n" );
  LCD.print( "    UV-.01 ALT-1000" );
  
  // Wait 5 sec
  delay( 5000 );
  
} // end loop


