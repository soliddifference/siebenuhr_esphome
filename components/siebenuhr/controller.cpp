#include "controller.h"

namespace esphome::siebenuhr 
{
    void Controller::initialize()
    {
        m_display = siebenuhr_core::Display::getInstance();
        if (m_display != nullptr) 
        {
            m_display->initialize(siebenuhr_core::ClockType::CLOCK_TYPE_REGULAR, 1);
            m_display->setHeartbeatEnabled(true);
        }
    }

    void Controller::update()
    {
        if (m_display == nullptr) 
        {
            // something went wrong in the setup!?
            m_display = siebenuhr_core::Display::getInstance();
        }
        
        m_display->update();
    }
}