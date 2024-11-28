#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace siebenuhr {

class SiebenuhrDisplay : public Component {
 public:
  void setup() override;
  void loop() override;

  void set_mode(int mode);

protected:
  int m_mode {0};
  int m_pin {2};
  bool m_state {false};
};

}  // namespace addressable_light
}  // namespace esphome
