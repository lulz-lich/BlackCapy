# BlackCapy ESP32 Pinout

This file documents the production-oriented pin map from `main/firmware/include/hardware_config.h`.

Run the report tool before hardware work:

```bash
scripts/check_hardware_config.py --strict
```

---

## Core Device Pins

| Function | Pin |
| --- | --- |
| Status LED | GPIO 25 |
| Buzzer | GPIO 26 |
| Button ADC ladder | GPIO 39 |
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
| PWM Generator | GPIO 27 |
| Analog Reader | GPIO 34 |

---

## External Module Pins

| Module | Pins |
| --- | --- |
| Expansion SPI CS | GPIO 13 |
| Expansion IRQ | GPIO 32 |
| Expansion RST | GPIO 33 |
| Expansion IO1 | GPIO 27 |
| Expansion IO2 | GPIO 35 |
| Expansion analog | GPIO 34 |
| IR | RX GPIO 32, TX GPIO 27 |
| RFID RC522 | CS GPIO 13, RST GPIO 33 |
| NFC PN532 | IRQ GPIO 32, RST GPIO 33 |
| GPS | RX GPIO 16, TX GPIO 17 |
| LoRa SX127x | CS GPIO 13, DIO0 GPIO 32, RST GPIO 33, DIO1 GPIO 35 |
| RF CC1101 | CS GPIO 13, GDO0 GPIO 32, RST GPIO 33, GDO2 GPIO 35 |
| CAN MCP2515 | CS GPIO 13 |

---

## Intentional Sharing

The expansion module families share one external module slot. Shared CS, IRQ,
RST and auxiliary pins are intentional because `ModuleManager` detects one
inserted capability module at a time through the ADC ID ladder.

Before board production, rerun:

```bash
scripts/check_hardware_config.py --strict
scripts/build.sh
scripts/release_gate.py
```

---

## Production Guidance

For the production board:

* Keep native microSD on its own CS
* Keep the physical button ladder isolated from module ID ADC
* Give each onboard SPI device a unique CS
* Keep external capability modules mutually exclusive unless a later board revision adds more CS lines
* Avoid boot strap pins for active-low CS lines unless the board design handles pull states
* Keep ADC module ID isolated from user analog tools
* Confirm UART ownership between GPS and UART Monitor workflows
