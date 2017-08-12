#include <MUX.h>
#include <VoltageDivider.h>

VoltageDivider VD( A0, 9910, 4620, 5.0 );

void setup() {
  Serial.begin( 9600 );
  VD.begin();
}

void loop() {
  Serial.println( VD.voltage(), 2 );
  delay( 500 );
}
