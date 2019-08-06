#particlewave

This Arduino project maps values from a CO sensor to an LED light strip, used to create these visualizations:
https://www.flickr.com/photos/long_exposure/sets/72157660025495836/
Below is the hardware used. 

CO Sensor and echem board

A sensor at head height on the stick continually measures carbon monoxide (abbreviated CO). Special electrolyte goo inside the sensor reacts with the CO in the atmosphere and changes electrical current passing through it. The sensor sends information about the CO level to a circuit board called an echem328. It was designed to turn the readings from the sensor into useful information. 

The sensor and echem are very good at measuring small concentrations of CO in the atmosphere. They can detect just a few CO molecules in a sea of billions of other gas molecules. In fact, they can detect you too! Try breathing on the sensor and see if the lights respond! 

Gas sensors can be fussy. The CO sensor has a long warm-up period where it reads higher than normal values before settling down. It is also temperature dependant and can overheat if left in the sun. Ideally, a sensor should have a constant flow of air across it. This is usually done with some kind of fan. If you get an air quality sensor, treat it well!

For more information on the sensor visit Alphasense: http://www.alphasense.com

For more information on the echem board visit: http://circuitsci.com/


Arduino microcontroller

The echem sends information to an Arduino circuit board. Arduino is an open source microcontroller designed to be used by everyone from experts to amateurs. Arduinos can be programmed to send and receive information from other electronic devices. Our Arduino board runs a program which converts the values it receives from the sensor into a number between 0 and 60. The Arduino then sends a message to the lights to tell which of the 60 LEDs to light up and what color they should be.

The Arduino uses a software serial port to communicate between the Arduino and echem. This allows us to use the real serial connection on the Arduino for data logging and troubleshooting. The Arduino is powered by 4 AA rechargeable batteries. It shares some of that power with the echem.

For more information on Arduino, check to see if your local maker space has a class or visit: https://www.arduino.cc/

Dotstar LED light strip

The Arduino sends instructions to a strip of 60 small lights called RGB LEDs. The lights are wired together, but each light can be individually controlled, changing color and intensity. The Dotstars are designed for light painting and other applications where a fast refresh rate is necessary. A collection of code called a library makes it easy to control the Dotstars and create fading, flashing, and many other effects.

It is possible to power the Dotstars directly from an Arduino, but it can be difficult for the microcontroller to provide enough current for a long strip. We use an auxiliary battery pack with 4 rechargeable AA batteries, which provides just the right amount of voltage to the lights. Non-rechargeable batteries need an additional resistor to prevent the extra voltage from zapping the LEDs! The pack is attached with Velcro for easy loading.  

For more information on DotStars, check out: https://learn.adafruit.com/adafruit-dotstar-leds/overview

