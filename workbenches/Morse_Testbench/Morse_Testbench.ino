// Morse_Testbench.ino
//
// Written by Wes Hileman on 15 March 2015
//
// Purpose: To beep out Morse code on an FM transmitter.

#define BAUD 9600

// This is the pin that enables/disables the transmitter 
// (connect to ENABLE on the transmitter).
#define ENABLE_PIN 4

// A dot's duration
#define DOT_TIME 200 // millisec
// A dash's duration
#define DASH_TIME 3*DOT_TIME // millisec
// The time between dots and dashes
#define DD_DELAY_TIME DOT_TIME // millisec
// The time between characters
#define CHAR_DELAY_TIME 3*DOT_TIME // millisec
// The time between words
#define WORD_DELAY_TIME 7*DOT_TIME // millisec

void setup() 
{ 
  // Initialize serial communiction
  Serial.begin(BAUD);
  pinMode( ENABLE_PIN, OUTPUT );
  digitalWrite( ENABLE_PIN, LOW );
}

void loop() 
{
  // Prompt user for message to transmit
  Serial.println("Type message");
  while(Serial.available() == 0);
  
  // Transmit the message
  while(Serial.available() > 0) {
    char letter = Serial.read();
    translateLetter(letter);
  }
  Serial.println("Complete!");
  Serial.println();
}

void dot() 
{
  digitalWrite( ENABLE_PIN, HIGH );
  delay( DOT_TIME );
  analogWrite( ENABLE_PIN, LOW );
  delay( DD_DELAY_TIME );
} // end dot

void dash() 
{
  digitalWrite( ENABLE_PIN, HIGH );
  delay( DASH_TIME );
  analogWrite( ENABLE_PIN, LOW );
  delay( DD_DELAY_TIME );
} // end dash

void translateLetter(char letter) {
  switch (letter) {
  case 'a':
  case 'A':
    dot(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'b':
  case 'B':
    dash(); dot(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'c':
  case 'C':
    dash(); dot(); dash(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'd':
  case 'D':
    dash(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'e':
  case 'E':
    dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'f':
  case 'F':
    dot(); dot(); dash(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'g':
  case 'G':
    dash(); dash(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'h':
  case 'H':
    dot(); dot(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'i':
  case 'I':
    dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'j':
  case 'J':
    dot(); dash(); dash(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'k':
  case 'K':
    dash(); dot(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'l':
  case 'L':
    dot(); dash(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'm':
  case 'M':
    dash(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'n':
  case 'N':
    dash(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'o':
  case 'O':
    dash(); dash(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'p':
  case 'P':
    dot(); dash(); dash(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 'q':
  case 'Q':
    dash(); dash(); dot(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'r':
  case 'R':
    dot(); dash(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 's':
  case 'S':
    dot(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case 't':
  case 'T':
    dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'u':
  case 'U':
    dot(); dot(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'v':
  case 'V':
    dot(); dot(); dot(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'w':
  case 'W':
    dot(); dash(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'x':
  case 'X':
    dash(); dot(); dot(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'y':
  case 'Y':
    dash(); dot(); dash(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case 'z':
  case 'Z':
    dash(); dash(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case ' ':
    delay( WORD_DELAY_TIME - CHAR_DELAY_TIME );
    break;
  case '1':
    dot(); dash(); dash(); dash(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case '2':
    dot(); dot(); dash(); dash(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case '3':
    dot(); dot(); dot(); dash(); dash();
    delay( CHAR_DELAY_TIME );
    break;
  case '4':
    dot(); dot(); dot(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case '5':
    dot(); dot(); dot(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case '6':
    dash(); dot(); dot(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case '7':
    dash(); dash(); dot(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case '8':
    dash(); dash(); dash(); dot(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case '9':
    dash(); dash(); dash(); dash(); dot();
    delay( CHAR_DELAY_TIME );
    break;
  case '0':
    dash(); dash(); dash(); dash(); dash();
    delay( CHAR_DELAY_TIME );
    break;
   
   case '.':
     dot(); dash(); dot(); dash(); dot(); dash();
     delay( CHAR_DELAY_TIME );
     break;
     
  default:
    delay( WORD_DELAY_TIME - CHAR_DELAY_TIME );
  }
}
