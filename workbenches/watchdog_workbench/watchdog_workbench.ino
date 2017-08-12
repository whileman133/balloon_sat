// Watchdog.ino
//
// Written by Wes Hileman on 25 Sep 2014
//
// Purpose: To demonstrate the function of the ATmega328 watchdog
// timer on the Arduino Uno board.
//
// ALGORITHM FOLLOWS
//
//  setup:
//    initialize watchdog timer
//    setup pins
//  end setup
//
//  loop:
//    reset watchdog timer
//    pulse piezo buzzer
//    if test button pressed
//      drive piezo continuously
//      infinite loop
//  end loop

#include <avr/wdt.h>

#define BUTTON_PIN 2
#define PIEZO_PIN 9

void setup()
{
  // initialize watchdog timer
  setupWdt();
  // setup pins
  pinMode(PIEZO_PIN, OUTPUT);  // pwm for piezo
  pinMode(BUTTON_PIN, INPUT);
}

void loop()
{
  // reset watchdog timer
  wdt_reset();
  // pulse piezo buzzer
  pulse();
  
  if( digitalRead(BUTTON_PIN) == HIGH ) 
  {
    // drive peizo continuously
    flatline();
    // enter infinite loop
    while (true);
  }
  
  // delay to put time between pulses
  delay(300);
}

void pulse(void)
{
  // PWM
  // 255 = 100% duty cycle
  // 0 = 0% duty cycle
  // 200 =~ 80% duty cycle
  analogWrite(PIEZO_PIN, 200);
  delay(300);
  analogWrite(PIEZO_PIN, LOW);
}

void flatline(void)
{
  analogWrite(PIEZO_PIN, 200);
}
 
void setupWdt(void)
{
  cli();
  wdt_reset();
  WDTCSR |= B00011000;
  WDTCSR = B01101000;
  sei();
}
