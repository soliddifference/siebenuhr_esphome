#pragma once

#include "esphome/core/log.h"

#include <siebenuhr_display.h>

namespace esphome::siebenuhr
{
    class Controller
    {
    public:
        void initialize(siebenuhr_core::ClockType type);
        void update();

        void setPower(bool powerEnabled);
        void setBrightness(int value);
        void setColor(int r, int g, int b);

        void setText(const std::string &text);
        void setTime(int hours, int minutes);

        void setAutoBrightnessEnabled(bool isEnabled);
        void setColorWheelEnabled(bool isEnabled);
        void setPowerMonitoringEnabled(bool isEnabled);

        inline siebenuhr_core::Display *getDisplay();

    private:
        CRGB getColorWheelColor(int hours, int minutes);
        void readAndPrintPowerMonitoring();

        bool m_autoBrightnessEnabled = false;
        bool m_colorWheelEnabled = false;
        bool m_powerMonitoringEnabled = false;

        int m_hours = -1;
        int m_minutes = -1;

        inline bool isTimeSet() 
        {
            return (m_hours != -1 && m_minutes != -1);
        }

        unsigned int m_lastSensorReadTime = 0;
        bool m_isBH1750Initialized = false;
        bool m_isINA219Initialized = false;
        
        siebenuhr_core::Display *m_display;
    };
}