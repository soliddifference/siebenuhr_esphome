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

        inline siebenuhr_core::Display *getDisplay();

    private:
        CRGB getColorWheelColor();

        bool m_autoBrightnessEnabled = false;
        bool m_colorWheelEnabled = false;

        siebenuhr_core::Display *m_display;
    };
}