// The present count of particles detected by the
// geiger counter.
unsigned long geigerCount = 0;
// The time of the last read of the Geiger counter.
unsigned long lastGeigerRead;

#define GEIGER_INTERRUPT 3

void countGeiger(void)
{
  geigerCount++;
}

void setup() 
{
  Serial.begin(9600);
  attachInterrupt(GEIGER_INTERRUPT, countGeiger, RISING);
  lastGeigerRead = millis();
  Serial.println( "Time (ms), Count" );
}

void loop() 
{
  while( millis() - lastGeigerRead < 1000 ) delayMicroseconds(10);
  Serial.print( millis() );
  Serial.print( "," );
  Serial.println( geigerCount );
  geigerCount = 0;
  lastGeigerRead = millis();
}

float readGeiger()
{
  float countsPerSecond = (float)(geigerCount*1000) / (float)(millis() - lastGeigerRead);
  
  geigerCount = 0;
  lastGeigerRead = millis();
  
  return countsPerSecond;
}
