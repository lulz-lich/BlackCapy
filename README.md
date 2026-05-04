# BlackCapy

BlackCapy is a professional modular embedded toolkit for hardware interaction, wireless analysis, automation, structured logging, native microSD storage and expansion modules.

The project targets ESP32-based portable devices with a 320x240 display, physical navigation buttons and an expansion bus for specialized hardware modules.

BlackCapy is inspired by Arch Linux principles: freedom, modularity, control and customization. It should stay powerful and transparent without becoming unnecessarily niche or difficult to use.

---

## Current Focus

BlackCapy is currently in the core firmware consolidation phase.

The immediate goals are:

* Keep `AppManager` as the single source of truth for apps
* Keep `ModuleManager` and `ModuleManifest` responsible for external modules
* Keep `Logger` for system behavior and `CaptureWriter` for operational evidence
* Treat microSD as native onboard storage, not as an expansion module
* Preserve PlatformIO compatibility with `src_dir = main`
* Keep serial as a temporary debug interface while the physical UI matures

---

## Implemented Foundation

### Core

* AppManager registration, selection, execution and runnable checks
* AppContract permissions and app status metadata
* ModuleManifest for supported external module types
* ModuleManager with mock and ADC resistor-ID detection
* CaptureWriter for `/captures/*.log` operational evidence
* Logger with optional `/logs/system.log` output
* FileSystem initialization for native microSD directories
* StoragePolicy separating internal Preferences from operational microSD data
* InputManager for debounced physical buttons
* DisplayManager with serial rendering fallback
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

`build.sh` runs `scripts/validate_apps.py` before PlatformIO so AppManager registration problems fail early.

Asset and script validation:

```bash
scripts/validate_assets.py
```

Documentation validation:

```bash
scripts/validate_docs.py
```

Architecture boundary validation:

```bash
scripts/validate_architecture.py
```

PlatformIO include-path validation:

```bash
scripts/validate_platformio.py
```

Hardware pin conflict report:

```bash
scripts/check_hardware_config.py
```

Upload:

```bash
scripts/flash.sh
```

Prepare a development microSD tree:

```bash
scripts/generate_assets.py --clean
```

More details are in [`usage.md`](usage.md). Hardware validation steps are in [`docs/hardware.md`](docs/hardware.md).

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

Performance comes first. Serial rendering is a development fallback, not the final primary interface.

---

## Expansion Direction

Expansion modules are for specialized capabilities such as RF, Sub-GHz, IR, NFC, RFID, CAN, GPS, LoRa and sensors.

Future Sub-GHz work should target a serious tool level: scan, frequency sweep, live RSSI, raw capture, decoder support, signal database on microSD, dedicated UI, AppManager/ModuleManager/CaptureWriter integration and physically confirmed authorized replay with mandatory logs.

BadUSB is not a priority. If it is added later, it should be an external hardware module with native USB HID support, disabled by default, with physical confirmation and logs.

---

## Status

Phase: core firmware consolidation and hardware-facing architecture.

The project is moving from serial-first development toward a standalone professional portable device.
