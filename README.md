# BlackCapy

BlackCapy is a professional modular embedded toolkit for hardware interaction, wireless analysis, automation, structured logging, native microSD storage and expansion modules.

The project targets ESP32-based portable devices with a 320x240 display, physical navigation buttons and an expansion bus for specialized hardware modules.

BlackCapy is inspired by Arch Linux principles: freedom, modularity, control and customization. It should stay powerful and transparent without becoming unnecessarily niche or difficult to use.

---

## Release Baseline

BlackCapy 1.0.0 is the current software baseline for the ESP32 firmware, microSD runtime tree, validation scripts and AI companion gateway.

The baseline guarantees:

* Keep `AppManager` as the single source of truth for apps
* Keep `ModuleManager` and `ModuleManifest` responsible for external modules
* Keep `Logger` for system behavior and `CaptureWriter` for operational evidence
* Treat microSD as native onboard storage, not as an expansion module
* Preserve PlatformIO compatibility with `src_dir = main`
* Use the 320x240 SPI TFT as the primary UI backend, with serial kept as a debug mirror
* Use ADC-ladder physical buttons to keep GPIO available for display, storage and expansion
* Keep development module mocks disabled in production firmware
* Keep cloud AI behind a gateway, with no provider API key in firmware

---

## Implemented Foundation

### Core

* AppManager registration, selection, execution and runnable checks
* AppContract permissions and app status metadata
* ModuleManifest for supported external module types
* ModuleManager with ADC resistor-ID detection and production-disabled mocks
* CaptureWriter for `/captures/*.log` operational evidence
* Logger with optional `/logs/system.log` output
* FileSystem initialization for native microSD directories
* StoragePolicy separating internal Preferences from operational microSD data
* InputManager for debounced physical buttons
* DisplayManager with Adafruit ILI9341 SPI TFT rendering and serial debug mirror
* ScreenManager for Home, Tools, Status and Settings
* UIController connecting physical input to screens and AppManager
* EventBus, StatusManager and AutomationEngine base

### Apps

BlackCapy currently registers apps through `main/firmware/src/main.cpp` using `AppEntry`.

Implemented or scaffolded app areas include:

* System, diagnostics, health and settings
* WiFi and BLE
* GPIO, PWM, analog, I2C and UART
* Storage, logs, captures, signal databases and AI companion reports
* Automation
* Modules and module diagnostics
* IR, RF/Sub-GHz, RFID, NFC, CAN, GPS and LoRa
* AI companion gateway analysis and local report viewing

Apps that collect operational evidence use `CaptureWriter`; system behavior uses `Logger`.

---

## Project Structure

```txt
blackcapy/
├── platformio.ini
├── README.md
├── usage.md
├── docs/
├── hardware/
├── scripts/
├── tests/
├── assets/
└── main/
    ├── firmware/
    ├── core/
    └── apps/
```

Important paths:

```txt
main/firmware/src/main.cpp          App registration and boot flow
main/core/app_manager/             Single source of truth for apps
main/core/module_manager/          Runtime external module detection
main/core/module_manifest/         Supported module definitions
main/core/capture/                 Operational evidence writer
main/core/logging/                 System logger
main/core/filesystem/              Native microSD filesystem
```

---

## Build

Requirements:

* ESP32 development board
* PlatformIO CLI or VSCode PlatformIO extension
* USB cable

Build:

```bash
scripts/build.sh
```

`build.sh` runs the project validation gates before PlatformIO so AppManager, assets, AI, docs, architecture, include paths and ignore rules fail early.

Validation scripts:

```bash
scripts/validate_apps.py
scripts/validate_assets.py
scripts/validate_ai.py
scripts/validate_docs.py
scripts/validate_architecture.py
scripts/validate_platformio.py
scripts/validate_gitignore.py
```

Hardware pin conflict report:

```bash
scripts/check_hardware_config.py
```

Production release gate:

```bash
scripts/release_gate.py
```

Create release artifacts manually after a successful build:

```bash
scripts/package_release.py --clean
```

Upload:

```bash
scripts/flash.sh
```

Prepare a development microSD tree:

```bash
scripts/generate_assets.py --clean
```

Convert artist-friendly image files into BlackCapy display assets:

```bash
python3 -m pip install pillow
scripts/convert_art.py art/icon.png -o assets/icons/icon.bmp --max-width 32 --max-height 32
scripts/convert_art.py art/boot.gif -o assets/animations/boot.anim --max-width 96 --max-height 64
```

More details are in [`usage.md`](usage.md). Hardware validation steps are in [`docs/hardware.md`](docs/hardware.md).
Pixel-art and animation placement examples are in [`docs/art_assets.md`](docs/art_assets.md).

---

## Architecture Rules

Do not create parallel app systems.

Never create:

```txt
ToolManager
AppManagerV2
ToolRegistryV2
Secondary app lists
```

Use the existing modules:

```txt
Apps          -> AppManager
External HW   -> ModuleManager + ModuleManifest
System events -> Logger
Evidence      -> CaptureWriter
Storage paths -> StoragePolicy + FileSystem
```

---

## UI Direction

BlackCapy uses a hybrid interface:

* ASCII for technical data, logs, menus and diagnostics
* Pixel art for splash screens, icons, status and context

Performance comes first. The physical TFT is the primary interface; serial output remains available for debugging and bench validation.

---

## Expansion Direction

Expansion modules are for specialized capabilities such as RF, Sub-GHz, IR, NFC, RFID, CAN, GPS, LoRa and sensors.

Sub-GHz capability should remain conservative and auditable: scan, frequency sweep, live RSSI, raw capture, decoder support, signal database on microSD, dedicated UI, AppManager/ModuleManager/CaptureWriter integration and physically confirmed authorized replay with mandatory logs.

BadUSB is not a priority. If it is added later, it should be an external hardware module with native USB HID support, disabled by default, with physical confirmation and logs.

---

## Status

Phase: 1.0.0 release baseline.

The firmware, tooling, docs and gateway are validation-gated. Market release still requires physical target-board validation, enclosure validation, regulatory review and manufacturing QA.
