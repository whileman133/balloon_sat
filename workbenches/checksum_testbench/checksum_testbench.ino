void setup() 
{
  Serial.begin( 9600 );
}

void loop() 
{
  void (&function)(unsigned long) = Serial.begin;
  
  char str[ 50 ] = "Checksum Test!";
  char chk = parityByte( str );
  char dat[ 52 ];
  sprintf( dat, "%s*%c", str, chk );
  
  Serial.println( dat );
  //dat[ 0 ] = 'c';
  Serial.println( testParity( dat ) ? "ACK" : "NACK" );
  delay( 1000 );
}

// Compute a parity-byte checksum for a string
char parityByte( char *string )
{
  char checksum = *string, c;
  
  while( ( c = *(++string) ) != '\0' )
  {
    checksum = checksum ^ c;
  } // end while
  
  return checksum;
} // end parityByte

bool testParity( char *string )
{
  char result = *string, c;
  
  while( ( c = *(++string) ) != '\0' )
  {
    // Parity byte indicator
    if( c == '*' )
    {
      continue;
    } // end if
    
    result = result ^ c;
  } // end while
  
  return !result;
} // end testParity
