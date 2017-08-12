// MUX.cpp
//
// Written by Wes Hileman on Mar. 7 2015

#include "Arduino.h"
#include "MUX.h"

MUX::MUX( int* selectPins, unsigned int size )
{

	_selectPins = selectPins;
	_numSelect = size;
	_muxSize = ipow( 2,size ) - 1;
	
	// Set the pin modes of the select pins to output
	for( int index = 0; index < size; index++ )
	{
		pinMode( selectPins[ index ], OUTPUT );
	} // end for
	
} // end MUX

bool MUX::select( char pin )
{

	if( !isValidPin( pin ) )
	{
		return false;
	} // end if

	for( int index = 0; index < _numSelect; index++ )
	{
		
		digitalWrite( _selectPins[ index ], ( bitRead( pin, index ) == 1 ) ? HIGH : LOW );
		
	} // end for
	
	return true;

} // end select

bool MUX::isValidPin( char pin )
{
	return 0 <= (int)pin && (int)pin <= _muxSize;
} // end isValidPin

int MUX::ipow( int base, int exp )
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
} // end ipow