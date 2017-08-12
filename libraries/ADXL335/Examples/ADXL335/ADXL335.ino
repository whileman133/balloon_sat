#include <ADXL335.h>

ADXL335 accelerometer(A1, A2, A3);

void setup()
{
  
  Serial.begin(9600);
  accelerometer.begin();
  
}

void loop()
{
  
  float x, y, z;
  x = accelerometer.voltageX();
  y = accelerometer.voltageY();
  z = accelerometer.voltageZ();
  
  Serial.print("X (volts): ");
  Serial.println(x);
  
  Serial.print("Y (volts): ");
  Serial.println(y);
  
  Serial.print("Z (volts): ");
  Serial.println(z);
  
  delay(10);
  
}

