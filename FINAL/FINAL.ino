
//Light libraries
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

//math library
#include <math.h>

//Light initializing. 
uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels
const int light_low = 3;
const int light_high = 24;
const int numlights = 25;
const int steady_lights = 3;
int light_val;

//gas sensor initializing
int gas_low = 0;
int gas_high = 1023;
int gas_tune_low = 30;
int gas_tune_high = 200;
int raw_gas_val;
#define gasSensorPin  3

//color and delay
uint32_t good_color = 511; //RGB 24 bit color 0x00BFFF. 701?
uint32_t bad_color = 767; //0xFF4500
uint32_t curr_color = 511; 
uint8_t wait = 500; //delay between readings

//wind sensor initializing
#define analogPinForRV    4   
#define analogPinForTMP   5

int TMP_Therm_ADunits;  //temp termistor value from wind sensor
float RV_Wind_ADunits;    //RV output from wind sensor 
float RV_Wind_Volts;
unsigned long lastMillis;
int TempCtimes100;
float zeroWind_volts = 1.87; //change based on calibtration in "zero wind" conditions
float WindSpeed_MPH;
int windRD_low = 420; //change the range based on testing with desired low and high wind
int windRD_high = 500;
uint8_t wind_val;

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin);

void setup() {
  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
  Serial.begin(9600); //begin serial communication
}

void loop()
{
  
  
//Wind speed calcs

  TMP_Therm_ADunits = analogRead(analogPinForTMP);
  RV_Wind_ADunits = analogRead(analogPinForRV);
  RV_Wind_Volts = (RV_Wind_ADunits *  0.0048828125);
  WindSpeed_MPH =  pow(((RV_Wind_Volts - zeroWind_volts) /.2300) , 2.7265);
  /*Serial.print("  TMP volts ");
  Serial.print(TMP_Therm_ADunits * 0.0048828125);
    
  Serial.print(" RV volts ");
  Serial.print((float)RV_Wind_Volts);*/
  
  Serial.print(" RV AD ");
  Serial.print((float)RV_Wind_ADunits);
  
  /*Serial.print("\t  TempC*100 ");
  Serial.print(TempCtimes100 );
  
  Serial.print("   ZeroWind volts ");
  Serial.print(zeroWind_volts);
  
  Serial.print("   WindSpeed MPH ");
  Serial.print((float)WindSpeed_MPH);*/
  
//wind speed mapping to light array
  wind_val = map((int)RV_Wind_ADunits, windRD_low, windRD_high, light_low, light_high);
  wind_val = constrain(wind_val, light_low, light_high);
  Serial.print("  Wind Value ");
  Serial.println((int)wind_val);
//gas sensor reading and tuning
  
  raw_gas_val = analogRead(gasSensorPin);
  //gas_tune();
  Serial.print("  raw gas val ");
  Serial.print(raw_gas_val);
  if (raw_gas_val < gas_tune_low){
    raw_gas_val = gas_tune_low;
  }
  else if(raw_gas_val > gas_tune_high){
    raw_gas_val = gas_tune_high;
  }  
//update color based on gas
  curr_color = map(raw_gas_val, gas_tune_low, gas_tune_high, good_color, bad_color);
  constrain(curr_color, good_color, bad_color);
  Serial.print("  Color  ");
  Serial.println((int) curr_color);  // read analog input pin 0
  convertColor();
  update_light_height();

  /*for(int i = 0; i<wind_val; i++){
    strip.setPixelColor(i, curr_color);
  }
  strip.show();
 for(int i = 0; i<wind_val; i++){
    strip.setPixelColor(i, 0x000000);
  }
  */
  delay(wait);
}

void convertColor()
{
  byte redIntensity;
  byte greenIntensity;
  byte blueIntensity;

  if (curr_color <= 255)          // zone 1
  {
    redIntensity = 255 - curr_color;    // red goes from on to offcurr_colorcurr_color
    greenIntensity = curr_color;        // green goes from off to on
    blueIntensity = 0;             // blue is always off
  }
  else if (curr_color <= 511)     // zone 2
  {
    redIntensity = 0;                     // red is always off
    greenIntensity = 255 - (curr_color - 256); // green on to off
    blueIntensity = (curr_color - 256);        // blue off to on
  }
  else // color >= 512       // zone 3
  {
    redIntensity = (curr_color - 512);         // red off to on
    greenIntensity = 0;                   // green is always off
    blueIntensity = 255 - (curr_color - 512);  // blue on to off
  }
  
  //convert to hex
  curr_color = redIntensity;
  curr_color <<= 8;
  curr_color |= greenIntensity;
  curr_color <<= 8;
  curr_color |= blueIntensity;
}
  

void update_light_height()
{ //turn all except 3 off
  for(int i=0; i <= light_high; i++){
    strip.setPixelColor(i,0x000000);
  }
  for(int i=light_high; i >light_high - steady_lights; i--){
    strip.setPixelColor(i,curr_color);
  }
  //turn wind_val on
  for (int i=light_high - steady_lights; i > light_high - steady_lights - wind_val; i--)
 {
  strip.setPixelColor(i, curr_color);
 }
strip.show();
}

void gas_tune()
{
  
  if (raw_gas_val < gas_tune_low)
  {
    gas_tune_low = raw_gas_val;
  }

  // We also initialized "high" to be 0. We'll save anything
  // we read that's higher than that:
  
  if (raw_gas_val > gas_tune_high)
  {
    gas_tune_high = raw_gas_val;
  }
  
  // Once we have the highest and lowest values, we can stick them
  // directly into the map() function. No manual tweaking needed!
  
  // One trick we'll do is to add a small offset to low and high,
  // to ensure that the LED is fully-off and fully-on at the limits
  // (otherwise it might flicker a little bit).
  
  raw_gas_val = map(raw_gas_val, gas_tune_low+30, gas_tune_high-30, 0, 255);
  raw_gas_val = constrain(raw_gas_val, gas_tune_low, gas_tune_high);
  Serial.print("  gas tune low ");
  Serial.print(gas_tune_low);
  Serial.print("  gas tune high ");
  Serial.print(gas_tune_high);
  
}
