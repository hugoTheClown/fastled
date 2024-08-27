#include <FastLED.h>
#define NUM_LEDS 49
#define DATA_PIN 3
CRGB leds[NUM_LEDS];

void setup() {
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

int red = 0;
int green = 10;
int blue = 10;

void loop() {
   // Move a single white led 
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB( 0, 0, red);
      // leds[whiteLed] = CRGB::SteelBlue;
      
      red= ++red % 255;

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(10);

      // Turn our current led back to black for the next loop around
      leds[whiteLed] = CRGB::Black;
   }
}
