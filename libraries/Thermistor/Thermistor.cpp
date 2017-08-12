// Thermistor.cpp
//
// Written by Wes Hileman on 7 March 2015
//
// Purpose: To read temperature data from a thermistor.
// Also provides logic to interface the thermistor to a MUX.

#include "Thermistor.h"

Thermistor::Thermistor( int pin, int seriesResistance, int bParam, float tAmb, int rAmb, MUX* mux, char muxPin)
{
	_pin = pin;
	_R = seriesResistance;
	_B = bParam;
	_To = tAmb;
	_Ro = rAmb;
	_mux = mux;
	_muxPin = muxPin;
} // end constructor

void Thermistor::begin(void)
{
	pinMode( _pin, INPUT );
} // end begin

float Thermistor::temp(void)
{

	float res;
	
	if( _R && _B && _To && _Ro )
	{
	  res =  ( 1023.0 / (float)raw_adc() - 1.0 ) * _R;
	  return -273.15 + 1.0 / ( 1.0/(float)_To + log( res/(float)_Ro )/(float)_B );
	}
	else
	{
		return -1.0;
	}
	
} // end temp

uint16_t Thermistor::raw_adc(void)
{
	
	// If a MUX object was passed in, select the supplied muxPin
	// before sampling the ADC value.
	if( _mux )
	{
		(*_mux).select( _muxPin );
	} // end if
	
	return analogRead( _pin );
	
} // end adc_raw