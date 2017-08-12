#ifndef ADXL335_h
#define ADXL335_h

#include "Arduino.h"
#include "MUX.h"

class ADXL335
{

	public:
		ADXL335( float aref, int x_pin, int y_pin, int z_pin, MUX* mux = NULL, int muxPin = NULL );
		void begin(void);
		float voltageX(void);
		float voltageY(void);
		float voltageZ(void);
		float accelerationX(void);
		float accelerationY(void);
		float accelerationZ(void);
	private:
		float _aref;
		int _x_pin, _y_pin, _z_pin, _muxPin;
		MUX* _mux;

};

#endif