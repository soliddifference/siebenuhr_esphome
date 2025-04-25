#pragma once

#define SIEBENUHR_ESPHOME_VERSION "1.0.10"

#include "esphome/core/log.h"
#include "esphome/core/component.h"

#include "esphome/components/light/light_output.h"

#define WITH_WIFI
#ifdef WITH_WIFI
    #include "esphome/components/time/real_time_clock.h"
#endif

#include <siebenuhr_display.h>

#include "controller.h"

namespace esphome::siebenuhr {

class SiebenuhrClock : public light::LightOutput, public Component 
{
    public:
        void setup() override;
        void loop() override;

        light::LightTraits get_traits() override;
        void write_state(light::LightState *state) override;
        void dump_config() override;

        void set_type(int type);
        void set_auto_brightness(bool enable);
        void set_power_monitoring(bool enable);
        void set_text(std::string text);
        void set_personality(std::string rendererName);

        #ifdef WITH_WIFI
        void set_time_component(esphome::time::RealTimeClock *timeComponent);
        #endif

    protected:
        siebenuhr_core::ClockType m_type = siebenuhr_core::ClockType::CLOCK_TYPE_REGULAR;
        siebenuhr_core::PersonalityType m_personality = siebenuhr_core::PersonalityType::PERSONALITY_SOLIDCOLOR;

        #ifdef WITH_WIFI
        esphome::time::RealTimeClock *m_timeComponent = nullptr;
        #endif
        
        int m_currentHours = 0;
        int m_currentMinutes = 0;

        Controller m_controller;
};

extern SiebenuhrClock *global_siebenuhr_clock;

}
