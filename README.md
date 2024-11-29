# Siebenuhr ESPHome
This repository develops siebenuhr_esphome, an ESPHome-compatible version of the Siebenuhr ESP32 firmware. It transitions the standalone PlatformIO-based firmware into n [ESPHome External Component](https://esphome.io/components/external_components.html).

---

## Prerequisites

Before proceeding, ensure you have the following installed:

1. [ESPHome](https://esphome.io/guides/installing.html) installed locally or in a Home Assistant environment.
2. An ESP32 board and the necessary USB connection for flashing.
3. A configured development environment (Python, ESPHome CLI, VisualCode, etc.).

---

## Getting Started

### Prerequisites
1. Ensure you have [ESPHome](https://esphome.io/) installed and set up on your system.
2. Clone the repository to your local machine:
   ```bash
   git clone https://github.com/your-repo-name.git
   cd your-repo-name
   ```

### Configuration
1. Make a copy of either `siebenuhr_git.yaml` or `siebenuhr_local.yaml`:
   - Use `siebenuhr_local.yaml` for development purposes as it refers to the local version of the code and is optimized for fast compile and testing iterations.
   ```bash
   cp siebenuhr_local.yaml your_modified_config.yaml
   ```

2. Edit `your_modified_config.yaml` to configure it for your local environment:
   - Update the most critical settings, specifically WiFi credentials, in the `wifi` section:
     ```yaml
     wifi:
       ssid: !secret wifi_ssid
       password: !secret wifi_password
     ```

### Compiling and Testing
1. Test if your setup compiles by running:
   ```bash
   esphome compile your_modified_config.yaml
   ```

2. Alternatively, you can compile, upload, and connect to your device in one step:
   ```bash
   esphome run your_modified_config.yaml
   ```

---

**Happy coding!**
