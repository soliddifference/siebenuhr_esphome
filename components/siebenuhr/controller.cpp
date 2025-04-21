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
        if (m_autoBrightnessEnabled) 
        {
            getDisplay()->setEnvLightLevel(g_lightMeter.readLightLevel(), 2, 150);
        }

        if (m_colorWheelEnabled)
        {
            CRGB color = getColorWheelColor();
            getDisplay()->setColor(color);
        }

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
        if (!m_colorWheelEnabled)
        {
            getDisplay()->setColor(CRGB(r, g, b));
            ESP_LOGI(TAG, "Color set to RGB(%d, %d, %d)", r, g, b);
        }
    }

    void Controller::setText(const std::string &text)
    {
        getDisplay()->setText(text);
        ESP_LOGI(TAG, "SET: text=%s", text.c_str());
    }

    void Controller::setTime(int hours, int minutes) 
    {

        char formatted_time[5];  // Buffer for "HHMM" + null terminator
        snprintf(formatted_time, sizeof(formatted_time), "%02d%02d", hours, minutes);
        getDisplay()->setText(std::string(formatted_time));

        ESP_LOGI(TAG, "Time set: %02d:%02d", hours, minutes);
    }    

    void Controller::setAutoBrightnessEnabled(bool isEnabled)
    {   
        m_autoBrightnessEnabled = isEnabled;
        ESP_LOGI(TAG, "SET: auto_brightness=%s", m_autoBrightnessEnabled ? "TRUE" : "FALSE");
    }

    void Controller::setColorWheelEnabled(bool isEnabled)
    {
        m_colorWheelEnabled = isEnabled;
        ESP_LOGI(TAG, "SET: color_wheel=%s", m_colorWheelEnabled ? "TRUE" : "FALSE");
    }

    
    CRGB Controller::getColorWheelColor() 
    {
        uint8_t hue = 0;
        if (false)
        {
            // fast rotation: Duration of a full cycle in milliseconds
            const uint32_t cycle_duration_ms = 20000; // 20 seconds
            uint32_t now = millis();    
            hue = (now % cycle_duration_ms) * 255 / cycle_duration_ms;
        }
        else
        {
            // legacy version from siebenuhr v1.0
            int sec_of_day = hour() * 3600 + minute() * 60;
            hue = (int)(((float)sec_of_day / (float)86400) * 255) % 255;
        }

        return CHSV(hue, 255, 255); // Full saturation and brightness
    }
}