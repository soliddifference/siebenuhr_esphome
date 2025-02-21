#include "siebenuhr.h"

namespace esphome::siebenuhr
{
    static const char *const TAG = "🚀 Siebenuhr";

    void SiebenuhrClock::set_mode(int mode)
    {
        m_mode = mode;
        ESP_LOGI(TAG, "Mode: %s", m_mode == 1 ? "MINI" : "REGULAR");
    }

    void SiebenuhrClock::set_time_component(esphome::time::RealTimeClock *timeComponent) 
    {
        m_timeComponent = timeComponent;
        ESP_LOGI(TAG, "setTimeComponent");
    }

    void SiebenuhrClock::set_text(const std::string &text)
    {
        ESP_LOGI("🚀 Siebenuhr", "New Text Set: %s", text.c_str());        
        // m_controller.setText(text);
    }

    void SiebenuhrClock::setup()
    {
        ESP_LOGI(TAG, "Initializing");
        m_controller.initialize();

        this->set_interval(1, [this]() { this->loop(); });
    }

    void SiebenuhrClock::loop()
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