#include "siebenuhr.h"

namespace esphome::siebenuhr {

    const char *const TAG = "🚀 Siebenuhr";

    SiebenuhrClock *global_siebenuhr_clock = nullptr;
    
    void SiebenuhrClock::setup() 
    {
        ESP_LOGI(TAG, "Siebenuhr ESPHOME v%s CORE v%s Build Date: %s %s", SIEBENUHR_ESPHOME_VERSION, SIEBENUHR_CORE_VERSION, __DATE__, __TIME__);    

        global_siebenuhr_clock = this;

        ESP_LOGI(TAG, "Setting up Siebenuhr Clock...");       
        
        // Initialize the controller with the stored personality
        m_controller.initialize(m_type);
        m_controller.setPersonality(m_personality);

        // Update the clock every milliseconds
        this->set_interval(1, [this]() { this->loop(); });
    }

    void SiebenuhrClock::loop() 
    {
        #ifdef WITH_WIFI
        if (m_timeComponent && m_timeComponent->now().is_valid())
        {
            auto current_time = m_timeComponent->now();
            if (true)
            {
                if (m_currentHours != current_time.hour || m_currentMinutes != current_time.second)
                {
                    m_controller.setTime(current_time.minute, current_time.second);
                    m_currentHours = current_time.minute;
                    m_currentMinutes = current_time.second;
                }
            }
            else
            {
                if (m_currentHours != current_time.hour || m_currentMinutes != current_time.minute)
                {
                    m_controller.setTime(current_time.hour, current_time.minute);
                    m_currentHours = current_time.hour;
                    m_currentMinutes = current_time.minute;
                }
            }
        }
        #endif

        m_controller.update();
    }

    light::LightTraits SiebenuhrClock::get_traits() 
    {
        light::LightTraits traits;
        traits.set_supported_color_modes({light::ColorMode::RGB, light::ColorMode::BRIGHTNESS});
        return traits;        
    }

    void SiebenuhrClock::write_state(light::LightState *state) 
    {
        m_controller.setLightState(state);

        // Handle power (on/off)
        bool is_on =  state->current_values.is_on();
        m_controller.setPower(is_on);

        // Handle brightness (0.0 - 1.0 float)
        float brightness = state->current_values.get_brightness();
        m_controller.setBrightness(static_cast<int>(brightness * 255.0f));

        // Handle color (RGB values between 0.0 and 1.0)
        float red = state->current_values.get_red();
        float green = state->current_values.get_green();
        float blue = state->current_values.get_blue();
        m_controller.setColor(static_cast<int>(red * 255.0f), static_cast<int>(green * 255.0f), static_cast<int>(blue * 255.0f));
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

    void SiebenuhrClock::set_auto_brightness(bool enable)
    {
        m_controller.setAutoBrightnessEnabled(enable);
    }

    void SiebenuhrClock::set_power_monitoring(bool enable)
    {
        m_controller.setPowerMonitoringEnabled(enable);
    }

    void SiebenuhrClock::set_text(std::string text) 
    {
        m_controller.setText(text);
    }

    void SiebenuhrClock::set_personality(std::string personality)
    {
        ESP_LOGI(TAG, "SET: Personality: %s", personality.c_str());
        
        if (personality == "SOLIDCOLOR")
        {
            m_personality = siebenuhr_core::PersonalityType::PERSONALITY_SOLIDCOLOR;
        }
        else if (personality == "COLORWHEEL")
        {
            m_personality = siebenuhr_core::PersonalityType::PERSONALITY_COLORWHEEL;
        }
        else if (personality == "RAINBOW")
        {
            m_personality = siebenuhr_core::PersonalityType::PERSONALITY_RAINBOW;
        }
        else if (personality == "MOSAIK")
        {
            m_personality = siebenuhr_core::PersonalityType::PERSONALITY_MOSAIK;
        }
        else
        {
            ESP_LOGW(TAG, "Unknown personality type: %s, defaulting to SOLIDCOLOR", personality.c_str());
            m_personality = siebenuhr_core::PersonalityType::PERSONALITY_SOLIDCOLOR;
        }
    }

    #ifdef WITH_WIFI
    void SiebenuhrClock::set_time_component(esphome::time::RealTimeClock *timeComponent) 
    {
        m_timeComponent = timeComponent;
        ESP_LOGI(TAG, "SET: TimeComponent");
    }
    #endif

}