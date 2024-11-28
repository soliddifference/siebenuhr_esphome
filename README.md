# Siebenuhr ESPHome
This repository develops siebenuhr_esphome, an ESPHome-compatible version of the Siebenuhr ESP32 firmware. It transitions the standalone PlatformIO-based firmware into n [ESPHome External Component](https://esphome.io/components/external_components.html).

---

## Prerequisites

Before proceeding, ensure you have the following installed:

1. [ESPHome](https://esphome.io/guides/installing.html) installed locally or in a Home Assistant environment.
2. An ESP32 board and the necessary USB connection for flashing.
3. A configured development environment (Python, ESPHome CLI, etc.).

---

## Getting Started

Follow these steps to build and run the Siebenuhr ESPHome firmware.

### 1. Clone the Repository
Checkout the code from the repository:
```bash
git clone https://github.com/soliddifference/siebenuhr_esphome.git
cd siebenuhr_esphome
```

### 2. Update Configuration
1. Make a copy of the placeholder file:
   ```bash
   cp __init__RENAME.py __init__.py
   ```
2. Edit the Wi-Fi configuration in `__init__.py` to match your network credentials:
   ```yaml
   wifi:
     ssid: "YourSSID"
     password: "YourPassword"
   ```

### 3. Build and Upload the Firmware
Run the following command to build and upload the firmware to your ESP32:
```bash
esphome run siebenuhr_config.yaml
```

This will compile the configuration, flash the ESP32 board, and start the ESPHome firmware.
