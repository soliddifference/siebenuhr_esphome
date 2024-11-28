#pragma once

#include "esphome.h"
#include <FastLED.h>

class SiebenUhrComponent : public Component {
 public:
  void setup() override;
  void loop() override;

 private:
  static const int LED_PIN = 5; // Change this to your GPIO pin
  static const int NUM_LEDS = 1;
  CRGB leds[NUM_LEDS];
};