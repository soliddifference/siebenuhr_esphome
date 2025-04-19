#pragma once

#define FIRMWARE_VERSION "1.0.1"

#include <Arduino.h>

#include "esphome/core/component.h"
#include "esphome/core/log.h"

#ifdef WITH_WIFI
    #include "esphome/components/time/real_time_clock.h"
#endif

#include "controller.h"

#include <string>

namespace esphome::siebenuhr 
{
    class SiebenuhrClock : public Component 
    {
    public:
        void setup() override;
        void loop() override;

        void set_mode(int mode);
        #ifdef WITH_WIFI
        void set_time_component(esphome::time::RealTimeClock *timeComponent);
        #endif
        void set_text(const std::string &text);

    protected:
        siebenuhr_core::ClockType m_type = siebenuhr_core::ClockType::CLOCK_TYPE_REGULAR;

        #ifdef WITH_WIFI
        esphome::time::RealTimeClock *m_timeComponent = nullptr;
        #endif

        int m_currentHours = 0;
        int m_currentMinutes = 0;

        Controller m_controller;
    };
}
