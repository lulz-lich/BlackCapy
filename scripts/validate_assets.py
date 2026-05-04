#!/usr/bin/env python3
"""Validate BlackCapy development assets and automation scripts."""

from __future__ import annotations

import argparse
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ASSETS = ROOT / "assets"
SCRIPTS = ROOT / "scripts"

SCRIPT_COMMANDS = {"delay", "print", "log", "gpio", "uart"}
SUBGHZ_CONFIG_KEYS = {"start_mhz", "end_mhz", "step_mhz", "timeout_ms", "log_format"}


def validate_bitmap(path: Path) -> list[str]:
    errors: list[str] = []
    width: int | None = None
    height = 0

    for line_number, raw_line in enumerate(path.read_text(encoding="utf-8").splitlines(), start=1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        if any(char not in "01" for char in line):
            errors.append(f"{path}: line {line_number} is not binary pixel data.")
            continue

        if width is None:
            width = len(line)
        elif len(line) != width:
            errors.append(
                f"{path}: line {line_number} width {len(line)} does not match {width}."
            )

        height += 1

    if width is None or height == 0:
        errors.append(f"{path}: no binary pixel data found.")

    return errors


def validate_script(path: Path) -> list[str]:
    errors: list[str] = []

    for line_number, raw_line in enumerate(path.read_text(encoding="utf-8").splitlines(), start=1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        command = line.split(maxsplit=1)[0].lower()
        if command not in SCRIPT_COMMANDS:
            errors.append(f"{path}: line {line_number} unknown command '{command}'.")
            continue

        parts = line.split()

        if command == "delay":
            if len(parts) != 2 or not parts[1].isdigit() or int(parts[1]) <= 0:
                errors.append(f"{path}: line {line_number} expected: delay <milliseconds>.")
        elif command in {"print", "log"}:
            if len(parts) < 2:
                errors.append(f"{path}: line {line_number} expected: {command} <text>.")
        elif command == "gpio":
            if len(parts) < 3:
                errors.append(f"{path}: line {line_number} expected gpio subcommand.")
            elif parts[1] == "read":
                if len(parts) != 3 or not parts[2].isdigit():
                    errors.append(f"{path}: line {line_number} expected: gpio read <pin>.")
            elif parts[1] == "set":
                if (
                    len(parts) != 4
                    or not parts[2].isdigit()
                    or parts[3].lower() not in {"high", "low", "1", "0"}
                ):
                    errors.append(f"{path}: line {line_number} expected: gpio set <pin> <high|low|1|0>.")
            else:
                errors.append(f"{path}: line {line_number} unknown gpio subcommand '{parts[1]}'.")
        elif command == "uart":
            if len(parts) < 4 or parts[1] != "write" or not parts[2].isdigit():
                errors.append(f"{path}: line {line_number} expected: uart write <port> <data>.")

    return errors


def validate_subghz_config(path: Path) -> list[str]:
    errors: list[str] = []
    seen: set[str] = set()

    for line_number, raw_line in enumerate(path.read_text(encoding="utf-8").splitlines(), start=1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        if "=" not in line:
            errors.append(f"{path}: line {line_number} expected key=value.")
            continue

        key, value = line.split("=", 1)
        key = key.strip()
        value = value.strip()

        if key not in SUBGHZ_CONFIG_KEYS:
            errors.append(f"{path}: line {line_number} unknown key '{key}'.")
        if not value:
            errors.append(f"{path}: line {line_number} has empty value.")

        seen.add(key)

    missing = SUBGHZ_CONFIG_KEYS - seen
    if missing:
        errors.append(f"{path}: missing keys: " + ", ".join(sorted(missing)))

    return errors


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Validate BlackCapy assets.")
    parser.add_argument(
        "--strict",
        action="store_true",
        help="Fail if no assets or scripts are found.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    errors: list[str] = []

    bitmap_files = sorted(ASSETS.rglob("*.bmp")) if ASSETS.exists() else []
    script_files = sorted(ASSETS.glob("*.txt")) if ASSETS.exists() else []
    script_files += sorted(SCRIPTS.glob("*.txt")) if SCRIPTS.exists() else []
    config_files = sorted(ASSETS.glob("subghz_scan.cfg")) if ASSETS.exists() else []

    if args.strict and not bitmap_files:
        errors.append("No bitmap assets found.")

    if args.strict and not script_files:
        errors.append("No automation scripts found.")

    for path in bitmap_files:
        errors.extend(validate_bitmap(path))

    for path in script_files:
        errors.extend(validate_script(path))

    for path in config_files:
        errors.extend(validate_subghz_config(path))

    if errors:
        print("Asset validation failed:")
        for error in errors:
            print(f"- {error}")
        return 1

    print(
        "Asset validation passed: "
        f"{len(bitmap_files)} bitmap assets, {len(script_files)} scripts, "
        f"{len(config_files)} configs."
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
