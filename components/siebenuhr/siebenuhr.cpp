#include "siebenuhr.h"
#include "esphome/core/log.h"

namespace esphome {
namespace siebenuhr {

static const char *const TAG = "siebenuhr.display";

void Siebenuhr::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Siebenuhr Component...");
  pinMode(pin_, OUTPUT);
}

void Siebenuhr::loop() {
  static unsigned long last_toggle = 0;
  unsigned long now = millis();
  if (now - last_toggle > 1000) {  // Toggle every second
    m_state = !m_state;
    digitalWrite(pin_, m_state);
    ESP_LOGD(TAG, "Toggling LED: %s", state_ ? "ON" : "OFF");
    last_toggle = now;
  }
}

}  // namespace siebenuhr
}  // namespace esphome
