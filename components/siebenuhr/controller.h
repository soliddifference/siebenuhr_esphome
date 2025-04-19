#pragma once

#include <siebenuhr_display.h>

namespace esphome::siebenuhr
{
    class Controller
    {
    public:
        void initialize(siebenuhr_core::ClockType type);
        void update();

        void setText(const std::string &text);
        void setTime(int hours, int minutes);

    private:
        siebenuhr_core::Display *m_display;
    };
}