#pragma once

#include "esphome/core/component.h"

#include "esphome/core/color.h"
#include "esphome/core/log.h"

#include <vector>

namespace esphome {
namespace siebenuhr {

class SiebenuhrDisplay : public Component {
 public:
  void setup() override;
  void loop() override;

  void set_mode(int mode) 
  {
    m_mode = mode;
    ESP_LOGD(TAG, "SiebenUhr Mode: %s", m_mode == 1 ? "MINI" : "REGULAR");
  }

protected:
  int m_mode {0};
  int m_pin {2};
  bool m_state {false};
};

}  // namespace addressable_light
}  // namespace esphome
