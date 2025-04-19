#include "controller.h"

#include <Wire.h>
#include <BH1750.h>

#define SDA_PIN 14
#define SCL_PIN 13

BH1750 g_lightMeter; // Default address 0x23

namespace esphome::siebenuhr
{
    void Controller::initialize(siebenuhr_core::ClockType type)
    {
        m_display = siebenuhr_core::Display::getInstance();
        if (m_display != nullptr)
        {
            m_display->initialize(type, 4);
            m_display->setHeartbeatEnabled(true);

            Wire.begin(SDA_PIN, SCL_PIN);
            if (g_lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) 
            {
                ESP_LOGI(GLOBAL_TAG, "BH1750 Initialized");
                m_EnvLightLevelEnabled = true;
            } 
            else 
            {
                ESP_LOGE(GLOBAL_TAG, "Error initialising BH1750");
            }
        }
    }

    void Controller::update()
    {
        if (m_display == nullptr)
        {
            // something went wrong in the setup!?
            m_display = siebenuhr_core::Display::getInstance();
        }

        if (m_EnvLightLevelEnabled) 
        {
            m_display->setEnvLightLevel(g_lightMeter.readLightLevel(), 2, 150);
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