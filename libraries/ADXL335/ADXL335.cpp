#include "Arduino.h"
#include "ADXL335.h"

ADXL335::ADXL335( float aref, int x_pin, int y_pin, int z_pin, MUX* mux, int muxPin )
{

	_aref = aref;
	_x_pin = x_pin;
	_y_pin = y_pin;
	_z_pin = z_pin;
	_mux = mux;
	_muxPin = muxPin;

}

void ADXL335::begin(void)
{

	if( _mux )
	{
		pinMode( _muxPin, INPUT);
	} // end if
	else
	{
		pinMode( _x_pin, INPUT );
		pinMode( _y_pin, INPUT );
		pinMode( _z_pin, INPUT );
	} // end else

}

float ADXL335::voltageX(void)
{

	float value = 0;
	
	// If a MUX object was passed in, select the supplied muxPin
	// before sampling the ADC value.
	if( _mux )
	{
		(*_mux).select( (char)_x_pin );
		value = analogRead( _muxPin );
	} // end if
	else
	{
		value = analogRead( _x_pin );
	} // end else
	
	return ((float)value * _aref) / 1023.0;

}

float ADXL335::voltageY(void)
{

	float value;
	
	// If a MUX object was passed in, select the supplied muxPin
	// before sampling the ADC value.
	if( _mux )
	{
		(*_mux).select( (char)_y_pin );
		value = analogRead( _muxPin );
	} // end if
	else
	{
		value = analogRead( _y_pin );
	} // end else
	
	return ((float)value * _aref) / 1023.0;

}

float ADXL335::voltageZ(void)
{

	float value;
	
	// If a MUX object was passed in, select the supplied muxPin
	// before sampling the ADC value.
	if( _mux )
	{
		(*_mux).select( (char)_z_pin );
		value = analogRead( _muxPin );
	} // end if
	else
	{
		value = analogRead( _z_pin );
	} // end else
	
	return ((float)value * _aref) / 1023.0;

}

float ADXL335::accelerationX(void)
{

	return (voltageX() - 1.64)/0.3;

}

float ADXL335::accelerationY(void)
{

	return (voltageY() - 1.64)/0.3;

}

float ADXL335::accelerationZ(void)
{

	return (voltageZ() - 1.64)/0.3;

}