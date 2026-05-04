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

Or use the project wrapper:

```bash
scripts/build.sh
```

The wrapper respects `BLACKCAPY_ENV`; if unset, it uses `esp32dev`.

`scripts/build.sh` runs AppManager registration validation before compiling.

Manual validation:

```bash
scripts/validate_apps.py
scripts/validate_assets.py
scripts/validate_ai.py
scripts/validate_docs.py
scripts/validate_architecture.py
scripts/validate_platformio.py
scripts/validate_gitignore.py
scripts/check_hardware_config.py
```

---

## Upload Firmware

```bash
platformio run --target upload
```

Or use:

```bash
scripts/flash.sh
```

To select a serial port:

```bash
BLACKCAPY_PORT=/dev/ttyUSB0 scripts/flash.sh
```

---

## Clean Build Artifacts

```bash
scripts/clean.sh
```

---

## Prepare microSD Assets

Generate a ready-to-copy microSD directory tree:

```bash
scripts/generate_assets.py --clean
```

Output:

```txt
.pio/blackcapy_sdcard/
├── logs/
├── assets/
├── scripts/
├── captures/
├── signals/
├── ai/
├── reports/
├── prompts/
├── modules/
├── themes/
└── plugins/
```

Automation scripts are placed in `/scripts`, derived signal databases in `/signals`, AI gateway config in `/ai`, reports in `/reports`, prompts in `/prompts`, while UI assets and configs are placed under their runtime paths.

---

## App Scaffold Helper

`create_app.py` is intentionally conservative. It creates a minimal app scaffold only when `--write` is passed and never edits `main.cpp`, `blackcapy.h`, or AppManager registration automatically.

Preview:

```bash
scripts/create_app.py "CAN Sniffer" \
  --category modules \
  --permission APP_PERMISSION_CAN \
  --tool-id TOOL_CAN_SNIFFER
```

Create files after review:

```bash
scripts/create_app.py "CAN Sniffer" \
  --category modules \
  --permission APP_PERMISSION_CAN \
  --tool-id TOOL_CAN_SNIFFER \
  --write
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

## AI Gateway

Run the reference gateway on a trusted computer, Raspberry Pi or cloud host:

```bash
scripts/ai_gateway.py --host 0.0.0.0 --port 8787
```

Mock mode works without secrets. To enable OpenAI cloud analysis, set `OPENAI_API_KEY` on the gateway host and edit `/ai/gateway.cfg` on the microSD:

```txt
enabled=true
endpoint=http://<gateway-ip>:8787/analyze
device_token=
timeout_ms=12000
```

Then run on BlackCapy:

```txt
ai analyze
ai report
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
Use `apps`, `list` or `registry` in the serial shell to print the current
AppManager registry from firmware.
```

---

## Shell Commands

In addition to numeric selection, BlackCapy supports command-based interaction:

```txt
registry        → show AppManager registry
apps/list       → show AppManager registry
run <id>        → run app by ID
status          → show system status
profile         → show current profile
heap            → show memory usage
about           → show splash/info
clear_storage   → reset saved settings
script <name>   → run automation script from /scripts
theme           → show current theme
theme <name>    → set theme
capture stats   → show capture stat commands
capture export  → export capture category to CSV
analyze subghz  → analyze Sub-GHz capture data
spectrum subghz → show Sub-GHz spectrum summary
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
