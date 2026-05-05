# BlackCapy 1.0.0 Release Checklist

This checklist defines the software release baseline for BlackCapy.

It does not replace target-board validation. Pinout, display wiring, module wiring, power behavior and RF behavior must still be tested on the actual hardware revision.

---

## Software Gates

Run:

```bash
scripts/build.sh
```

Required passing gates:

```txt
validate_apps.py
validate_assets.py
validate_ai.py
validate_docs.py
validate_architecture.py
validate_platformio.py
validate_gitignore.py
pio run
```

Optional hardware report:

```bash
scripts/check_hardware_config.py
```

This report is intentionally non-blocking because the development pin map may differ from the final board.

---

## Firmware Baseline

Expected firmware version:

```txt
BLACKCAPY_VERSION = 1.0.0
```

Expected core properties:

* `AppManager` is the single app source of truth
* microSD is native onboard storage
* direct `SD.*` access is isolated to `main/core/filesystem`
* `Logger` and `CaptureWriter` remain separate
* `CaptureWriter` categories are validated
* physical UI flow is Home, Tools, Status and Settings
* Tools renders 4 apps per page
* cloud AI requires gateway configuration and recent physical OK confirmation

---

## microSD Baseline

Generate the card tree:

```bash
scripts/generate_assets.py --clean
```

Expected root directories:

```txt
/logs
/assets
/scripts
/captures
/signals
/ai
/reports
/prompts
/modules
/themes
/plugins
```

---

## AI Gateway Baseline

Mock mode:

```bash
scripts/ai_gateway.py --host 0.0.0.0 --port 8787
```

OpenAI-backed mode:

```bash
export OPENAI_API_KEY="..."
export OPENAI_MODEL="gpt-5.2"
scripts/ai_gateway.py --host 0.0.0.0 --port 8787
```

The gateway must keep provider credentials off the ESP32.

---

## Hardware Release Blockers

Before calling a hardware build field-ready, resolve or intentionally document:

* GPIO conflicts in `hardware_config.h`
* display controller and pin mapping
* physical button mapping
* native microSD boot and write behavior
* ADC module ID resistor ladder values
* RF/Sub-GHz, IR, NFC, RFID, CAN, GPS and LoRa module wiring
* power stability under module load
