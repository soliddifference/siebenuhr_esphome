#include "siebenuhr.h"

namespace esphome::siebenuhr
{
    static const char *const TAG = "siebenuhr";

    void SiebenuhrDisplay::setMode(int mode)
    {
        m_mode = mode;
        ESP_LOGCONFIG(TAG, "SiebenUhr Mode: %s", m_mode == 1 ? "MINI" : "REGULAR");
    }

    void SiebenuhrDisplay::setTimeComponent(esphome::time::RealTimeClock *timeComponent) 
    {
        m_timeComponent = timeComponent;
    }

    void SiebenuhrDisplay::setText(const std::string &text)
    {
        ESP_LOGI("Siebenuhr", "New Text Set: %s", text.c_str());        
        m_controller.setText(text);
    }

    void SiebenuhrDisplay::setup()
    {
        ESP_LOGCONFIG(TAG, "Initializing Siebenuhr...");
        m_controller.initialize();
    }

    void SiebenuhrDisplay::loop()
    {

        if (m_timeComponent && m_timeComponent->now().is_valid()) 
        {
            auto current_time = m_timeComponent->now();
            int hours = current_time.hour;
            int minutes = current_time.minute;
            if (m_currentHours != hours || m_currentMinutes != minutes)
            {
                m_controller.setTime(hours, minutes);
                m_currentHours = hours;
                m_currentMinutes = minutes;
            }
        }

        m_controller.update();
    }
}