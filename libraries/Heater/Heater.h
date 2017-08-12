// Heater.h
//
// Written by Wes Hileman on 22 March 2015
//
// Purpose: To drive a heater enabled by a digital pin
// according to the temperature reported by a thermistor.
//
// NOTE: The temperature reported by the thermistor should
// be in degrees C.

#ifndef Heater_h
#define Heater_h

#include "Arduino.h"
#include "Thermistor.h"

class Heater
{

	public:
		Heater( int enablePin, float centerTemp, float epsilon, Thermistor* thermistor );
		void begin(void);
		bool cycle(void);
	private:
		int _enPin;
		float _Tc, _ep;
		Thermistor* _thm;
};

#endif