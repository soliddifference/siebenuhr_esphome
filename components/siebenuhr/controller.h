#pragma once

#include "siebenuhr_controller.h"
#include "siebenuhr_color.h"

#include "esphome/components/light/light_state.h"

namespace esphome::siebenuhr {

class Controller : public siebenuhr_core::BaseController {
public:
    void setLightState(light::LightState *state);

protected:
    bool sendBrightnessToHomeAssistant(int brightness) override;
    bool sendColorToHomeAssistant(CRGB color) override;

private:
    light::LightState *m_lightState = nullptr;
};

}