# Siebenuhr ESPHome

[![Build](https://github.com/soliddifference/siebenuhr_esphome/actions/workflows/build.yml/badge.svg)](https://github.com/soliddifference/siebenuhr_esphome/actions/workflows/build.yml)

ESPHome (for Home Assistant) integration with **Siebenuhr 7-segment LED clocks**. This component wraps the [siebenuhr_core](https://github.com/soliddifference/siebenuhr_core) library as an [ESPHome External Component](https://esphome.io/components/external_components.html), enabling Home Assistant integration with OTA updates, WiFi config, and light controls.

The hardware clocks can be ordered from our web shop at [soliddifference.com](https://soliddifference.com/)

> If you don't use Home Assistant, you can use our [standalone "factory" firmware](https://github.com/soliddifference/siebenuhr) which can be flashed using PlatformIO.

## Quick Install

Pre-built firmware is available via our [web flasher](https://support.soliddifference.com/firmware). Connect your clock via USB and flash directly from your browser - no development tools required.

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

## Creating a Release

Releases are automated via GitHub Actions. When a version tag is pushed:

1. The `build-release` workflow compiles firmware for both Mini and Regular variants
2. A GitHub release is created with the firmware binaries attached
3. The `notify-support-site` workflow triggers a sync to update the web flasher

To create a new release:

```bash
# Tag the release
git tag -a v1.0.0 -m "Release description"
git push origin v1.0.0
```

The firmware binaries will be named:
- `firmware-mini-{version}.bin` - For Mini clocks (28 LEDs per segment)
- `firmware-regular-{version}.bin` - For Regular clocks (119 LEDs per segment)

### Release Configurations

The release builds use dedicated configurations (`siebenuhr_mini.yaml` and `siebenuhr_regular.yaml`) that:
- Don't require pre-configured WiFi credentials
- Start in AP mode with captive portal for initial setup
- Support Improv Serial and ESP32 Improv for easy WiFi configuration

---

## Project Structure

```
siebenuhr_esphome/
├── components/siebenuhr/     # ESPHome external component
│   ├── __init__.py
│   ├── light.py              # ESPHome light platform config
│   ├── controller.cpp/.h     # Controller logic
│   └── siebenuhr.cpp/.h      # Main component
├── siebenuhr_local.yaml      # Local development config
├── siebenuhr_git.yaml        # Production config (uses git refs)
├── siebenuhr_mini.yaml       # Release build for Mini variant
├── siebenuhr_regular.yaml    # Release build for Regular variant
└── .github/workflows/
    ├── build.yml             # CI build check
    ├── build-release.yml     # Release build workflow
    └── notify-support-site.yml # Support site notification
```

---

**Happy coding!**
