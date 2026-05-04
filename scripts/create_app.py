#!/usr/bin/env python3
"""Create a conservative BlackCapy app scaffold.

This script intentionally does not edit main.cpp or register the app.
AppManager remains the single source of truth: after reviewing the scaffold,
add one AppEntry manually to main/firmware/src/main.cpp.
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
APPS_DIR = ROOT / "main" / "apps"

VALID_CATEGORIES = {"system", "wireless", "hardware", "modules", "automation"}
VALID_PERMISSIONS = {
    "APP_PERMISSION_NONE",
    "APP_PERMISSION_WIFI",
    "APP_PERMISSION_BLE",
    "APP_PERMISSION_GPIO",
    "APP_PERMISSION_STORAGE",
    "APP_PERMISSION_MODULES",
    "APP_PERMISSION_SYSTEM",
    "APP_PERMISSION_IR",
    "APP_PERMISSION_RF",
    "APP_PERMISSION_RFID",
    "APP_PERMISSION_NFC",
    "APP_PERMISSION_CAN",
    "APP_PERMISSION_GPS",
    "APP_PERMISSION_LORA",
}
VALID_STATUS = {
    "APP_STATUS_STABLE",
    "APP_STATUS_EXPERIMENTAL",
    "APP_STATUS_DISABLED",
}


def snake_case(value: str) -> str:
    value = value.strip().lower()
    value = re.sub(r"[^a-z0-9]+", "_", value)
    return value.strip("_")


def pascal_case(value: str) -> str:
    return "".join(part.capitalize() for part in snake_case(value).split("_"))


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Create a minimal BlackCapy app scaffold without auto-registration."
    )
    parser.add_argument("name", help="Human app name, for example 'CAN Sniffer'")
    parser.add_argument(
        "--folder",
        help="Folder under main/apps. Defaults to snake_case(name).",
    )
    parser.add_argument(
        "--category",
        required=True,
        choices=sorted(VALID_CATEGORIES),
        help="AppManager category.",
    )
    parser.add_argument(
        "--permission",
        default="APP_PERMISSION_NONE",
        choices=sorted(VALID_PERMISSIONS),
        help="App permission enum.",
    )
    parser.add_argument(
        "--status",
        default="APP_STATUS_EXPERIMENTAL",
        choices=sorted(VALID_STATUS),
        help="App status enum.",
    )
    parser.add_argument(
        "--tool-id",
        required=True,
        help="Existing or planned ToolId enum name, for example TOOL_CAN_SNIFFER.",
    )
    parser.add_argument(
        "--description",
        default="Describe this BlackCapy app.",
        help="AppEntry description.",
    )
    parser.add_argument(
        "--write",
        action="store_true",
        help="Actually create files. Without this flag the script only previews.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    folder = snake_case(args.folder or args.name)
    function_suffix = pascal_case(args.name)
    function_name = f"run{function_suffix}"

    app_dir = APPS_DIR / folder
    header_path = app_dir / f"{folder}.h"
    source_path = app_dir / f"{folder}.cpp"
    guard = f"{folder.upper()}_H"

    header = f"""#ifndef {guard}
#define {guard}

void {function_name}();

#endif
"""

    source = f"""#include "{folder}.h"

#include <Arduino.h>

#include "logger.h"

void {function_name}() {{
  logInfo("{args.name} started.");
  Serial.println();
  Serial.println("========== {args.name.upper()} ==========");
  Serial.println("Implement app behavior here.");
  Serial.println("========================================");
  logInfo("{args.name} finished.");
}}
"""

    snippet = f"""AppEntry snippet for main/firmware/src/main.cpp:

{{
  {args.tool_id},
  "{args.name}",
  "{args.category}",
  "{args.description}",
  {args.permission},
  {args.status},
  {function_name}
}}
"""

    print(f"Target app directory: {app_dir.relative_to(ROOT)}")
    print(snippet)

    if not args.write:
      print("Dry run only. Re-run with --write to create the scaffold.")
      return 0

    app_dir.mkdir(parents=True, exist_ok=True)

    for path, content in ((header_path, header), (source_path, source)):
        if path.exists():
            raise SystemExit(f"Refusing to overwrite existing file: {path}")
        path.write_text(content, encoding="utf-8")
        print(f"Created {path.relative_to(ROOT)}")

    print("Review the files, then manually add the ToolId and AppEntry.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
