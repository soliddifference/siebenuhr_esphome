#pragma once

#include "esphome/core/log.h"
#include "esphome/components/light/light_output.h"

#include <siebenuhr_display.h>
#include <siebenuhr_encoder.h>
#include <siebenuhr_button.h>

namespace esphome::siebenuhr
{
    enum CONTROLLER_MENU {
        BRIGHTNESS = 0,
        HUE,
        MENU_END
    };

    struct ControllerMenu_t {
        CONTROLLER_MENU uid;
        String name;
        String message;
    };

    class Controller
    {
    public:
    	static const int m_menuMaxEntries = CONTROLLER_MENU::MENU_END;
	    static const ControllerMenu_t m_menu[m_menuMaxEntries];

        void initialize(siebenuhr_core::ClockType type);
        void update();

        void setLightState(light::LightState *state);

        void setMenu(CONTROLLER_MENU menu);
        void handleMenuChange();
        void handleManualBrightnessChange();
        void handleManualHueChange();

        void setPower(bool powerEnabled);
        void setBrightness(int value);
        void setColor(int r, int g, int b);

        void setText(const std::string &text);
        void setTime(int hours, int minutes);

        void setAutoBrightnessEnabled(bool isEnabled);
        void setPowerMonitoringEnabled(bool isEnabled);
        void setPersonality(siebenuhr_core::PersonalityType personality);

        inline siebenuhr_core::Display *getDisplay();

    private:
        void initializeControls();
        void readAndPrintPowerMonitoring();

        siebenuhr_core::ClockType m_clockType;

        bool m_autoBrightnessEnabled = false;
        bool m_powerMonitoringEnabled = false;

        int m_hours = -1;
        int m_minutes = -1;
        
        CRGB m_currentColor = CRGB::Black;
        int m_currentBrightness = 0;

        inline bool isTimeSet() 
        {
            return (m_hours != -1 && m_minutes != -1);
        }

        unsigned int m_lastSensorReadTime = 0;
        bool m_isBH1750Initialized = false;
        bool m_isINA219Initialized = false;

        uint8_t m_menuCurPos = CONTROLLER_MENU::BRIGHTNESS;
        uint32_t m_menuPosLastTimeChange;
        
        siebenuhr_core::UIKnob* m_encoder = nullptr;
        siebenuhr_core::UIButton* m_button1 = nullptr;
        siebenuhr_core::UIButton* m_button2 = nullptr;
        siebenuhr_core::Display *m_display = nullptr;

        light::LightState *m_lightState = nullptr;
    };
}