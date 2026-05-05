# BlackCapy Hardware BOM

This BOM is a working baseline for firmware and board planning.

It is not a procurement-locked production BOM.

---

## Core

| Item | Notes |
| --- | --- |
| ESP32 module/dev board | Current firmware target is `esp32dev` |
| 320x240 SPI TFT | ILI9341-compatible display backend |
| microSD socket | Native onboard storage |
| Six physical buttons | ADC resistor ladder for UP, DOWN, LEFT, RIGHT, OK, BACK |
| Status LED | Runtime state indication |
| Buzzer | Audible status/feedback |
| Pull resistors | Buttons, boot safety and module ID as required |

---

## Expansion Bus

Expose enough signals for specialized modules:

| Bus | Purpose |
| --- | --- |
| I2C | NFC, sensors, low-speed expanders |
| SPI | RF, LoRa, CAN, RFID and storage-class modules |
| UART | GPS, UART diagnostics and serial modules |
| GPIO | IR, interrupts, resets and module control |
| ADC ID | Resistor-based module identification |

---

## Supported Module Families

| Module | Example Hardware |
| --- | --- |
| IR | IR receiver/transmitter pair |
| RF/Sub-GHz | CC1101-class module |
| RFID | MFRC522 or compatible reader |
| NFC | PN532-class reader |
| CAN | MCP2515-based CAN module |
| GPS | UART GPS receiver |
| LoRa | SX127x-class module |

---

## Production Notes

Before ordering boards:

* Pass `scripts/check_hardware_config.py --strict`
* Confirm expansion slot wiring and mutually exclusive module policy
* Confirm display bus and backlight pins
* Confirm power budget for radio modules
* Confirm level shifting if needed
* Confirm enclosure access to buttons, SD and expansion bus
