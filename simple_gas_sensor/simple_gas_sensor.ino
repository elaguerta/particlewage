const int gasSensor =0;
int gasvalue;

void setup()
{
  Serial.begin(9600);      // sets the serial port to 9600
}

void loop()
{
  gasvalue = analogRead( gasSensor);       // read analog input pin 0
  Serial.println(gasvalue, DEC);  // prints the value read
  delay(100);                        // wait 100ms for next reading
}

