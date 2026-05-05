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
* Factory Test
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
* Button Calibration

### Storage And Evidence

Examples:

* Storage Status
* Log Viewer
* Capture Viewer
* Signal Database

Rules:

* System behavior goes through `Logger`
* Operational evidence goes through `CaptureWriter`
* Capture categories are limited to known operational domains
* Derived signal records live under `/signals`

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

### SubGHz Scanner

SubGHz Scanner performs a configured receive sweep, stores operational evidence through `CaptureWriter`, and can derive operator-friendly views from `/captures/subghz.log`.

Shell helpers:

```txt
run 22
analyze subghz
spectrum subghz
subghz db
```

`subghz db` writes `/signals/subghz_signals.csv` with timestamp, frequency, RSSI, payload length, HEX payload and Base64 payload. Timeouts and receive errors remain in the capture log, but are excluded from the signal database.

Signal Database shows the latest derived records from microSD. It does not replace raw captures and does not write evidence.

### Factory And Calibration

Factory Test verifies release-critical runtime assumptions from the device itself: firmware version, AppManager registry, heap reserve, WiFi mode, microSD availability, required assets, module ADC, button ADC and AI gateway config parsing.

Button Calibration samples `BTN_ADC_PIN`, prints expected ladder ranges from `hardware_config.h` and reports average, minimum and maximum ADC values. Use it when validating a new board revision or changing resistor values.

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
