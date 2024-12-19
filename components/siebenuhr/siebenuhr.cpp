#include <Arduino.h>
#include "siebenuhr.h"

#include <siebenuhr_controller.h>

namespace esphome::siebenuhr {

static const char *const TAG = "siebenuhr";

void SiebenuhrDisplay::set_mode(int mode)
{
    m_mode = mode;
    ESP_LOGCONFIG(TAG, "SiebenUhr Mode: %s", m_mode == 1 ? "MINI" : "REGULAR");
}

void SiebenuhrDisplay::setup() 
{
    ESP_LOGCONFIG(TAG, "Setting up Siebenuhr Component...");

    siebenuhr_core::Controller *pController = siebenuhr_core::Controller::getInstance();
    if (pController != nullptr) 
    {
        pController->initialize(siebenuhr_core::Controller::ClockType::Mini);
        pController->setHeartbeatEnabled(true);
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