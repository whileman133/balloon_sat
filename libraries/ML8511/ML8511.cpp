#include "Arduino.h"
#include "ML8511.h"

ML8511::ML8511( int pin, float analog_ref )
{

	_pin = pin;
	_aref = analog_ref;

}

void ML8511::begin(void)
{

	pinMode( _pin, INPUT );

}

float ML8511::voltage(void)
{

	float value = analogRead( _pin );
	
	return (value * _aref) / 1023.0;

}

float ML8511::intensity(void)
{
	
	return (voltage() - 0.98)*8.33;
	
}