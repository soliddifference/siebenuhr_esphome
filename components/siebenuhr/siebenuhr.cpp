#include "siebenuhr.h"

namespace esphome::siebenuhr {

    const char *const TAG = "🚀 Siebenuhr";

    SiebenuhrClock *global_siebenuhr_clock = nullptr;
    
    void SiebenuhrClock::setup() 
    {
        ESP_LOGI(TAG, "Siebenuhr ESPHOME v%s CORE v%s Build Date: %s %s", SIEBENUHR_ESPHOME_VERSION, SIEBENUHR_CORE_VERSION, __DATE__, __TIME__);    

        global_siebenuhr_clock = this;

        m_controller.initialize(m_type);

        this->set_interval(1, [this]() { this->loop(); });
    }

    void SiebenuhrClock::loop() 
    {
        // ESP_LOGI(TAG, "loop");

        #ifdef WITH_WIFI
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
        #endif

        m_controller.update();
    }

    light::LightTraits SiebenuhrClock::get_traits() 
    {
        auto traits = light::LightTraits();
        traits.set_supported_color_modes({light::ColorMode::BRIGHTNESS});
        traits.set_supported_color_modes({light::ColorMode::RGB});
        return traits;
    }

    void SiebenuhrClock::write_state(light::LightState *state) 
    {

    }

    void SiebenuhrClock::dump_config() 
    { 
        ESP_LOGCONFIG(TAG, "Siebenuhr"); 
    }

    void SiebenuhrClock::set_type(int type)
    {
        if (type == 1) 
        {
            m_type = siebenuhr_core::ClockType::CLOCK_TYPE_MINI;
        }
        ESP_LOGI(TAG, "SET: clocktype=%s", m_type == siebenuhr_core::ClockType::CLOCK_TYPE_MINI ? "MINI" : "REGULAR");
    }

    void SiebenuhrClock::set_text(std::string text) 
    {
        ESP_LOGI(TAG, "SET: text=%s", text.c_str());
        m_controller.setText(text);
    }

    #ifdef WITH_WIFI
    void SiebenuhrClock::set_time_component(esphome::time::RealTimeClock *timeComponent) 
    {
        m_timeComponent = timeComponent;
        ESP_LOGI(TAG, "SET: TimeComponent");
    }
    #endif

}