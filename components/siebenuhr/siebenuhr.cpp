#include <Arduino.h>
#include "siebenuhr.h"

// #include "siebenuhr_controller.h"
#include <siebenuhr_controller.h>

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

    siebenuhr_core::Controller *pController = siebenuhr_core::Controller::getInstance();
    if (pController != nullptr) 
    {
        pController->initialize(m_mode);
        pController->setInterval(1000);
        pController->setLEDPin(2);
    }
}

void SiebenuhrDisplay::loop() 
{
    siebenuhr_core::Controller *pController = siebenuhr_core::Controller::getInstance();
    if (pController != nullptr) 
    {
        pController->update();
    }
}

}