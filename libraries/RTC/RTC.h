#ifndef RTC_h
#define RTC_h

#include "Arduino.h"
#include "Wire.h"

#define RTC_ADDRESS 0x68
#define TIMESTAMP_LENGTH 18

class RTC
{

	public:
		void begin(void);
		void timestamp( char *buffer );
		void start(void);
	private:
		byte bcdToDec(byte val);
	
};

#endif