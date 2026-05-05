# BlackCapy Hardware Validation

This checklist captures the hardware tests required before calling a firmware build field-ready.

Serial logs are useful during validation, but the final product direction is the physical 320x240 display and button interface.

---

## Target Hardware

```txt
MCU: ESP32
Display: 320x240 SPI TFT, ILI9341-compatible by default
Buttons: UP, DOWN, LEFT, RIGHT, OK, BACK
Storage: native onboard microSD
Status: LED and buzzer
Expansion bus: I2C, SPI, UART, GPIO
Module ID: ADC resistor identification
```

---

## Boot

Verify:

* Device boots without brownout resets
* Serial baud is `115200`
* Logger starts
* EventBus emits boot event
* StatusManager starts in `IDLE`
* AppManager reports all registered apps

Expected serial indicators:

```txt
Logger initialized.
App manager initialized.
BlackCapy core loaded.
Professional toolkit ready.
```

---

## microSD

microSD is native onboard storage, not an external module.

Verify the firmware creates:

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

Run:

```bash
scripts/generate_assets.py --clean
```

Copy `.pio/blackcapy_sdcard/` to the microSD root before testing UI assets and automation scripts.

Expected behavior:

* Logger may write `/logs/system.log`
* CaptureWriter writes operational evidence to `/captures/*.log`
* Automation scripts load from `/scripts`
* UI assets load from `/assets`

---

## Buttons

Verify physical button mapping:

```txt
UP
DOWN
LEFT
RIGHT
OK
BACK
```

Expected behavior:

* Home, Tools, Status and Settings screens navigate with LEFT/RIGHT
* Tools screen shows 4 apps per page
* UP/DOWN changes selected app
* OK runs selected app if runnable
* BACK returns to Home

---

## Module Detection

Verify ADC resistor-ID detection for:

```txt
MODULE_IR
MODULE_RF
MODULE_RFID
MODULE_NFC
MODULE_CAN
MODULE_GPS
MODULE_LORA
```

Expected behavior:

* Unknown ADC values do not register modules
* Supported ADC ranges register exactly one module type
* Apps requiring missing modules remain locked
* Apps become runnable after their module is detected

---

## CaptureWriter vs Logger

Verify the separation:

* System events go to Logger
* Operational evidence goes to CaptureWriter

Examples:

```txt
Boot completed                 -> Logger
RF RSSI sample                 -> CaptureWriter
NFC UID                        -> CaptureWriter
CAN frame                      -> CaptureWriter
Module missing warning         -> Logger
```

---

## Expansion Bus

Verify bus availability and pin conflicts:

* I2C scanner runs without blocking
* SPI chip selects do not conflict between SD, RF, LoRa, CAN and RFID
* UART monitor does not interfere with debug serial unless intentionally configured
* GPIO apps avoid reserved display, SD and module-ID pins

---

## Display

Current firmware uses an Adafruit ILI9341 SPI TFT backend, with serial rendering kept as a debug mirror.

Default display pins are configured in `main/firmware/include/hardware_config.h`:

```txt
DISPLAY_TFT_CS_PIN  GPIO 15
DISPLAY_TFT_DC_PIN  GPIO 2
DISPLAY_TFT_RST_PIN GPIO 4
DISPLAY_TFT_BL_PIN  -1, unused by default
```

Verify:

* 320x240 initialization
* Text rendering
* Pixel/icon rendering
* `.anim` frame animation rendering
* Status bar rendering
* Screen refresh timing
* No overlap on Tools, Status and Settings screens

Performance comes before visual polish.
