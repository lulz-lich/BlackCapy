# BlackCapy Modules

Expansion modules provide specialized hardware capabilities.

microSD is native onboard storage and must not be modeled as an external module.

---

## Core Responsibilities

### ModuleManifest

Location:

```txt
main/core/module_manifest/
```

Defines supported module types and metadata.

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

### ModuleManager

Location:

```txt
main/core/module_manager/
```

Responsibilities:

* Register detected modules
* Reject unsupported module types
* Detect module type by ADC resistor ID
* Keep development mock detection disabled in production builds
* Answer `moduleManagerHas(type)` for AppManager runnable checks

---

## ADC Resistor ID Ranges

Current firmware mapping:

```txt
300-600     IR
750-1050    RF
1200-1450   RFID
1600-1850   NFC
2000-2250   CAN
2400-2650   GPS
2800-3100   LoRa
```

Unknown values must not register modules.

---

## Module Permissions

Module apps use app permissions:

```txt
APP_PERMISSION_IR
APP_PERMISSION_RF
APP_PERMISSION_RFID
APP_PERMISSION_NFC
APP_PERMISSION_CAN
APP_PERMISSION_GPS
APP_PERMISSION_LORA
```

`AppManager` checks these permissions against detected modules before running an app.

---

## Evidence Capture

Module apps that collect operational data must use `CaptureWriter`.

Current evidence-oriented module apps:

```txt
IR Console
RF Analyzer
RFID Reader
NFC Reader
CAN Monitor
GPS Status
LoRa Status
SubGHz Scanner
```

Logger remains responsible for system behavior.

---

## Pin Strategy

The production-oriented `hardware_config.h` uses one mutually exclusive expansion slot for external capability modules.

Intentional sharing:

* RF, LoRa, CAN and RFID share the expansion SPI CS line
* RF, LoRa, NFC and IR share expansion IRQ/input lines as appropriate
* RF, LoRa, RFID and NFC share the expansion reset line
* UART monitor and GPS sharing UART pins

Before hardware production, run `scripts/check_hardware_config.py --strict`, `scripts/release_gate.py` and the checklist in `docs/hardware.md`.

---

## Sub-GHz Direction

Sub-GHz support should target serious field capability:

* Scan
* Frequency sweep
* Live RSSI
* Raw capture
* Decoders
* Signal database on microSD
* Dedicated UI
* CaptureWriter evidence
* Physical confirmation for any authorized replay
* Mandatory system logs for transmission actions

Do not treat Sub-GHz as simple packet capture only.

Current Sub-GHz capture flow:

```txt
/scripts/subghz_scan.cfg
  -> SubGHz Scanner
  -> /captures/subghz.log
  -> analyze subghz
  -> spectrum subghz
  -> /signals/subghz_signals.csv
```

The signal database is generated with `subghz db`. It is intentionally receive-only and does not authorize or imply replay capability.
