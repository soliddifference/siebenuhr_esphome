#include "siebenuhr.h"

namespace esphome::siebenuhr
{
    static const char *const TAG = "siebenuhr";

    void SiebenuhrDisplay::set_mode(int mode)
    {
        m_mode = mode;
        ESP_LOGCONFIG(TAG, "SiebenUhr Mode: %s", m_mode == 1 ? "MINI" : "REGULAR");
    }

    void SiebenuhrDisplay::setup()
    {
        ESP_LOGCONFIG(TAG, "Setting up Siebenuhr Component...");
        m_controller.initialize();
    }

    void SiebenuhrDisplay::loop()
    {
        m_controller.update();
    }
}