/*

  MPL3115A2 altimeter, pressure, and temperature sensor library.
  Based on that of A. Weiss.
  
  Author: W. Hileman, last modified 26 May 2014.
  MIT Lisense.

*/

#include "Arduino.h"
#include "MPL3115A2.h"
#include "Wire.h"

#define STATUS 0x00
#define OUT_P_MSB 0x01
#define OUT_P_CSB 0x02
#define OUT_P_LSB 0x03
#define OUT_T_MSB 0x04
#define OUT_T_LSB 0x05
#define DR_STATUS 0x06
#define OUT_P_DELTA_MSB 0x07
#define OUT_P_DELTA_CSB 0x08
#define OUT_P_DELTA_LSB 0x09
#define OUT_T_DELTA_MSB 0x0A
#define OUT_T_DELTA_LSB 0x0B
#define WHO_AM_I 0x0C
#define F_STATUS 0x0D
#define F_DATA 0x0E
#define F_SETUP 0x0F
#define TIME_DLY 0x10
#define SYSMOD 0x11
#define INT_SOURCE 0x12
#define PT_DATA_CFG 0x13
#define BAR_IN_MSB 0x14
#define BAR_IN_LSB 0x15
#define P_TGT_MSB 0x16
#define P_TGT_LSB 0x17
#define T_TGT 0x18
#define P_WND_MSB 0x19
#define P_WND_LSB 0x1A
#define T_WND 0x1B
#define P_MIN_MSB 0x1C
#define P_MIN_CSB 0x1D
#define P_MIN_LSB 0x1E
#define T_MIN_MSB 0x1F
#define T_MIN_LSB 0x20
#define P_MAX_MSB 0x21
#define P_MAX_CSB 0x22
#define P_MAX_LSB 0x23
#define T_MAX_MSB 0x24
#define T_MAX_LSB 0x25
#define CTL_REG1 0x26
#define CTL_REG2 0x27
#define CTL_REG3 0x28
#define CTL_REG4 0x29
#define CTL_REG5 0x2A
#define OFF_P 0x2B
#define OFF_T 0x2C
#define OFF_H 0x2D

#define MPL3115A2_ADDRESS 0x60

#define TIMEOUT 1000 // ms

#define WHO_I_AM 196

MPL3115A2::MPL3115A2(void)
{
	_mode = NONE;
}

bool MPL3115A2::begin( byte oversample_rate )
{
	
	if (!isRunning()) return false;
	
	altimeter();
	oversampleRate( oversample_rate );
	eventFlags();
	
	return true;

}

bool MPL3115A2::isRunning( void )
{

	if ( read(WHO_AM_I) != WHO_I_AM ) return false;
	
	return true;

}

float MPL3115A2::altitude(void)
{
	
	if( _mode != ALTITUDE )
	{
		altimeter();
	}

	oneShot();
	
	if ( ! (altitudeReady() && aquire( 3, OUT_P_MSB )) ) return -999;
	
	byte msb, csb, lsb;
  msb = Wire.read();
  csb = Wire.read();
  lsb = Wire.read();
  
  return (float)( (msb << 8) | csb) + (float)( (lsb>>4)/16.0 );

}

float MPL3115A2::pressure(void)
{
	
	if( _mode != PRESSURE )
	{
		barometer();
	}
	
	oneShot();
	
	if ( ! (pressureReady() && aquire( 3, OUT_P_MSB )) ) return -999;
	
	byte msb, csb, lsb;
  msb = Wire.read();
  csb = Wire.read();
  lsb = Wire.read();
  
  long pressure_whole = (long)msb<<16 | (long)csb<<8 | (long)lsb;
  // Shift out the four unused bits and the two decimal bits.
  pressure_whole >>= 6;

	// Obtain the decimal component.
  lsb &= B00110000;
  lsb >>= 4;

  return (float)pressure_whole + ((float)lsb/4.0);

}

float MPL3115A2::temperature(void)
{

	oneShot();
	
	if ( ! (temperatureReady() && aquire( 2, OUT_T_MSB )) ) return -999;
  
  byte msb, lsb;
  msb = Wire.read();
  lsb = Wire.read();
  
  return (float)(msb + ((lsb>>4)/16.0));

}

void MPL3115A2::altimeter(void)
{

	byte reg_value;
	
	reg_value = read( CTL_REG1 );
	// Set the ALT bit.
	reg_value |= B10000000;
	write( CTL_REG1, reg_value );
	
	_mode = ALTITUDE;

}

void MPL3115A2::barometer(void)
{

	byte reg_value;
	
	reg_value = read( CTL_REG1 );
	// Clear the ALT bit.
	reg_value &= B01111111;
	write( CTL_REG1, reg_value );
	
	_mode = PRESSURE;

}

void MPL3115A2::activate(void)
{

	byte reg_value;
	
	reg_value = read( CTL_REG1 );
	// Set the SBYB bit.
	reg_value |= B00000001;
	write( CTL_REG1, reg_value );

}

void MPL3115A2::standby(void)
{

	byte reg_value;
	
	reg_value = read( CTL_REG1 );
	// Clear the SBYB bit.
	reg_value &= B1111110;
	write( CTL_REG1, reg_value );

}

void MPL3115A2::fifoMode( byte mode )
{

	if ( mode > 3 ) mode = 3;
	mode <<= 6;
	
	byte reg_value;
	
	reg_value = read( F_SETUP );
	// Clear the f_mode bits.
	reg_value &= B00111111;
	// Set the bits according to mode.
	reg_value |= mode;
	write( F_SETUP, reg_value );

}

void MPL3115A2::oversampleRate( byte rate )
{
	
	if ( rate > 7 ) rate = 7;
	rate <<= 3;
	
	byte reg_value;
	
	reg_value = read( CTL_REG1 );
	reg_value &= B11000111;
	reg_value |= rate;
	write( CTL_REG1, reg_value );
	
}

void MPL3115A2::eventFlags(void)
{

	write( PT_DATA_CFG, 0x07 );
	
}

void MPL3115A2::oneShot( void )
{

	byte reg_value;
	
	reg_value = read( CTL_REG1 );
	// Clear the one-shot (OST) bit
	reg_value &= B11111101;
	write( CTL_REG1, reg_value );
	
	reg_value = read( CTL_REG1 );
	// Set the one-shot bit.
	reg_value |= B00000010;
	write( CTL_REG1, reg_value );

}

bool MPL3115A2::altitudeReady(void)
{
	
	return pressureReady();
	
}

bool MPL3115A2::pressureReady(void)
{

	int counter = 0;
	while ( (read( STATUS ) & B0000100) == 0 )
	{
		if ( ++counter > TIMEOUT ) return false;
		delay(1);
	}
	
	return true;

}

bool MPL3115A2::temperatureReady(void)
{
	
	int counter = 0;
	while ( (read( STATUS ) & B0000010) == 0 )
	{
		if ( ++counter > TIMEOUT ) return false;
		delay(1);
	}
	
	return true;

}

bool MPL3115A2::aquire( byte num_bytes, byte address )
{

	Wire.beginTransmission( MPL3115A2_ADDRESS );
  Wire.write( address );
  Wire.endTransmission( false );

	Wire.requestFrom( (byte)MPL3115A2_ADDRESS, num_bytes );
  
  int counter = 0;
  while(Wire.available() < num_bytes)
  {
    if( ++counter > TIMEOUT ) return false;
    delay(1);
  }
	
	return true;

}

byte MPL3115A2::read( byte address )
{

	Wire.beginTransmission( MPL3115A2_ADDRESS );
	Wire.write( address );
	Wire.endTransmission( false );
	Wire.requestFrom( MPL3115A2_ADDRESS, 1 );
	return Wire.read();

}

void MPL3115A2::write( byte address, byte value )
{

	Wire.beginTransmission( MPL3115A2_ADDRESS );
	Wire.write( address );
	Wire.write( value );
	Wire.endTransmission( true );

}