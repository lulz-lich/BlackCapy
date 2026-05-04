# BlackCapy Tools

BlackCapy apps are registered through `AppManager` in `main/firmware/src/main.cpp`.

`AppManager` is the single source of truth. Do not create secondary app lists, `ToolManager`, `ToolRegistry` or `AppManagerV2`.

---

## AppEntry Contract

Every app entry uses seven fields:

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

Valid categories:

```txt
system
wireless
hardware
modules
automation
```

The build wrapper runs:

```bash
scripts/validate_apps.py
```

This validates AppEntry shape, ToolId usage, category, permission, status and run function availability.

---

## Current App Areas

### System

Examples:

* System Monitor
* System Benchmark
* Timer Tool
* Random Generator
* Settings
* Diagnostics
* Health Monitor
* Reboot

### Wireless

Examples:

* WiFi Scanner
* WiFi Monitor
* BLE Scanner

### Hardware

Examples:

* GPIO Console
* PWM Generator
* Analog Reader
* I2C Scanner
* UART Monitor

### Storage And Evidence

Examples:

* Storage Status
* Log Viewer
* Capture Viewer

Rules:

* System behavior goes through `Logger`
* Operational evidence goes through `CaptureWriter`
* Capture categories are limited to known operational domains

### Modules

Examples:

* Module Manager
* Module Manifest
* Module ID Diagnostics
* IR Console
* RF Analyzer
* SubGHz Scanner
* RFID Reader
* NFC Reader
* CAN Monitor
* GPS Status
* LoRa Status

Module-dependent apps must use module permissions such as `APP_PERMISSION_RF`, `APP_PERMISSION_NFC` or `APP_PERMISSION_CAN`.

---

## Adding A Tool

Use the scaffold helper for a dry run:

```bash
scripts/create_app.py "Example Tool" \
  --category hardware \
  --permission APP_PERMISSION_GPIO \
  --tool-id TOOL_EXAMPLE_TOOL
```

The helper does not edit registration automatically.

After reviewing the scaffold:

1. Add a `ToolId` in `main/firmware/include/blackcapy.h`
2. Add the app include in `main/firmware/src/main.cpp`
3. Add one `AppEntry`
4. Run `scripts/build.sh`

---

## UI Behavior

The Tools screen shows `APP_MANAGER_VISIBLE_ITEMS` apps per page. The current value is 4.

Navigation:

```txt
UP/DOWN      Select app
LEFT/RIGHT   Change page
OK           Run selected app
BACK         Return Home
```
