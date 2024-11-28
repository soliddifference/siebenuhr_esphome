#include "siebenuhr_esphome.h"

void SiebenUhrComponent::setup() {
  // Initialize FastLED
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}

void SiebenUhrComponent::loop() {
  // Example: Cycle LED through red, green, and blue
  static int state = 0;

  switch (state) {
    case 0:
      leds[0] = CRGB::Red;
      break;
    case 1:
      leds[0] = CRGB::Green;
      break;
    case 2:
      leds[0] = CRGB::Blue;
      break;
  }

  FastLED.show();
  delay(500);

  state = (state + 1) % 3;
}