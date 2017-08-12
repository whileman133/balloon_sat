// Thermistor.h
//
// Written by Wes Hileman on 7 March 2015
//
// Purpose: To read temperature data from a thermistor.
// Also provides logic to interface the thermistor to a MUX.

#ifndef Thermistor_h
#define Thermistor_h

#include "Arduino.h"
#include "MUX.h"

class Thermistor
{

	public:
		Thermistor( int pin, int seriesResistance = NULL, int bParam = NULL, float tAmb = NULL, int rAmb = NULL, MUX* mux = NULL, char muxPin = NULL );
		void begin(void);
		float temp(void);
	private:
		int _R, _B, _Ro, _pin;
		float _To;
		MUX* _mux;
		char _muxPin;
};

#endif