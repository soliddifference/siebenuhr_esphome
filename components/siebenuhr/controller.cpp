#include "controller.h"

namespace esphome::siebenuhr {

void Controller::setLightState(light::LightState *state) {
    m_lightState = state;
}

bool Controller::sendBrightnessToHomeAssistant(int brightness) {
    if (m_lightState != nullptr) {
        m_lightState->make_call()
            .set_color_mode(light::ColorMode::RGB)
            .set_brightness((float)brightness / 255.0f)
            .perform();
        return true;
    }
    return false;
}

bool Controller::sendColorToHomeAssistant(CRGB color) {
    if (m_lightState != nullptr) {
        m_lightState->make_call()
            .set_color_mode(light::ColorMode::RGB)
            .set_red((float)color.red / 255.0f)
            .set_green((float)color.green / 255.0f)
            .set_blue((float)color.blue / 255.0f)
            .perform();
        LOG_D("Color sent to Home Assistant: %d %d %d", color.red, color.green, color.blue);
        return true;
    }
    return false;
}

} 