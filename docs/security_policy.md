# BlackCapy Security Policy

BlackCapy is a professional hardware and wireless analysis platform.

Use it only on systems, devices, networks and signals where you have authorization.

---

## Operating Principles

BlackCapy firmware should be designed around:

* Explicit operator action
* Physical confirmation for risky actions
* Clear logging of system behavior
* Evidence separation through CaptureWriter
* Conservative defaults
* Disabled-by-default behavior for sensitive capabilities

---

## Logging And Evidence

Use the correct channel:

```txt
Logger         System behavior
CaptureWriter  Operational evidence
```

Examples:

```txt
Tool started            -> Logger
Module missing          -> Logger
RF RSSI sample          -> CaptureWriter
CAN frame               -> CaptureWriter
NFC UID                 -> CaptureWriter
Export completed        -> Logger
```

Supported capture categories are intentionally restricted:

```txt
ir
rf
subghz
rfid
nfc
can
gps
lora
```

Serial capture export only accepts simple output filenames using letters, numbers, `_`, `-` and `.`. Path traversal is rejected.

---

## Transmission And Replay

Any future feature that transmits, replays or emulates signals must require:

* Explicit user selection
* Physical confirmation
* Clear UI warning
* Mandatory Logger event
* CaptureWriter evidence when operationally relevant
* Safe default disabled state

This applies especially to Sub-GHz replay, IR transmit, NFC/RFID emulation and similar capabilities.

---

## BadUSB

BadUSB is not a current priority.

If implemented later, it must be:

* An external hardware module
* Based on hardware with native USB HID support, such as ESP32-S3 or RP2040
* Disabled by default
* Physically confirmed before execution
* Logged through Logger
* Clearly separated from normal firmware flows

---

## Automation Scripts

Automation scripts are loaded from `/scripts` on the native microSD.

Rules:

* Keep the command set small and explicit
* Validate examples with `scripts/validate_assets.py`
* Avoid hidden network or radio side effects
* Prefer operator-visible output for actions

---

## Reporting Issues

When reporting a security issue, include:

* Firmware version or commit
* Hardware variant
* Steps to reproduce
* Expected behavior
* Actual behavior
* Logs or captures when safe to share

Do not include sensitive captures from third-party systems without permission.
