// ApogeePayloadFinal.ino
//
// Written by Wes Hileman on 22 March 2015
//
// Purpose: To control the Apogee payload during flight.

// INCLUDES 

// Spacecraft-bus libraries
#include <avr/wdt.h>                 // watchdog timer
#include <RTC.h>                     // real-time clock
#include <MUX.h>                     // multiplexer
#include <Heater.h>                  // heating pads
#include <SendOnlySoftwareSerial.h>  // serial Tx for OpenLogs and LCD
#include <VoltageDivider.h>          // voltage monitor and solar panels

// Sensor libraries
#include <Wire.h>        // I2C communication library
#include <MPL3115A2.h>   // altimeter
#include <HMC5883L.h>    // triple-axis magnetometer
#include <ML8511.h>      // uv sensor
#include <ADXL335.h>     // triple-axis accelerometer
#include <Thermistor.h>  // thermistor temperature-sensors

// CONSTANTS

// Aliases for multiplexer pins
enum MuxPin { M0, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15 };

#define ANALOG_REF 3.3
#define WATCHDOG_TIMEOUT WDTO_8S
#define LOG_BAUD 9600
#define LCD_BAUD 9600

// Temperature control
#define BATT_CENTER_TEMP 0
#define BATT_EPSILON 5
#define INT_CENTER_TEMP 0
#define INT_EPSILON 5

// Timing
#define CYCLE_TIMEOUT 1000  // milliseconds
#define SLEEP_TIME 10       // microseconds
#define DIAGNOSTIC_TIME 100 // milliseconds

#define ALTIMETER_OVERSAMPLE_RATE 7
#define COMPASS_AV_SAMPLES AV8
#define COMPASS_SR F3HZ
#define COMPASS_GAIN G1370
#define COMPASS_MODE CONTINUOUS

// ========== PIN MAPPING ==========

#define MUX_IO_PIN A1
#define DATA_LOG_TX_PIN 10
#define BATT_HEAT1_PIN 4
#define BATT_HEAT2_PIN 5
#define INT_HEAT1_PIN 6
#define INT_HEAT2_PIN 7
#define TEST_BUTTON_PIN 12
#define LCD_PIN 8
#define VOLT_METER_PIN M4

#define NUM_MUX_SELECT_PINS 4
// Multiplexer select pins
int muxSelectPins[ NUM_MUX_SELECT_PINS ] = { A5, A4, A3, A2 };

#define GEIGER1_INTERRUPT 2
#define GEIGER2_INTERRUPT 3
#define UV_PIN A0
#define ACCEL_X_PIN 15
#define ACCEL_Y_PIN 14
#define ACCEL_Z_PIN 13
#define BATT_THERM1_PIN M12 
#define BATT_THERM2_PIN M11
#define INT_THERM1_PIN M10
#define INT_THERM2_PIN M9
#define EXT_THERM1_PIN M8
#define EXT_THERM2_PIN M7
#define EXT_THERM3_PIN M6
#define EXT_THERM4_PIN M5
#define SOLAR_PANEL1_PIN M0
#define SOLAR_PANEL2_PIN M1
#define SOLAR_PANEL3_PIN M2
#define SOLAR_PANEL4_PIN M3

// ======== END PIN MAPPING ========

// OBJECTS

// Spacecraft-bus objects
RTC clock;
MUX mux( muxSelectPins, NUM_MUX_SELECT_PINS );
SendOnlySoftwareSerial datalog( DATA_LOG_TX_PIN );
SendOnlySoftwareSerial lcd( LCD_PIN );
// Thermistors that control heaters
Thermistor battTherm1( MUX_IO_PIN, 10040.0, 3950.0, 293.2, 12900.0, &mux, BATT_THERM1_PIN );
Thermistor battTherm2( MUX_IO_PIN, 9960.0, 3950.0, 292.1, 13350.0, &mux, BATT_THERM2_PIN );
Thermistor intTherm1( MUX_IO_PIN, 9950.0, 3950.0, 292.1, 13280.0, &mux, INT_THERM1_PIN );
Thermistor intTherm2( MUX_IO_PIN, 9940.0, 3950.0, 292.1, 13570.0, &mux, INT_THERM2_PIN );
// Heater objects (require thermistor objects)
Heater battHeater1( BATT_HEAT1_PIN, BATT_CENTER_TEMP, BATT_EPSILON, &battTherm1 );
Heater battHeater2( BATT_HEAT2_PIN, BATT_CENTER_TEMP, BATT_EPSILON, &battTherm2 );
Heater intHeater1( INT_HEAT1_PIN, INT_CENTER_TEMP, INT_EPSILON, &intTherm1 );
Heater intHeater2( INT_HEAT2_PIN, INT_CENTER_TEMP, INT_EPSILON, &intTherm2 );
// Battery-voltage monitor
VoltageDivider battMeter( VOLT_METER_PIN, 9910, 4620, ANALOG_REF, &mux, MUX_IO_PIN );

// Sensor objects
MPL3115A2 alt;
HMC5883L compass( COMPASS_AV_SAMPLES, COMPASS_SR, COMPASS_GAIN, COMPASS_MODE );
ML8511 uvSen( UV_PIN, ANALOG_REF );
ADXL335 accel( ANALOG_REF, ACCEL_X_PIN, ACCEL_Y_PIN, ACCEL_Z_PIN, &mux, MUX_IO_PIN );
// Thermistors
Thermistor extTherm1( MUX_IO_PIN, 9650.0, 3950.0, 292.1, 13630.0, &mux, EXT_THERM1_PIN );
Thermistor extTherm2( MUX_IO_PIN, 9990.0, 3950.0, 292.1, 13630.0, &mux, EXT_THERM2_PIN );
Thermistor extTherm3( MUX_IO_PIN, 9880.0, 3950.0, 292.1, 13430.0, &mux, EXT_THERM3_PIN );
Thermistor extTherm4( MUX_IO_PIN, 9900.0, 3950.0, 292.1, 13250.0, &mux, EXT_THERM4_PIN );
// Solar panels
VoltageDivider solar1( SOLAR_PANEL1_PIN, 9920, 4590, ANALOG_REF, &mux, MUX_IO_PIN );
VoltageDivider solar2( SOLAR_PANEL2_PIN, 9870, 4570, ANALOG_REF, &mux, MUX_IO_PIN );
VoltageDivider solar3( SOLAR_PANEL3_PIN, 9870, 4650, ANALOG_REF, &mux, MUX_IO_PIN );
VoltageDivider solar4( SOLAR_PANEL4_PIN, 9940, 4620, ANALOG_REF, &mux, MUX_IO_PIN );

// Geiger-counter counts
unsigned long geigerCount1 = 0, geigerCount2 = 0;

void countGeiger1( void )
{
  geigerCount1++;
} // end countGeiger1

void countGeiger2( void )
{
  geigerCount2++;
} // end countGeiger1

void setup()
{
  // Initialize I2C communication
  Wire.begin();
  
  // ========== SPACECRAFT BUS ==========
  
  // Initialize real-time clock
  clock.begin();
  // Initialize MUX I/O pin as input
  pinMode( MUX_IO_PIN, INPUT );
  // Initialize data logging
  datalog.begin( LOG_BAUD );
  // Initialize heaters
  battHeater1.begin();
  battHeater2.begin();
  intHeater1.begin();
  intHeater2.begin();
  // Initialize test-button input
  pinMode( TEST_BUTTON_PIN, INPUT );
  // Initialize lcd
  lcd.begin( LCD_BAUD );
  // Send calibration code to lcd
  lcd.print("?f?c0?G420?f");
  // Initialize battery voltage-monitor
  battMeter.begin();
  
  // Set analog reference voltage to external AREF (3.3V regulator)
  analogReference( EXTERNAL );
  // Enable watchdog timer
  wdt_enable( WATCHDOG_TIMEOUT );
  
  // Print data-file header
  datalog.println( "Time (ms), Timestamp, Pressure (Pa), UV Int (mW/cm^2), Comp Theta (deg), Comp Phi (deg), Comp Mag (G), X Accel (g), Y Accel (g), Z Accel (g), Batt T1 (C), Batt T2 (C), INT T1 (C), INT T2 (C), EXT T1 (C), EXT T2 (C), EXT T3 (C), EXT T4 (C), Heat1, Heat2, Heat3, Heat4, GC1 (counts/cycle), GC2 (counts/cycle), Batt V, Solar1 (V), Solar2 (V), Solar3 (V), Solar4 (V)" );
  
  // ============ PAYLOAD ============
  
  // Initialize altimter
  alt.begin( ALTIMETER_OVERSAMPLE_RATE );
  alt.barometer();
  // Initialize magnetometer
  compass.begin();
  // Initialize UV sensor
  uvSen.begin();
  // Initialize accelerometer
  accel.begin();
  // Initialize Geiger-counter interrupts
  attachInterrupt( GEIGER1_INTERRUPT, countGeiger1, RISING );
  attachInterrupt( GEIGER2_INTERRUPT, countGeiger2, RISING );
  // Initialize solar panels
  solar1.begin();
  solar2.begin();
  solar3.begin();
  solar4.begin();
} // end setup

void loop() 
{
  // Update the start time of the latest cycle
  unsigned long lastCycleTime = millis();
  // String to store timestamp data
  char timestamp[ TIMESTAMP_LENGTH ] = "";
  // Variables to store sensor data
  float pressure, uv, compassTheta, compassPhi, compassMag, accelX, accelY, accelZ;
  float battTemp1, battTemp2, intTemp1, intTemp2, extTemp1, extTemp2, extTemp3, extTemp4;
  float battV, solar1V, solar2V, solar3V, solar4V;
  unsigned long gCount1, gCount2;
  bool heat1State, heat2State, heat3State, heat4State;
  
  // Reset watchdog timer
  wdt_reset();
  
  // Collect data
  clock.timestamp( &timestamp[ 0 ] );
  pressure = alt.pressure();
  uv = uvSen.intensity();
  compass.data( &compassTheta, &compassPhi, &compassMag );
  accelX = accel.accelerationX();
  accelY = accel.accelerationY();
  accelZ = accel.accelerationZ();
  battTemp1 = battTherm1.temp();
  battTemp2 = battTherm2.temp();
  intTemp1 = intTherm1.temp();
  intTemp2 = intTherm2.temp();
  extTemp1 = extTherm1.temp();
  extTemp2 = extTherm2.temp();
  extTemp3 = extTherm3.temp();
  extTemp4 = extTherm4.temp();
  gCount1 = readGeiger1();
  gCount2 = readGeiger2();
  solar1V = solar1.voltage();
  solar2V = solar2.voltage();
  solar3V = solar3.voltage();
  solar4V = solar4.voltage();
  
  battV = battMeter.voltage();
  
  // Drive heaters
  heat1State = battHeater1.cycle();
  heat2State = battHeater2.cycle();
  heat3State = intHeater1.cycle();
  heat4State =  intHeater2.cycle();
  
  datalog.print( lastCycleTime );
  datalog.print( ", " );
  datalog.print( timestamp );
  datalog.print( ", " );
  datalog.print( pressure );
  datalog.print( ", " );
  datalog.print( uv );
  datalog.print( ", " );
  datalog.print( compassTheta );
  datalog.print( ", " );
  datalog.print( compassPhi );
  datalog.print( ", " );
  datalog.print( compassMag );
  datalog.print( ", " );
  datalog.print( accelX );
  datalog.print( ", " );
  datalog.print( accelY );
  datalog.print( ", " );
  datalog.print( accelZ );
  datalog.print( ", " );
  datalog.print( battTemp1 );
  datalog.print( ", " );
  datalog.print( battTemp2 );
  datalog.print( ", " );
  datalog.print( intTemp1 );
  datalog.print( ", " );
  datalog.print( intTemp2 );
  datalog.print( ", " );
  datalog.print( extTemp1 );
  datalog.print( ", " );
  datalog.print( extTemp2 );
  datalog.print( ", " );
  datalog.print( extTemp3 );
  datalog.print( ", " );
  datalog.print( extTemp4 );
  datalog.print( ", " );
  datalog.print( heat1State ? "ON" : "OFF" );
  datalog.print( ", " );
  datalog.print( heat2State ? "ON" : "OFF" );
  datalog.print( ", " );
  datalog.print( heat3State ? "ON" : "OFF" );
  datalog.print( ", " );
  datalog.print( heat4State ? "ON" : "OFF" );
  datalog.print( ", " );
  datalog.print( gCount1 );
  datalog.print( ", " );
  datalog.print( gCount2 );
  datalog.print( ", " );
  datalog.print( battV );
  datalog.print( ", " );
  datalog.print( solar1V );
  datalog.print( ", " );
  datalog.print( solar2V );
  datalog.print( ", " );
  datalog.print( solar3V );
  datalog.print( ", " );
  datalog.println( solar4V );
  
  // Wait until the cycle times out
  while( millis() - lastCycleTime < CYCLE_TIMEOUT )
  {
    
    if( digitalRead( TEST_BUTTON_PIN ) == HIGH )
    {
      runDiagnostics();
    } // end if
    
    delayMicroseconds( SLEEP_TIME );
  } // end while
  
} // end loop

void runDiagnostics()
{
  char timestamp[ TIMESTAMP_LENGTH ] = "";
  float compassTheta, compassPhi, compassMag;
  clock.timestamp( &timestamp[ 0 ] );
  compass.data( &compassTheta, &compassPhi, &compassMag );
  
  // Clear screen
  lcd.print( "?f" );
  // Output diagnostic data
  lcd.print( "B1T " );
  lcd.print( battTherm1.temp() );
  lcd.print( " B2T " );
  lcd.print( battTherm1.temp() );
  lcd.print( "?n" );
  lcd.print( "Ax" );
  lcd.print( accel.accelerationX() );
  lcd.print( "Mg" );
  lcd.print( compassMag );
  lcd.print( " S" );
  lcd.print( solar1.voltage() );
  lcd.print( "?n" );
  lcd.print( "Alt" );
  lcd.print( alt.isRunning() ? "OK" : "NK" );
  lcd.print( " UV" );
  lcd.print( uvSen.intensity() );
  lcd.print( "B" );
  lcd.print( battMeter.voltage() );
  lcd.print( "V" );
  lcd.print( "?n" );
  lcd.print( timestamp );
  
  delay( DIAGNOSTIC_TIME );
} // end runDiagnostics

unsigned long readGeiger1()
{
  unsigned long gc1 = geigerCount1;
  geigerCount1 = 0;
  
  return gc1;
} // end readGeiger1

unsigned long readGeiger2()
{
  unsigned long gc2 = geigerCount2;
  geigerCount2 = 0;
  
  return gc2;
} // end readGeiger2
