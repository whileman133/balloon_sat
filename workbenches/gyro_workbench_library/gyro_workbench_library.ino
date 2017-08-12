#include <Wire.h>
#include <ITG3200.h>

const int xoff = -27, yoff = 46, zoff = -13;
ITG3200 gyro( &xoff, &yoff, &zoff );

void setup() 
{
  Serial.begin( 9600 );
  Wire.begin();
  gyro.begin( ITG_BW98HZ, 0  );
  Serial.println( "X Vel, Y Vel, Z Vel" );
}

void loop() 
{
  static int x = 0, y = 0, z = 0;
  
  gyro.data( &x, &y, &z );
  
  Serial.print( x );
  Serial.print( ", " );
  Serial.print( y );
  Serial.print( ", " );
  Serial.println( z );
  
  delay( 50 );
}
