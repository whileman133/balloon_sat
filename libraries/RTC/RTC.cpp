#include "Arduino.h"
#include "RTC.h"
#include "Wire.h"

void RTC::begin(void)
{

	start();

}

void RTC::timestamp( char *buffer )
{
	
	byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
	
	Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(byte(0x00));
  Wire.endTransmission();
	
  Wire.requestFrom(RTC_ADDRESS, 7);
	
	if (Wire.available() < 7);
	
  second     = bcdToDec(Wire.read() & 0x7f);
  minute     = bcdToDec(Wire.read());
  hour       = bcdToDec(Wire.read() & 0x3f);  
  dayOfWeek  = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year       = bcdToDec(Wire.read());
	
	sprintf( buffer, "%02d-%02d-%02d %02d:%02d:%02d", year, month, dayOfMonth, hour, minute, second );
}

void RTC::start()
{

	byte enable;

	Wire.beginTransmission(RTC_ADDRESS);
	Wire.write(byte(0x00));
	Wire.endTransmission();
	
	Wire.requestFrom(RTC_ADDRESS, 1);
	
	if (Wire.available() > 0)
	{
		enable = Wire.read() & B01111111;
		
		Wire.beginTransmission(RTC_ADDRESS);
		Wire.write(0x00);
		Wire.write(enable);
		Wire.endTransmission();
	}

}

byte RTC::bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}