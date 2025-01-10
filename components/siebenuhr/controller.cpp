#include "controller.h"

namespace esphome::siebenuhr 
{
    void Controller::initialize()
    {
        m_display = siebenuhr_core::Controller::getInstance();
        if (m_display != nullptr) 
        {
            m_display->initialize(siebenuhr_core::Controller::ClockType::Regular, 1);
            m_display->setHeartbeatEnabled(true);
        }
    }

    void Controller::update()
    {
        if (m_display == nullptr) 
        {
            // something went wrong in the setup!?
            m_display = siebenuhr_core::Controller::getInstance();
        }
        
        m_display->update();
    }
}