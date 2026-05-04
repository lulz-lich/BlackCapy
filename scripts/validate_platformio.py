#!/usr/bin/env python3
"""Validate PlatformIO configuration paths used by BlackCapy."""

from __future__ import annotations

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PLATFORMIO = ROOT / "platformio.ini"


def main() -> int:
    errors: list[str] = []

    for line_number, raw_line in enumerate(PLATFORMIO.read_text(encoding="utf-8").splitlines(), start=1):
        line = raw_line.strip()

        if not line.startswith("-I "):
            continue

        include_path = line[3:].strip()
        if "*" in include_path:
            errors.append(f"line {line_number}: wildcard include paths are not supported: {include_path}")
            continue

        resolved = ROOT / include_path
        if not resolved.is_dir():
            errors.append(f"line {line_number}: include path does not exist: {include_path}")

    if errors:
        print("PlatformIO validation failed:")
        for error in errors:
            print(f"- {error}")
        return 1

    print("PlatformIO validation passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
