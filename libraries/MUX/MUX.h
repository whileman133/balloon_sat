// MUX.h
//
// Written by Wes Hileman on Mar. 7 2015

#ifndef MUX_h
#define MUX_h

class MUX
{

	public:
		MUX( int* selectPins, unsigned int size );
		bool select( char pin );
	private:
		unsigned int _numSelect;
		unsigned int _muxSize;
		int* _selectPins;
		bool isValidPin( char pin );
		int ipow( int base, int exp );
		
};

#endif