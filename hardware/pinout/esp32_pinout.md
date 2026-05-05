# BlackCapy ESP32 Pinout

This file documents the current development pin map from `main/firmware/include/hardware_config.h`.

The map is not production-final. Run the report tool before hardware work:

```bash
scripts/check_hardware_config.py
```

---

## Core Device Pins

| Function | Pin |
| --- | --- |
| Status LED | GPIO 2 |
| Buzzer | GPIO 13 |
| Button UP | GPIO 32 |
| Button DOWN | GPIO 33 |
| Button LEFT | GPIO 25 |
| Button RIGHT | GPIO 26 |
| Button OK | GPIO 27 |
| Button BACK | GPIO 14 |
| Module ADC ID | GPIO 36 |

---

## Buses

| Bus | Pin |
| --- | --- |
| I2C SDA | GPIO 21 |
| I2C SCL | GPIO 22 |
| SPI MOSI | GPIO 23 |
| SPI MISO | GPIO 19 |
| SPI SCK | GPIO 18 |
| Native microSD CS | GPIO 5 |
| Display TFT CS | GPIO 15 |
| Display TFT DC | GPIO 2 |
| Display TFT RST | GPIO 4 |
| UART RX | GPIO 16 |
| UART TX | GPIO 17 |

---

## Tool Defaults

These are configurable development defaults for hardware apps.

| Tool | Pin |
| --- | --- |
| PWM Generator | GPIO 18 |
| Analog Reader | GPIO 34 |

---

## External Module Pins

| Module | Pins |
| --- | --- |
| IR | RX GPIO 34, TX GPIO 4 |
| RFID RC522 | CS GPIO 15, RST GPIO 27 |
| NFC PN532 | IRQ GPIO 32, RST GPIO 33 |
| GPS | RX GPIO 16, TX GPIO 17 |
| LoRa SX127x | CS GPIO 5, DIO0 GPIO 26, RST GPIO 14, DIO1 GPIO 35 |
| RF CC1101 | CS GPIO 13, GDO0 GPIO 25, RST GPIO 12, GDO2 GPIO 39 |
| CAN MCP2515 | CS GPIO 2 |

---

## Known Development Conflicts

The current map intentionally exposes conflicts so firmware can compile before the board pinout is final.

Known conflicts include:

* Native microSD CS and LoRa CS both using GPIO 5
* Status LED and CAN CS both using GPIO 2
* Buzzer and RF CS both using GPIO 13
* Buttons sharing pins with NFC, RFID, LoRa and RF signal pins
* PWM test pin using SPI SCK
* Analog test pin sharing IR RX

Before board production, resolve these conflicts in `hardware_config.h` and rerun:

```bash
scripts/check_hardware_config.py
scripts/build.sh
```

---

## Production Guidance

For the production board:

* Keep native microSD on its own CS
* Keep physical buttons free from module IRQ/reset lines
* Give each SPI module a unique CS if modules can be connected together
* Avoid boot strap pins for active-low CS lines unless the board design handles pull states
* Keep ADC module ID isolated from user analog tools
* Confirm UART ownership between GPS and UART Monitor workflows
