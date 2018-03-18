/*
 * RGB LED Fireplace driver code 1.0
 * for ESP8266 module - Basic version (no WiFi control in this version)
 * 
 * LED strip is WS2812 strip, 60 LEDs/meter, 1 meter long (total: 60 LEDs)
 * Power consumption is about 600-700 mA for current code, beware!
 * 
 * Connect the WS2812 data pin (D) to pin "D7" of the ESP8266 module
 * Connect WS2812 +5V to USB power (+5V) and GND to GND
 * Can be easily adapted also for Arduino
 * Tested with NodeMCU 0.9 (ESP-12)
 */

#include <Adafruit_NeoPixel.h>
#include "TickerScheduler.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif

int value = 0;
TickerScheduler ticker(2); // initialize for 2 tasks

#define RGBPIN         13 // digital 13 on ESP8266 is pin marked with "D7" on PCB!
#define NUMPIXELS      60 // 60 pixels on RGB LED strip
#define TASK_ANIM 0
#define REDUCED_STEPS 1200 // reduced current consumption for 1200 steps (1200 * 55ms)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, RGBPIN, NEO_GRB + NEO_KHZ800);

#define EEPROM_SIZE 1024
#define COLORSCALE 8 // 5 bit: 0-31 scaled to x8
unsigned int gAnimationMode = 0;
unsigned int gAnimationIdx = 0;

#define NUMPALETTE 4
int rcolors[NUMPALETTE] = {80,120,170,200}; // the 4 possible color intensities for red (and yellow)

// Do animation gets called every 50 milliseconds
// It starts up with using about half of the LEDs for the first minute,
// then lights up all. The reason for this is to keep power consumption below 500 mA if you 
// want to plug it into a PC USB port and update it, you have 1 minute to do so. Some USB
// ports shut down communication if current draw is above > 500 mA
void doAnimation( void *dummy) {
  if (gAnimationMode == 0) return;  
  gAnimationIdx++;
  if ((gAnimationIdx > REDUCED_STEPS) && gAnimationMode == 1) gAnimationMode = 2; // reduced mode for 1 minute
  if (gAnimationIdx > 60000) gAnimationIdx = 0;
  int color = gAnimationIdx;

  for ( int c = 0; c < NUMPIXELS; c++ ) {
    if (gAnimationMode == 1 && c == 20) break; // reduced mode for 1 minute, only 20 LEDs
    int cred, cgreen, cblue;

    int ra = random(20); // get a random number 0..19
    if (ra > 7) continue; // if > 7 then skip changing this pixel
    if (ra >= NUMPALETTE) ra = NUMPALETTE - 1; // only 4 values in rcolors array [0..3]
    cred = rcolors[ra]; // select a color intensity from the table
    cgreen = 0;
    cblue = 0;
    if ((c == 10 || c == 25) ) {
      // have 2 yellow pixels (at position 10 and 25), this is to light those
      cred -= 20;
      cgreen = cred;
    }
    pixels.setPixelColor(c, pixels.Color(cred, cgreen, cblue));
  }
  pixels.show();
}

void setupAnimation() {
  ticker.add(TASK_ANIM, 50, doAnimation, NULL);
  ticker.disable(TASK_ANIM);
}

void stopAnimationMode() {
  ticker.disable(TASK_ANIM);
  gAnimationMode = 0;
}

void eraseScreen() {
  for (int c = 0; c < NUMPIXELS; c++) {
      pixels.setPixelColor(c, pixels.Color(0, 0, 0));
  }
  pixels.show();
}


void startAnimationMode( int md ) {
  gAnimationMode = md;
  gAnimationIdx = 0;
  randomSeed(millis());
  ticker.enable(TASK_ANIM);  
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(115200);
  pixels.setPixelColor(0, pixels.Color(128, 0, 0));
  pixels.show();
  setupAnimation();
  startAnimationMode(1);
}

void loop() {
  ticker.update();
}

