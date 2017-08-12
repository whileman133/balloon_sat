#include <ADXL335.h>
#include <MUX.h>

int pins[ 4 ] = { A5, A4, A3, A2 };

MUX mux( pins, 4 );
ADXL335 accelerometer( 3.3, 15, 14, 13, &mux, A1 );

void setup()
{
  
  analogReference( EXTERNAL );
  
  pinMode( A1, INPUT );
  
  Serial.begin(9600);
  accelerometer.begin();
  
  Serial.println("X (g), Y (g), Z (g)");
}

void loop()
{
  
  float x, y, z;
  x = accelerometer.accelerationX();
  y = accelerometer.accelerationY();
  z = accelerometer.accelerationZ();
  
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.print(z);
  Serial.println();
  
  delay(100);
  
}

