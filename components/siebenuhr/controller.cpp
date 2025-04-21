#include "controller.h"

#include <Wire.h>
#include <BH1750.h>

#define SDA_PIN 14
#define SCL_PIN 13

BH1750 g_lightMeter; // Default address 0x23

namespace esphome::siebenuhr
{
    const char *const TAG = "🚀 Siebenuhr";

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
                ESP_LOGI(TAG, "BH1750 Initialized");
                m_EnvLightLevelEnabled = true;
            } 
            else 
            {
                ESP_LOGE(TAG, "Error initialising BH1750");
            }
        }
    }

    siebenuhr_core::Display *Controller::getDisplay()
    {
        if (m_display == nullptr)
        {
            ESP_LOGV(TAG, "Display not set. Initialize not called (yet) or something went wrong in the setup!?");
            m_display = siebenuhr_core::Display::getInstance();
        }
        return m_display;
    }

    void Controller::update()
    {
        // if (m_EnvLightLevelEnabled) 
        // {
        //     getDisplay()->setEnvLightLevel(g_lightMeter.readLightLevel(), 2, 150);
        // }

        getDisplay()->update();
    }

    void Controller::setPower(bool powerEnabled)
    {
        getDisplay()->setPowerEnabled(powerEnabled);
        ESP_LOGI(TAG, "Power set to %s", powerEnabled ? "ON" : "OFF");
    }

    void Controller::setBrightness(int value)
    {
        getDisplay()->setBrightness(value);
        ESP_LOGI(TAG, "Brightness set to %d", value);
    }

    void Controller::setColor(int r, int g, int b)
    {
        getDisplay()->setColor(CRGB(r, g, b));
        ESP_LOGI(TAG, "Color set to RGB(%d, %d, %d)", r, g, b);
    }

    void Controller::setText(const std::string &text)
    {
        getDisplay()->setText(text);
    }

    void Controller::setTime(int hours, int minutes) 
    {

        char formatted_time[5];  // Buffer for "HHMM" + null terminator
        snprintf(formatted_time, sizeof(formatted_time), "%02d%02d", hours, minutes);
        getDisplay()->setText(std::string(formatted_time));

        ESP_LOGI(TAG, "Time set: %02d:%02d", hours, minutes);
    }    
}