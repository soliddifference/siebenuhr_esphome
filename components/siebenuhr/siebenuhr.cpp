#include <Arduino.h>
#include "siebenuhr.h"

#include "siebenuhr_controller.h"

namespace esphome::siebenuhr {

static const char *const TAG = "siebenuhr";

void SiebenuhrDisplay::set_mode(int mode)
{
    m_mode = mode;
    ESP_LOGD(TAG, "SiebenUhr Mode: %s", m_mode == 1 ? "MINI" : "REGULAR");
}

void SiebenuhrDisplay::setup() 
{
    ESP_LOGCONFIG(TAG, "Setting up Siebenuhr Component...");
    pinMode(m_pin, OUTPUT);

    siebenuhr_core::Controller *pController = siebenuhr_core::Controller::getInstance();
    if (pController != nullptr) 
    {
        pController->initialize();
    }
}

void SiebenuhrDisplay::loop() 
{
    siebenuhr_core::Controller *pController = siebenuhr_core::Controller::getInstance();
    if (pController != nullptr) 
    {
        pController->update();
    }

    static unsigned long last_toggle = 0;
    unsigned long now = millis();
    if (now - last_toggle > 1000) {  // Toggle every second
        m_state = !m_state;
        digitalWrite(m_pin, m_state);
        ESP_LOGD(TAG, "Toggling LED: %s (%s)", m_state ? "ON" : "OFF", m_mode == 1 ? "MINI" : "REGULAR");
        last_toggle = now;
    }
}

}