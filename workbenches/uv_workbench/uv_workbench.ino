#include <ML8511.h>

ML8511 uvSensor(A5, 3.3);

void setup()
{
  
  Serial.begin(9600);
  uvSensor.begin();
  analogReference( EXTERNAL );
  
}

void loop()
{
  
  float v = uvSensor.voltage();
  
  Serial.print("UV (volts): ");
  Serial.println(v, 5);
  
  delay(10);
  
}


