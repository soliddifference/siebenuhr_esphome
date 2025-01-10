#pragma once

#include <siebenuhr_controller.h>

namespace esphome::siebenuhr 
{
    class Controller 
    {
        public:
            void initialize();
            void update();
        
        private:
            siebenuhr_core::Controller *m_display;
    };
}