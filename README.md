# Arduino / ESP driven RGB LED Fireplace

You can find the basic code with comments in the ESPFireplace11Base directory.
This is the most basic, not WiFi enabled code, it just drives the LEDs.
You need Arduino IDE and also the ESP8266 Core for Arduino installed, see here:
https://github.com/esp8266/Arduino

I used a NodeMCU (0.9) board.
The code should be easily adaptable also to Arduino boards (UNO/Nano/Mini/etc).

The connection to the RGB LED strip (WS2812B, 60 LEDs/meter, 1 meter) is in the code comments, but it's very easy:
The LED strip requires 5V power, while the NodeMCU outputs a 3.3V signal, but this was enough in practice.
It is also recommended to connect a capacitor (1000 uF, 6.3V or higher) close to the RGB LED strip, across the + and GND wires.

NodeMCU (ESP) pin | WS2812 LED pion
------------ | -------------
D7 | D
VCC (USB PWR) | +5V
GND | GND

It is also recommended to add a capacitor close to the LED strip, 

For any ideas for improvement, feedback, patches, email to:
johnzero AT johnzero DOT hu
