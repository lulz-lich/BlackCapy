# BlackCapy Production QA

This document defines the final bench flow for a market release candidate.

---

## Release Package

Run:

```bash
scripts/release_gate.py
```

Expected output:

```txt
.pio/blackcapy_release/
├── RELEASE.txt
├── SHA256SUMS
├── firmware/
└── microsd/
```

Use `SHA256SUMS` to verify files before flashing or copying the microSD tree.

---

## Firmware Flash

Flash the firmware artifacts from:

```txt
.pio/blackcapy_release/firmware/
```

Verify boot on a powered target board before closing the enclosure.

---

## microSD Prep

Copy the contents of:

```txt
.pio/blackcapy_release/microsd/
```

to the root of the device microSD card.

Verify the card contains:

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

## Bench Acceptance

Pass criteria:

* Device boots without brownout resets
* TFT initializes and renders Home, Tools, Status and Settings
* ADC button ladder triggers UP, DOWN, LEFT, RIGHT, OK and BACK
* microSD initializes and writes a log
* AppManager reports all registered apps
* Module ADC ID detects known resistor values
* Locked apps remain locked when their module is absent
* CaptureWriter writes evidence for supported capture categories
* AI gateway config never stores provider API keys on the device

---

## Ship Hold

Hold shipment if any of these fail:

* `scripts/release_gate.py`
* `scripts/check_hardware_config.py --strict`
* TFT boot rendering
* Button ladder mapping
* microSD write test
* Power stability under expected module load
* Required local regulatory or certification review
