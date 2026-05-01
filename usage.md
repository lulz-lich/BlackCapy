# BlackCapy Usage

BlackCapy is a professional modular portable toolkit designed for hardware interaction, wireless analysis, automation, logging and expansion modules.

This document explains how to build, upload, run and use the current firmware.

---

## Requirements

* ESP32 development board
* PlatformIO (recommended)
* USB cable
* VSCode with PlatformIO extension
* Serial monitor set to `115200`

---

## Project Structure (Relevant)

```txt
main/
├── firmware/
│   ├── src/        → main firmware entry
│   └── include/    → core definitions
├── core/           → system modules (ui, logging, automation, etc)
├── apps/           → tools (wifi, gpio, diagnostics, etc)
```

Main entry point:

```txt
main/firmware/src/main.cpp
```

---

## Build Firmware

From the project root:

```bash
platformio run
```

---

## Upload Firmware

```bash
platformio run --target upload
```

---

## Serial Monitor

```bash
platformio device monitor
```

Baud rate:

```txt
115200
```

---

## Current Interface (Development Stage)

At the current stage, BlackCapy uses the Serial Monitor as the main interface.

Available interaction methods:

```txt
- Type tool number → run tool
- help             → show menu
- clear            → clear screen
```

---

## Available Tools

```txt
1  → System Monitor
2  → WiFi Scanner
3  → GPIO Console
4  → Module Manager
5  → Automation Engine
6  → BLE Scanner
7  → I2C Scanner
8  → System Benchmark
9  → Log Viewer
10 → Settings
11 → Diagnostics
12 → UART Monitor
13 → Health Monitor
14 → Reboot
```

---

## Shell Commands

In addition to numeric selection, BlackCapy supports command-based interaction:

```txt
registry        → show tool registry
status          → show system status
profile         → show current profile
heap            → show memory usage
about           → show splash/info
clear_storage   → reset saved settings
```

---

## Hybrid UI Concept

BlackCapy uses a hybrid interface design:

```txt
ASCII       → data, menus, logs
Pixel Art   → icons, splash, visual context
```

Currently:

```txt
- ASCII fully active
- Pixel-style visuals simulated in terminal
```

Future versions will render pixel art on the device display.

---

## Future Hardware Interface

BlackCapy will evolve to a dedicated hardware interface:

```txt
Display:     320x240
Navigation:  physical buttons (Flipper Zero style)
```

Planned behavior:

```txt
- Button-based navigation
- Fast menu switching
- Visual icons and pixel UI
- Serial reserved for debug/logging
```

---

## Workflow Example

```txt
1. Power device
2. Open Serial Monitor
3. Wait for menu
4. Select tool (e.g. 2 → WiFi Scanner)
5. View output
6. Run another tool or command
```

---

## Notes

```txt
- Serial interface is temporary as primary UI
- Core architecture is already modular
- Tools are designed to be extended via modules
- System is built with performance-first mindset
```

---

## Status

```txt
Phase: Core Firmware + System Expansion
```

BlackCapy is currently evolving from a serial-based toolkit into a full standalone embedded system.
