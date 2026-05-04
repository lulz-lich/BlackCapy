#!/usr/bin/env python3
"""Validate BlackCapy AppManager registration consistency."""

from __future__ import annotations

import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MAIN_CPP = ROOT / "main" / "firmware" / "src" / "main.cpp"
BLACKCAPY_H = ROOT / "main" / "firmware" / "include" / "blackcapy.h"
APP_CONTRACT_H = ROOT / "main" / "core" / "app_contract" / "app_contract.h"


def strip_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    text = re.sub(r"//.*", "", text)
    return text


def extract_block(text: str, marker: str) -> str:
    start = text.index(marker)
    brace = text.index("{", start)
    depth = 0

    for index in range(brace, len(text)):
        char = text[index]
        if char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return text[brace + 1:index]

    raise ValueError(f"Could not parse block for {marker}")


def split_top_level_entries(block: str) -> list[str]:
    entries: list[str] = []
    start = None
    depth = 0
    in_string = False
    escaped = False

    for index, char in enumerate(block):
        if in_string:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == '"':
                in_string = False
            continue

        if char == '"':
            in_string = True
            continue

        if char == "{":
            if depth == 0:
                start = index + 1
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0 and start is not None:
                entries.append(block[start:index].strip())
                start = None

    return entries


def split_fields(entry: str) -> list[str]:
    fields: list[str] = []
    start = 0
    in_string = False
    escaped = False

    for index, char in enumerate(entry):
        if in_string:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == '"':
                in_string = False
            continue

        if char == '"':
            in_string = True
            continue

        if char == ",":
            fields.append(entry[start:index].strip())
            start = index + 1

    fields.append(entry[start:].strip())
    return fields


def enum_values(path: Path, enum_name: str) -> set[str]:
    text = strip_comments(path.read_text(encoding="utf-8"))
    block = extract_block(text, f"enum {enum_name}")
    values: set[str] = set()

    for item in block.split(","):
        name = item.split("=", 1)[0].strip()
        if name:
            values.add(name)

    return values


def source_contains_function(function_name: str) -> bool:
    pattern = re.compile(rf"\bvoid\s+{re.escape(function_name)}\s*\(")

    for path in (ROOT / "main").rglob("*.cpp"):
        if pattern.search(path.read_text(encoding="utf-8", errors="ignore")):
            return True

    return False


def main() -> int:
    main_cpp = strip_comments(MAIN_CPP.read_text(encoding="utf-8"))
    apps_block = extract_block(main_cpp, "AppEntry apps[]")
    entries = split_top_level_entries(apps_block)

    tool_ids = enum_values(BLACKCAPY_H, "ToolId")
    permissions = enum_values(APP_CONTRACT_H, "AppPermission")
    statuses = enum_values(APP_CONTRACT_H, "AppStatus")

    errors: list[str] = []
    seen_tool_ids: set[str] = set()

    for index, entry in enumerate(entries, start=1):
        fields = split_fields(entry)

        if len(fields) != 7:
            errors.append(f"App #{index} has {len(fields)} fields, expected 7.")
            continue

        tool_id, name, category, description, permission, status, function = fields

        if tool_id not in tool_ids:
            errors.append(f"{name}: unknown ToolId {tool_id}.")
        if tool_id in seen_tool_ids:
            errors.append(f"{name}: duplicate ToolId {tool_id}.")
        seen_tool_ids.add(tool_id)

        if not re.fullmatch(r'"[^"]+"', name):
            errors.append(f"{tool_id}: name must be a string literal.")
        if not re.fullmatch(r'"(system|wireless|hardware|modules|automation)"', category):
            errors.append(f"{tool_id}: invalid category {category}.")
        if not re.fullmatch(r'"[^"]+"', description):
            errors.append(f"{tool_id}: description must be a string literal.")
        if permission not in permissions:
            errors.append(f"{tool_id}: unknown permission {permission}.")
        if status not in statuses:
            errors.append(f"{tool_id}: unknown status {status}.")
        if not source_contains_function(function):
            errors.append(f"{tool_id}: run function not found: {function}.")

    unused_tool_ids = sorted(tool_ids - seen_tool_ids)
    if unused_tool_ids:
        errors.append("ToolIds without AppEntry: " + ", ".join(unused_tool_ids))

    if errors:
        print("App validation failed:")
        for error in errors:
            print(f"- {error}")
        return 1

    print(f"App validation passed: {len(entries)} apps registered.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
