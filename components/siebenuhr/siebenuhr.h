#pragma once

#include <Arduino.h>

#include "esphome/core/component.h"
#include "esphome/core/log.h"

#include "esphome/components/time/real_time_clock.h"

#include "controller.h"

#include <string>

namespace esphome::siebenuhr 
{
    class SiebenuhrDisplay : public Component 
    {
    public:
        void setup() override;
        void loop() override;

        void setMode(int mode);
        void setTimeComponent(esphome::time::RealTimeClock *timeComponent);

        void setText(const std::string &text);

    protected:
        int m_mode = 0;

        esphome::time::RealTimeClock *m_timeComponent = nullptr;

        int m_currentHours = 0;
        int m_currentMinutes = 0;

        Controller m_controller;
    };
}
