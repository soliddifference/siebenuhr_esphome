#pragma once

#include "esphome/core/log.h"
#include "esphome/components/light/light_output.h"

#include <siebenuhr_display.h>
#include <siebenuhr_encoder.h>

namespace esphome::siebenuhr
{
    class Controller
    {
    public:
        void initialize(siebenuhr_core::ClockType type);
        void update();

        void setLightState(light::LightState *state);

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

        bool m_autoBrightnessEnabled = false;
        bool m_powerMonitoringEnabled = false;

        int m_hours = -1;
        int m_minutes = -1;
        
        int m_currentBrightness = 0;

        inline bool isTimeSet() 
        {
            return (m_hours != -1 && m_minutes != -1);
        }

        unsigned int m_lastSensorReadTime = 0;
        bool m_isBH1750Initialized = false;
        bool m_isINA219Initialized = false;
        
        siebenuhr_core::UIKnob* m_encoder = nullptr;
        siebenuhr_core::Display *m_display = nullptr;

        light::LightState *m_lightState = nullptr;
    };
}