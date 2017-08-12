#include "VoltageDivider.h"

VoltageDivider::VoltageDivider( int pin, unsigned int rUpper, unsigned int rLower, float vOut, MUX* mux, int mPin )
{
	_pin = pin;
	_rUpper = rUpper;
	_rLower = rLower;
	_vOut = vOut;
	_mux = mux;
	_mPin = mPin;
} // end constructor Heater

void VoltageDivider::begin(void)
{
	if( !_mux )
	{
		pinMode( _pin, INPUT );
	} // end if
} // end begin

float VoltageDivider::voltage(void)
{
	unsigned int adcOut;

	if( _mux )
	{
		(*_mux).select( _pin );
		adcOut = analogRead( _mPin );
	} // end if
	else
	{
		adcOut = analogRead( _pin );
	} // end else
	
	return ( (float)_rUpper/(float)_rLower + 1 ) * _vOut * ((float)adcOut / 1023.0);
} // end cycle