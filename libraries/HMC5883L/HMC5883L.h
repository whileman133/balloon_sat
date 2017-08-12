// HMC5883L.h
//
// Written by Wes Hileman on 21 Feb. 2015
// MIT License.
//
// Purpose: To process raw magnetic data acquired by the HMC5883L I2C
// triple-axis magnetometer into compass data.

#ifndef HMC5883L_h
#define HMC5883L_h

enum AverageSamples
{
	AV1 = 0x0,
	AV2 = 0x1,
	AV4 = 0x2,
	AV8 = 0x3
};

enum SamplingRate 
{ 
	F0_75HZ = 0x0, 
	F1_5HZ =  0x1, 
	F3HZ =    0x2, 
	F7_5HZ =  0x3, 
	F15HZ =   0x4, 
	F30HZ =   0x5, 
	F75HZ =   0x6 
};

enum Gain 
{ 
	G1370 = 0x0, 
	G1090 = 0x1, 
	G820 =  0x2, 
	G660 =  0x3, 
	G440 =  0x4, 
	G390 =  0x5, 
	G330 =  0x6,
	G230 =  0x7  
};

enum Mode
{
	CONTINUOUS = 0x0,
	IDLE =       0x2
};

#include "Arduino.h"
#include "Wire.h"

class HMC5883L
{

	public:
		HMC5883L( enum AverageSamples av, enum SamplingRate sr, enum Gain gain, enum Mode mode );
		bool begin();
		void data( float* theta, float* phi, float* mag );
	private:
		enum AverageSamples _AV;
		enum SamplingRate _SR;
		enum Gain _Gain;
		enum Mode _Mode;
		int _gain;
		void readXYZ( int* x, int* y, int* z );
		
};

#endif