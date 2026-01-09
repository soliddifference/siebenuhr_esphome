# Siebenuhr ESPHome

ESPHome (for Home Assistant) integration with **Siebenuhr 7-segment LED clocks**. This component wraps the [siebenuhr_core](https://github.com/soliddifference/siebenuhr_core) library as an [ESPHome External Component](https://esphome.io/components/external_components.html), enabling Home Assistant integration with OTA updates, WiFi config, and light controls.

The hardwareclocks can be ordered from our web shop at [soliddifference.com](https://soliddifference.com/)

> If you don't use Home Assistant, you can use our [standalone "factory" firmware](https://github.com/soliddifference/siebenuhr) which can be flashed using PlatformIO.

---

## Prerequisites

- [ESPHome](https://esphome.io/guides/installing.html) installed locally or via Home Assistant
- ESP32 board with USB connection for flashing
- [uv](https://docs.astral.sh/uv/getting-started/installation/) for Python dependency management

---

## Getting Started

1. Clone the repository:

   ```bash
   git clone https://github.com/soliddifference/siebenuhr_esphome
   cd siebenuhr_esphome
   ```

2. Install dependencies and activate the virtual environment:

If needed, install uv: <https://docs.astral.sh/uv/getting-started/installation/> and then:

   ```bash
   uv sync
   source .venv/bin/activate  # macOS/Linux
   # or: .venv\Scripts\activate  # Windows
   ```

### Configuration

1. **Create your secrets file:**

   ```bash
   cp secrets_template.yaml secrets.yaml
   ```

   Edit `secrets.yaml` and enter your WiFi credentials and API keys.

2. **Choose a configuration:**

   - `siebenuhr_git.yaml` — Uses GitHub for both the component and core library.
   - `siebenuhr_local.yaml` — Uses local paths if changes are needed to the siebenuhr_core library.

3. **Local development with siebenuhr_core:**

   Clone the core library into the `tmp/` folder:

   ```bash
   git clone https://github.com/soliddifference/siebenuhr_core.git tmp/siebenuhr_core
   ```

   The `siebenuhr_local.yaml` references it using a portable path:

   ```yaml
   libraries:
     - file://${sysenv.PWD}/tmp/siebenuhr_core
   ```

   This allows editing the core library locally without Git commits for each test.

### Compiling and Flashing

Compile and upload to your device:

```bash
esphome run siebenuhr_local.yaml   # for local development
esphome run siebenuhr_git.yaml     # for production/release
esphome logs siebenuhr_local.yaml   # view device logs (wifi or usb connection)
```

This will also show you the logs from the device in the terminal.

---

**Happy coding!**
