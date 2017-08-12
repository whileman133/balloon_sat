#ifndef VoltageDivider_h
#define VoltageDivider_h

#include "Arduino.h"
#include "MUX.h"

class VoltageDivider
{

	public:
		VoltageDivider( int pin, unsigned int rUpper, unsigned int rLower, float vOut, MUX* mux = NULL, int mPin = NULL );
		void begin(void);
		float voltage(void);
	private:
		int _pin, _mPin;
		unsigned int _rUpper, _rLower;
		float _vOut;
		MUX* _mux;
};

#endif