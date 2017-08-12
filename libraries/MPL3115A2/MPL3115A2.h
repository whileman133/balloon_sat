/*

  MPL3115A2 altimeter, pressure, and temperature sensor library.
  Based on that of A. Weiss.
  
  Author: W. Hileman, last modified 26 May 2014.
  MIT Lisense.

*/

#ifndef MPL3115A2_h
#define MPL3115A2_h

#include "Arduino.h"
#include "Wire.h"

enum MPLMode { NONE, PRESSURE, ALTITUDE };

class MPL3115A2
{

	public:
		MPL3115A2(void);
		bool begin( byte oversample_rate );
		bool isRunning(void);
		void barometer(void);
		void altimeter(void);
		float temperature(void);
		float pressure(void);
		float altitude(void);
	private:
		byte read( byte address );
		void write( byte address, byte value );
		bool aquire( byte num_bytes, byte address );
		bool temperatureReady(void);
		bool pressureReady(void);
		bool altitudeReady(void);
		void oneShot(void);
		void oversampleRate( byte rate );
		void fifoMode( byte mode );
		void activate(void);
		void standby(void);
		void eventFlags(void);
		enum MPLMode _mode;
		
};

#endif