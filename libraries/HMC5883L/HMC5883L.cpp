// HMC5883L.cpp
//
// Written by Wes Hileman on 21 Feb. 2015
// MIT License.
//
// Purpose: To process raw magnetic data acquired by the HMC5883L I2C
// triple-axis magnetometer into compass data.

#include "Arduino.h"
#include "HMC5883L.h"
#include "Wire.h"

#define ADDRESS 0x1E
#define CONFIG_REG_BEGIN 0x00
#define DATA_REG_BEGIN 0x03

HMC5883L::HMC5883L( enum AverageSamples av, enum SamplingRate sr, enum Gain gain, enum Mode mode )
{
	_AV = av;
	_SR = sr;
	_Gain = gain;
	_Mode = mode;
	
	// The gain value is needed to compute field magnitude in gauss,
	// so extract it from the input gain enum value.
	switch( gain )
	{
		case G1370:
			_gain = 1370;
			break;
		case G1090:
			_gain = 1090;
			break;
		case G820:
			_gain = 820;
			break;
		case G660:
			_gain = 660;
			break;
		case G440:
			_gain = 440;
			break;
		case G390:
			_gain = 390;
			break;
		case G330:
			_gain = 330;
			break;
		case G230:
			_gain = 230;
			break;
		default:
			_gain = 0; // will result in an infinite magnitude
	} // end switch
}

bool HMC5883L::begin()
{
	
	//
	// Register Configuration
	//
	Wire.beginTransmission( ADDRESS );
	// Select the first configuration register
	Wire.write( CONFIG_REG_BEGIN );
	// Set number of samples to average and data output rate
	Wire.write( _AV<<5 | _SR<<2 );
	// Set gain
	Wire.write( _Gain<<5 );
	// Set mode
	Wire.write( _Mode );
	Wire.endTransmission();
	
	return true;

} // end begin

void HMC5883L::data( float* theta, float* phi, float* mag )
{
	
	// Magnetic-field data
	int x,y,z;
	x = y = z = 0;
	readXYZ( &x, &y, &z );
	
	*theta = atan2( y, x ) * 180 / PI;
	*phi = atan2( sqrt( square( x ) + square( y ) ), z ) * 180 / PI;
	*mag = sqrt( square( x ) + square( y ) + square( z ) ) / (float)_gain;
	
	//*azimuth   = atan2( y,x ) * 180 / PI; // Angle from x-axis in xy-plane
	//*elevation = atan2( z,x ) * 180 / PI; // Angle from x-axis in xz-plane
	//*horizon   = atan2( z,y ) * 180 / PI; // Angle from y-axis in yz-plane
	// Compute magnitude of the magnetic field in Gauss.
	
} // end data

void HMC5883L::readXYZ( int* x, int* y, int* z )
{
	
	// Select the first data register
	Wire.beginTransmission( ADDRESS );
	Wire.write( DATA_REG_BEGIN );
	Wire.endTransmission();
	
	Wire.requestFrom( ADDRESS, 6 );
	if( Wire.available() >= 6 )
	{
		*x = Wire.read()<<8;  // X msb
    *x |= Wire.read();    // X lsb
    *z = Wire.read()<<8;  // Z msb
    *z |= Wire.read();    // Z lsb
    *y = Wire.read()<<8;  // Y msb
    *y |= Wire.read();    // Y lsb
	} // end if

} // end readXYZ