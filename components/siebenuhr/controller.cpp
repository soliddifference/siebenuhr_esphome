#include "controller.h"

#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_INA219.h>

BH1750 g_bh1750; // Default address 0x23
Adafruit_INA219 g_ina219; // Default address 0x40

namespace esphome::siebenuhr
{
    const char *const TAG = "🚀 Siebenuhr";

    const ControllerMenu_t Controller::m_menu[Controller::m_menuMaxEntries] = {
		{CONTROLLER_MENU::BRIGHTNESS, "Brightness", "Brit"},
		{CONTROLLER_MENU::HUE, "Hue", "COLr"},
    };

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

        m_menuPosLastTimeChange = millis();

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

    void Controller::setMenu(CONTROLLER_MENU menu) {
        m_menuCurPos = menu;
        m_menuPosLastTimeChange = millis();
       
        switch (m_menuCurPos) {			
        case CONTROLLER_MENU::BRIGHTNESS:
        {
            int current_brightness = getDisplay()->getBrightness();
            m_encoder->setEncoderBoundaries(1, 255, current_brightness);
            ESP_LOGI(TAG, "Switch to Option: %s, value=%d", m_menu[m_menuCurPos].name.c_str(), current_brightness);
            break;
        }
        case CONTROLLER_MENU::HUE: 
        {
            getDisplay()->setPersonality(siebenuhr_core::PersonalityType::PERSONALITY_SOLIDCOLOR);
            CRGB current_color = getDisplay()->getColor();
            CHSV current_color_hsv = rgb2hsv_approximate(current_color);
            m_encoder->setEncoderBoundaries(0, 255, current_color_hsv.hue, true);
            ESP_LOGI(TAG, "Switch to Option: %s, hue=%d", m_menu[m_menuCurPos].name.c_str(), current_color_hsv.hue);
            break;
        }
        }
    }

    void Controller::handleMenuChange() 
    {
        // handle menu change
        if(m_encoder->isButtonReleased()) 
        {
            switch (m_menuCurPos) {			
            case CONTROLLER_MENU::BRIGHTNESS: {
                    getDisplay()->setNotification(" huE", 1500);
                    setMenu(CONTROLLER_MENU::HUE);
                    break;
                }
            case CONTROLLER_MENU::HUE: {
                    getDisplay()->setNotification("brit", 1500);
                    setMenu(CONTROLLER_MENU::BRIGHTNESS);
                    break;
                }
            }
        }

        // handle manual value changes of the appropriate menu
        if (m_encoder->hasPositionChanged()) 
        {          
            switch (m_menuCurPos) {
            case CONTROLLER_MENU::BRIGHTNESS: {
                handleManualBrightnessChange();
                break;
            }
            case CONTROLLER_MENU::HUE: {
                handleManualHueChange();
                break;
            }
            }
        }

    }   

    void Controller::handleManualBrightnessChange()
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

    void Controller::handleManualHueChange()
    {
        int hue = m_encoder->getPosition();
        CRGB color = CHSV(hue, 255, 255);

        if (m_lightState != nullptr)
        {
            // send state change back to home assistant server
            m_lightState->make_call()
                .set_color_mode(light::ColorMode::RGB)
                .set_red((float)color.red / 255.0f)
                .set_green((float)color.green / 255.0f)
                .set_blue((float)color.blue / 255.0f)
                .perform();
            // -> this will send a proper set hue change back to the clock!
        }
        else
        {
            // implicite conversion to CRGB
            getDisplay()->setColor(color);
        }
    }   

    void Controller::update()
    {
        if (m_encoder != nullptr) 
        {
            m_encoder->update();
            handleMenuChange();
        }
    
        if (m_autoBrightnessEnabled && m_isBH1750Initialized) 
        {
            getDisplay()->setEnvLightLevel(g_bh1750.readLightLevel(), m_currentBrightness, 100);
        }

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
        getDisplay()->setColor(CRGB(r, g, b));
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
        getDisplay()->setTime(hours, minutes);
        ESP_LOGI(TAG, "Time set: %02d:%02d", hours, minutes);
    }    

    void Controller::setAutoBrightnessEnabled(bool isEnabled)
    {   
        m_autoBrightnessEnabled = isEnabled;
        ESP_LOGI(TAG, "SET: auto_brightness=%s", m_autoBrightnessEnabled ? "TRUE" : "FALSE");
    }

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
            
            // Serial.println();
            m_lastSensorReadTime = millis();
        }
    }
}