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
* Support development mock detection
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

## Pin Conflicts

The current `hardware_config.h` is a development map and still needs board-level review.

Known risk areas:

* Shared SPI chip selects between SD, RF, LoRa, CAN and RFID
* Button pins reused by NFC or RF interrupt pins
* Status LED or buzzer pins reused by module CS lines
* UART monitor and GPS sharing UART pins

Before hardware production, run the checklist in `docs/hardware.md` and resolve board-specific conflicts in `hardware_config.h`.

---

## Sub-GHz Direction

Future Sub-GHz support should target serious field capability:

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

Do not treat future Sub-GHz as simple packet capture only.
