#ifndef ML8511_h
#define ML8511_h

#include "Arduino.h"

class ML8511
{

	public:
		ML8511(int pin, float analog_ref = 5.0);
		void begin(void);
		float voltage(void);
		float intensity(void);
	private:
		int _pin;
		float _aref;

};

#endif