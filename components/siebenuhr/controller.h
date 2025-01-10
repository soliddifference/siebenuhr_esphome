#pragma once

#include <siebenuhr_display.h>

namespace esphome::siebenuhr 
{
    class Controller 
    {
        public:
            void initialize();
            void update();
        
        private:
            siebenuhr_core::Display *m_display;
    };
}