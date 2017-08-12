// Heater.cpp
//
// Written by Wes Hileman on 22 March 2015
//
// Purpose: To drive a heater enabled by a digital pin
// according to the temperature reported by a thermistor.
//
// NOTE: The temperature reported by the thermistor should
// be in degrees C.

#include "Heater.h"

Heater::Heater( int enablePin, float centerTemp, float epsilon, Thermistor* thermistor )
{
	_enPin = enablePin;
	_Tc = centerTemp;
	_ep = epsilon;
	_thm = thermistor;
} // end constructor Heater

void Heater::begin(void)
{
	pinMode( _enPin, OUTPUT );
	// Disable the heater (enable bar)
	digitalWrite( _enPin, HIGH );
} // end begin

bool Heater::cycle(void)
{
	float temp = (*_thm).temp();

	if( temp < _Tc - _ep )
	{
		// Enable the heater
		digitalWrite( _enPin, LOW );
		return true;
	} // end if
	else if( temp > _Tc + _ep )
	{
		// Disable the heater
		digitalWrite( _enPin, HIGH );
		return false;
	} // end else...if
} // end cycle