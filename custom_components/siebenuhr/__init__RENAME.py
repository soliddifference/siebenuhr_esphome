esphome:
  name: siebenuhr_component
  platform: ESP32
  board: esp32dev

# Wi-Fi configuration
wifi:
  ssid: "YourSSID"
  password: "YourPassword"

# Enable logging
logger:

# Enable OTA updates
ota:

# Register the custom component
custom_component:
  - lambda: |-
      auto clock = new SiebenUhrComponent();
      App.register_component(clock);
      return {clock};