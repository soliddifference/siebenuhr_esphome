#include "controller.h"

namespace esphome::siebenuhr
{
    void Controller::initialize()
    {
        m_display = siebenuhr_core::Display::getInstance();
        if (m_display != nullptr)
        {
            m_display->initialize(siebenuhr_core::ClockType::CLOCK_TYPE_REGULAR, 4);
            m_display->setHeartbeatEnabled(true);
        }
    }

    void Controller::update()
    {
        if (m_display == nullptr)
        {
            // something went wrong in the setup!?
            m_display = siebenuhr_core::Display::getInstance();
        }

        m_display->update();
    }

    void Controller::setText(const std::string &text)
    {
        if (m_display == nullptr)
        {
            // something went wrong in the setup!?
            m_display = siebenuhr_core::Display::getInstance();
        }

        m_display->setText(text);
    }

    void Controller::setTime(int hours, int minutes) 
    {
        if (m_display == nullptr)
        {
            // something went wrong in the setup!?
            m_display = siebenuhr_core::Display::getInstance();
        }

        char formatted_time[5];  // Buffer for "HHMM" + null terminator
        snprintf(formatted_time, sizeof(formatted_time), "%02d%02d", hours, minutes);
        m_display->setText(std::string(formatted_time));

        ESP_LOGI("Controller", "Time set: %02d:%02d", hours, minutes);
    }    
}