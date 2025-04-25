#include "controller.h"

#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_INA219.h>

BH1750 g_bh1750; // Default address 0x23
Adafruit_INA219 g_ina219; // Default address 0x40

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

            Wire.begin(siebenuhr_core::constants::SDA_PIN, siebenuhr_core::constants::SCL_PIN);
            if (g_bh1750.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) 
            {
                ESP_LOGI(TAG, "BH1750 Initialized");
                m_isBH1750Initialized = true;
            } 
            else 
            {
                ESP_LOGE(TAG, "Error initialising BH1750");
            }

            if (g_ina219.begin()) 
            {
                ESP_LOGI(TAG, "INA219 Initialized");
                m_isINA219Initialized = true;
            } else 
            {
                ESP_LOGE(TAG, "Error initialising INA219");
            }
        }

        // setup hardware controls
        initializeControls();
    }

    void Controller::initializeControls()
    {
        m_encoder = new siebenuhr_core::UIKnob(siebenuhr_core::constants::ROT_ENC_A_PIN, siebenuhr_core::constants::ROT_ENC_B_PIN, siebenuhr_core::constants::ROT_ENC_BUTTON_PIN);;
        m_encoder->setEncoderBoundaries(1, 255, 128, false);
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
        if (m_encoder != nullptr) 
        {
            m_encoder->update();
            if (m_encoder->hasPositionChanged()) 
            {                
                if (m_autoBrightnessEnabled)
                {
                    long brightness = m_encoder->getPosition(); 
                    // send state change back to home assistant server
                    if (m_lightState != nullptr)
                    {
                        m_lightState->make_call()
                            .set_color_mode(light::ColorMode::RGB)
                            .set_brightness((float)brightness / 255.0f)
                            .perform();
                        // -> this will send a proper set brightness change back to the clock!
                    }
                    else 
                    {
                        m_currentBrightness = brightness;
                    }
                }
                else
                {
                    long brightness = m_encoder->getPosition(); 
                    if (brightness != getDisplay()->getBrightness())
                    {
                        // send state change back to home assistant server
                        if (m_lightState != nullptr)
                        {
                            m_lightState->make_call()
                                .set_color_mode(light::ColorMode::RGB)
                                .set_brightness((float)brightness / 255.0f)
                                .perform();
                            // -> this will send a proper set brightness change back to the clock!
                        }
                        else 
                        {
                            getDisplay()->setBrightness(brightness);
                        }
                    }
                }
            }
        }
    
        if (m_autoBrightnessEnabled && m_isBH1750Initialized) 
        {
            getDisplay()->setEnvLightLevel(g_bh1750.readLightLevel(), m_currentBrightness, 100);
        }

        // if (m_colorWheelEnabled && isTimeSet())
        // {
        //     CRGB color = getColorWheelColor(m_hours, m_minutes);
        //     getDisplay()->setColor(color);
        // }

        if (m_powerMonitoringEnabled && m_isINA219Initialized)
        {
            readAndPrintPowerMonitoring();
        }

        getDisplay()->update();
    }

    void Controller::setLightState(light::LightState *state)
    {
        m_lightState = state;
    }

    void Controller::setPower(bool powerEnabled)
    {
        getDisplay()->setPowerEnabled(powerEnabled);
        ESP_LOGV(TAG, "Power set to %s", powerEnabled ? "ON" : "OFF");
    }

    void Controller::setBrightness(int value)
    {
        if (m_autoBrightnessEnabled)
        {
            m_currentBrightness = value;
            m_encoder->setPosition(m_currentBrightness);
            ESP_LOGI(TAG, "Base-Brightness set to %d", m_currentBrightness);
        }
        else
        {
            if (value != m_currentBrightness)
            {
                getDisplay()->setBrightness(value);
                m_currentBrightness = getDisplay()->getBrightness();
                m_encoder->setPosition(m_currentBrightness);
                ESP_LOGV(TAG, "Brightness set to %d", value);
            }
        }
    }

    void Controller::setColor(int r, int g, int b)
    {
        // if (!m_colorWheelEnabled)
        // {
            getDisplay()->setColor(CRGB(r, g, b));
            // ESP_LOGI(TAG, "Color set to RGB(%d, %d, %d)", r, g, b);
        // }
    }

    void Controller::setText(const std::string &text)
    {
        getDisplay()->setText(text);
        ESP_LOGI(TAG, "SET: text=%s", text.c_str());
    }

    void Controller::setTime(int hours, int minutes) 
    {
        m_hours = hours;
        m_minutes = minutes;

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

    // void Controller::setColorWheelEnabled(bool isEnabled)
    // {
    //     m_colorWheelEnabled = isEnabled;
    //     ESP_LOGI(TAG, "SET: color_wheel=%s", m_colorWheelEnabled ? "TRUE" : "FALSE");
    // }

    // CRGB Controller::getColorWheelColor(int hours, int minutes) 
    // {
    //     uint8_t hue = 0;
    //     if (false)
    //     {
    //         // fast rotation for testing: Duration of a full cycle in milliseconds
    //         const uint32_t cycle_duration_ms = 20000; // 20 seconds
    //         uint32_t now = millis();    
    //         hue = (now % cycle_duration_ms) * 255 / cycle_duration_ms;
    //     }
    //     else
    //     {
    //         // legacy version from siebenuhr v1.0
    //         int sec_of_day = hours * 3600 + minutes * 60;
    //         hue = (int)(((float)sec_of_day / (float)86400) * 255) % 255;
    //     }

    //     return CHSV(hue, 255, 255); // Full saturation and brightness
    // }

    void Controller::setPowerMonitoringEnabled(bool isEnabled)
    {
        m_powerMonitoringEnabled = isEnabled;
    }

    void Controller::setPersonality(siebenuhr_core::PersonalityType personality)
    {
        if (m_display != nullptr)
        {
            m_display->setPersonality(personality);
        }
    }

    void Controller::readAndPrintPowerMonitoring()
    {
        if (millis() - m_lastSensorReadTime >= siebenuhr_core::constants::SensorReadInterval) {
            float busvoltage = g_ina219.getBusVoltage_V();
            float shuntvoltage = g_ina219.getShuntVoltage_mV();  // in mV
            float current_mA = shuntvoltage * 100.0;           // for 10 mΩ shunt
            float loadvoltage = busvoltage + (shuntvoltage / 1000.0); // in V
            float power_mW = busvoltage * current_mA;          // V * mA = mW
    
            bool current_in_A = abs(current_mA) >= 500.0;
            bool power_in_W = abs(power_mW) >= 500.0;

            ESP_LOGI(TAG, "Bus: %.2fV | Shunt: %.2fmV | Load: %.2fV | Current: %.2f%s | Power: %.2f%s", 
                busvoltage, 
                shuntvoltage, 
                loadvoltage, 
                current_in_A?(current_mA / 1000.0):current_mA, 
                current_in_A?"A":"mA",
                power_in_W?(power_mW / 1000.0):power_mW,
                power_in_W?"W":"mW"
            );

            // // Format Bus Voltage (already in V)
            // Serial.print("Bus: ");
            // Serial.print(busvoltage, 2);
            // Serial.print("V");
    
            // // Format Shunt Voltage (always in mV for INA219)
            // Serial.print(" | Shunt: ");
            // Serial.print(shuntvoltage, 2);
            // Serial.print("mV");
    
            // // Format Load Voltage (already in V)
            // Serial.print(" | Load: ");
            // Serial.print(loadvoltage, 2);
            // Serial.print("V");
    
            // // Format Current with adaptive units (mA or A)
            // Serial.print(" | Current: ");
            // if (abs(current_mA) >= 500.0) {
            //     Serial.print(current_mA / 1000.0, 2); // Convert to A with 2 decimal places
            //     Serial.print("A");
            // } else {
            //     Serial.print(current_mA, 1); // Keep as mA with 1 decimal place
            //     Serial.print("mA");
            // }
    
            // // Format Power with adaptive units (mW or W)
            // Serial.print(" | Power: ");
            // if (abs(power_mW) >= 500.0) {
            //     Serial.print(power_mW / 1000.0, 2); // Convert to W with 2 decimal places
            //     Serial.print("W");
            // } else {
            //     Serial.print(power_mW, 1); // Keep as mW with 1 decimal place
            //     Serial.print("mW");
            // }
            
            // Serial.println();
            m_lastSensorReadTime = millis();
        }
    }
}