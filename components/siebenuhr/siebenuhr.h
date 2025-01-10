#pragma once

#include <Arduino.h>

#include "esphome/core/component.h"
#include "esphome/core/color.h"

#include "esphome/core/log.h"

#include "controller.h"

namespace esphome::siebenuhr 
{
    class SiebenuhrDisplay : public Component 
    {
    public:
        void setup() override;
        void loop() override;

        void set_mode(int mode);

    protected:
        int m_mode {0};

        Controller m_controller;
    };
}
