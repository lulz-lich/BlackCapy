#!/usr/bin/env python3
"""Report potential pin conflicts in hardware_config.h.

This is a report tool, not a build gate. Some shared buses are expected, but
chip-selects, buttons, status pins and reset/IRQ lines should be reviewed before
hardware production.
"""

from __future__ import annotations

import re
import argparse
from collections import defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "main" / "firmware" / "include" / "hardware_config.h"

PIN_DEFINE = re.compile(r"^#define\s+([A-Z0-9_]+_PIN)\s+([A-Z0-9_]+|\d+)\b")

ALLOWED_SHARED_GROUPS = (
    {"SPI_CS_PIN", "SD_CS_PIN"},
    {"UART_RX_PIN", "GPS_RX_PIN"},
    {"UART_TX_PIN", "GPS_TX_PIN"},
    {"EXPANSION_SPI_CS_PIN", "RFID_CS_PIN", "LORA_CS_PIN", "RF_CS_PIN", "CAN_CS_PIN"},
    {"EXPANSION_IRQ_PIN", "IR_RX_PIN", "NFC_IRQ_PIN", "LORA_DIO0_PIN", "RF_GDO0_PIN"},
    {"EXPANSION_RST_PIN", "RFID_RST_PIN", "NFC_RST_PIN", "LORA_RST_PIN", "RF_RST_PIN"},
    {"EXPANSION_IO1_PIN", "IR_TX_PIN", "PWM_TEST_PIN"},
    {"EXPANSION_IO2_PIN", "LORA_DIO1_PIN", "RF_GDO2_PIN"},
    {"EXPANSION_ANALOG_PIN", "ANALOG_TEST_PIN"},
)


def resolve_value(name: str, values: dict[str, str]) -> str:
    seen: set[str] = set()
    value = values[name]

    while value in values and value not in seen:
        seen.add(value)
        value = values[value]

    return value


def allowed(names: set[str]) -> bool:
    return any(names <= group for group in ALLOWED_SHARED_GROUPS)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Report potential BlackCapy pin conflicts.")
    parser.add_argument(
        "--strict",
        action="store_true",
        help="Return a non-zero exit code if unapproved conflicts are found.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    raw_values: dict[str, str] = {}

    for line in CONFIG.read_text(encoding="utf-8").splitlines():
        match = PIN_DEFINE.match(line.strip())
        if match:
            raw_values[match.group(1)] = match.group(2)

    pins: dict[str, list[str]] = defaultdict(list)

    for name in sorted(raw_values):
        pins[resolve_value(name, raw_values)].append(name)

    warnings = []
    for pin, names in sorted(pins.items(), key=lambda item: int(item[0]) if item[0].isdigit() else 999):
        name_set = set(names)
        if len(names) > 1 and not allowed(name_set):
            warnings.append((pin, names))

    if not warnings:
        print("No hardware pin conflicts detected.")
        return 0

    print("Potential hardware pin conflicts:")
    for pin, names in warnings:
        print(f"- GPIO {pin}: " + ", ".join(names))

    print()
    print("Review these before board production. Shared buses may be valid, but CS,")
    print("button, IRQ, reset, LED and buzzer reuse should be intentional.")
    return 1 if args.strict else 0


if __name__ == "__main__":
    raise SystemExit(main())
