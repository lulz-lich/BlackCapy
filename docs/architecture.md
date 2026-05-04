# BlackCapy Architecture

## Overview

BlackCapy is a professional modular portable toolkit built around the ESP32 platform.

Its architecture is designed around the following principles:

* High freedom
* Modularity
* Full hardware control
* Performance-first design
* Clean separation of responsibilities
* Professional usability
* Educational documentation

BlackCapy is inspired by Arch Linux principles, but unlike Arch, it is not designed for a niche audience.

The goal is to create a professional-grade platform that is:

* Powerful
* Flexible
* Accessible enough for wider adoption
* Viable for investment
* Educational through documentation

---

# Core Philosophy

## Performance First

Visual features must never reduce responsiveness.

Priority order:

```txt id="ljqzqa"
Performance
Architecture
Reliability
Usability
Visual Identity
```

---

## Hybrid Interface

BlackCapy uses a hybrid interface by default.

This is not a switchable mode.

Both are always combined:

### ASCII

Used for:

* Logs
* Technical data
* Menus
* Diagnostics
* Runtime information

### Pixel Art

Used for:

* Splash screens
* Icons
* Context visualization
* Status indicators
* Identity

---

## Hardware Target

Current hardware target:

```txt id="eutc8y"
MCU: ESP32
Display: 320x240
Navigation: Physical buttons
Style: Flipper Zero inspired
```

Serial communication is temporary and used mainly for:

* Development
* Debugging
* Diagnostics

It is not the final primary interface.

---

# Project Structure

```txt id="cvjlwm"
blackcapy/
├── platformio.ini
├── README.md
├── usage.md
├── docs/
│   └── architecture.md
│
├── main/
│
│   ├── firmware/
│   │   ├── src/
│   │   │   └── main.cpp
│   │   └── include/
│   │       ├── blackcapy.h
│   │       └── hardware_config.h
│
│   ├── core/
│   │
│   │   ├── app_contract/
│   │   ├── app_manager/
│   │   ├── automation/
│   │   ├── capture/
│   │   ├── display/
│   │   ├── events/
│   │   ├── filesystem/
│   │   ├── input/
│   │   ├── logging/
│   │   ├── menu/
│   │   ├── module_manager/
│   │   ├── module_manifest/
│   │   ├── screens/
│   │   ├── shell/
│   │   ├── status/
│   │   ├── storage/
│   │   ├── storage_policy/
│   │   ├── ui/
│   │   └── ui_controller/
│
│   └── apps/
│       ├── automation/
│       ├── ble/
│       ├── can/
│       ├── captures/
│       ├── diagnostics/
│       ├── gpio/
│       ├── gps/
│       ├── health/
│       ├── ir/
│       ├── logs/
│       ├── lora/
│       ├── modules/
│       ├── nfc/
│       ├── rf/
│       ├── rfid/
│       ├── settings/
│       ├── storage/
│       ├── system/
│       ├── uart/
│       └── wifi/
```

---

# Core Systems

## Logger

Location:

```txt id="0mvv9p"
main/core/logging/
```

Responsibilities:

* Timestamped logs
* Log levels
* Runtime diagnostics

Rules:

```txt id="ywcs1k"
Logger handles logs only.
Logger never controls UI.
Logger never executes apps.
```

Logger records system behavior. It does not replace `CaptureWriter`.

---

## Capture Writer

Location:

```txt
main/core/capture/
```

Responsibilities:

* Operational evidence
* Capture files under `/captures`
* Export and capture statistics helpers

Rules:

```txt
CaptureWriter stores evidence.
Logger stores system behavior.
Do not mix those responsibilities.
```

---

## Storage

Location:

```txt id="ev8qzj"
main/core/storage/
```

Responsibilities:

* Persistent settings
* Profiles
* Runtime configuration

Current implementation:

```txt id="q0ltx7"
ESP32 Preferences
```

Future expansion:

```txt id="v0byiw"
Encrypted storage
Session export
```

Native microSD storage is handled by `FileSystem` and `StoragePolicy`.

---

## FileSystem And StoragePolicy

Locations:

```txt
main/core/filesystem/
main/core/storage_policy/
```

Responsibilities:

* Initialize native onboard microSD
* Create operational directories
* Keep critical settings in Preferences
* Keep logs, assets, scripts, captures, modules, themes and plugins on microSD

Expected microSD directories:

```txt
/logs
/assets
/scripts
/captures
/modules
/themes
/plugins
```

---

## Event Bus

Location:

```txt id="0v3oxr"
main/core/events/
```

Responsibilities:

* System events
* Runtime notifications
* Inter-module communication

Examples:

```txt id="b72g3r"
BOOT
TOOL_STARTED
TOOL_FINISHED
MODULE_DETECTED
ERROR
```

---

## Status Manager

Location:

```txt id="5m1w7k"
main/core/status/
```

Responsibilities:

* Global runtime state

Examples:

```txt id="jlwm0s"
IDLE
RUNNING_TOOL
ERROR
```

---

## App Manager

Location:

```txt id="4aqx6n"
main/core/app_manager/
```

Responsibilities:

* App registration
* App metadata
* App execution
* App selection

Important:

```txt id="jndq6t"
AppManager is the single source of truth for all apps.
```

Never create:

```txt id="tynmx3"
ToolManager
ToolRegistryV2
Secondary app lists
```

`AppEntry` has seven fields:

```cpp
{
  TOOL_ID,
  "Name",
  "category",
  "description",
  APP_PERMISSION_*,
  APP_STATUS_*,
  runFunction
}
```

The Tools screen shows `APP_MANAGER_VISIBLE_ITEMS` apps per page. The current value is `4`.

---

## Module Manifest

Location:

```txt
main/core/module_manifest/
```

Responsibilities:

* Define supported external module types
* Provide module names and interfaces
* Convert module type strings

Supported module types:

```txt
MODULE_IR
MODULE_RF
MODULE_RFID
MODULE_NFC
MODULE_CAN
MODULE_GPS
MODULE_LORA
```

---

## Module Manager

Location:

```txt
main/core/module_manager/
```

Responsibilities:

* Register detected external modules
* Detect modules by ADC resistor ID
* Support development mock detection
* Answer whether a required module is available

---

## Input Manager

Location:

```txt id="j1ntes"
main/core/input/
```

Responsibilities:

* Physical button reading
* Debouncing
* Navigation events

Supported inputs:

```txt id="jz5p9d"
UP
DOWN
LEFT
RIGHT
OK
BACK
```

---

## Display Manager

Location:

```txt id="f8rz18"
main/core/display/
```

Responsibilities:

* Display abstraction
* Text rendering
* Pixel rendering
* Themes

Current state:

```txt id="7mbp5j"
Serial fallback rendering
```

Future state:

```txt id="dly0p2"
Real 320x240 display rendering
```

---

## Screen Manager

Location:

```txt id="68l1zt"
main/core/screens/
```

Responsibilities:

* Screen rendering
* Screen transitions

Current screens:

```txt id="2o95w3"
HOME
TOOLS
STATUS
SETTINGS
```

---

## UI Controller

Location:

```txt id="1w1c2j"
main/core/ui_controller/
```

Responsibilities:

* Connect buttons to screens
* Navigation logic

Rules:

```txt id="z6tfkc"
UI Controller never executes hardware directly.
```

---

# Applications

Applications live in:

```txt id="8tb98f"
main/apps/
```

Each app must follow:

```txt id="ftkkkq"
One app
One responsibility
One entry function
```

Example:

```cpp id="jupw02"
void runWiFiScanner();
```

---

# Current Apps

```txt id="rf9jke"
System Monitor
WiFi Scanner
GPIO Console
Module Manager
Automation Engine
BLE Scanner
I2C Scanner
System Benchmark
Log Viewer
Settings
Diagnostics
UART Monitor
Health Monitor
Reboot
```

---

# Hardware Configuration

Location:

```txt id="i9d6xj"
main/firmware/include/hardware_config.h
```

This file is the single source of truth for:

* Display resolution
* Button pins
* Status LED
* Buzzer
* I2C
* SPI
* UART

Never hardcode pins outside this file.

---

# Development Rules

## Rule 1

Never create duplicate systems.

Wrong:

```txt id="ydtqt7"
ToolManager
ButtonHandler
DisplayRendererV2
```

Correct:

```txt id="o9rjfe"
Refactor existing modules
```

---

## Rule 2

Never add features only to increase feature count.

Every feature must improve:

```txt id="34m7v0"
Architecture
Usability
Performance
Professional capability
```

---

## Rule 3

Before adding new functionality:

Ask:

```txt id="h7vw1g"
Can this be integrated into an existing module?
```

If yes:

```txt id="70bsl0"
Refactor
```

If no:

```txt id="9k3d4m"
Create new module
```

---

# Future Priorities

Current priorities:

```txt id="0yhn5n"
1. Main firmware cleanup
2. Button navigation
3. Real display integration
4. Pixel art rendering
5. Module ecosystem
6. Automation engine expansion
7. Professional documentation
```

BlackCapy is not being built as a demo.

It is being built as a professional platform.
