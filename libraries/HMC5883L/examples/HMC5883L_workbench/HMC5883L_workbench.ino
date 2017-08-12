#include <Wire.h> // Arduino I2C Library
#include <HMC5883L.h>

// Initialize HMC5883L object
HMC5883L compass( AV8, F3HZ, G1370, CONTINUOUS );
// Parameters for the constructor are as follows.
//  1: Number of samples to average per output
//  2: Data output update rate
//  3: Gain ( output units / gauss )
//  4: Mode

void setup(){
  Serial.begin(9600);
  Wire.begin();
  compass.begin();
}

void loop()
{
  
  // Azimuth, elevation, & horizon relative to the magnetic
  // field at the position of the magnetomter. mag is the
  // magnitude of the field at the position.
  static float az = 0, ele = 0, hor = 0, mag = 0;
  
  // Load data into the variables from the compass.
  compass.data( &az, &ele, &hor, &mag );
  
  // Print the results.
  Serial.print("Azimuth: ");
  Serial.print( az );
  Serial.print("  Elevation: ");
  Serial.print( ele );
  Serial.print("  Horizon: ");
  Serial.print( hor );
  Serial.print("  B: ");
  Serial.print( mag );
  Serial.println(" gauss");
  
  delay(250);
}
