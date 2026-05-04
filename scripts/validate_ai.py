#!/usr/bin/env python3
"""Validate BlackCapy AI companion files."""

from __future__ import annotations

from pathlib import Path
import py_compile


ROOT = Path(__file__).resolve().parents[1]
GATEWAY_SCRIPT = ROOT / "scripts" / "ai_gateway.py"
GATEWAY_CONFIG = ROOT / "assets" / "ai" / "gateway.cfg"
FIELD_PROMPT = ROOT / "assets" / "prompts" / "field_analysis.txt"

CONFIG_KEYS = {"enabled", "endpoint", "device_token", "timeout_ms"}
FORBIDDEN_SECRET_MARKERS = ("sk-", "OPENAI_API_KEY=", "Bearer ")


def parse_config(path: Path) -> tuple[dict[str, str], list[str]]:
    values: dict[str, str] = {}
    errors: list[str] = []

    for line_number, raw_line in enumerate(path.read_text(encoding="utf-8").splitlines(), start=1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        if "=" not in line:
            errors.append(f"{path.relative_to(ROOT)}:{line_number}: expected key=value.")
            continue

        key, value = line.split("=", 1)
        key = key.strip()
        value = value.strip()

        if key not in CONFIG_KEYS:
            errors.append(f"{path.relative_to(ROOT)}:{line_number}: unknown key '{key}'.")

        values[key] = value

    missing = CONFIG_KEYS - set(values)
    if missing:
        errors.append(f"{path.relative_to(ROOT)}: missing keys: {', '.join(sorted(missing))}.")

    return values, errors


def main() -> int:
    errors: list[str] = []

    for path in (GATEWAY_SCRIPT, GATEWAY_CONFIG, FIELD_PROMPT):
        if not path.exists():
            errors.append(f"Missing AI file: {path.relative_to(ROOT)}")

    if GATEWAY_SCRIPT.exists():
        try:
            py_compile.compile(str(GATEWAY_SCRIPT), doraise=True)
        except py_compile.PyCompileError as exc:
            errors.append(f"{GATEWAY_SCRIPT.relative_to(ROOT)}: {exc.msg}")

    if GATEWAY_CONFIG.exists():
        values, config_errors = parse_config(GATEWAY_CONFIG)
        errors.extend(config_errors)

        enabled = values.get("enabled", "").lower()
        if enabled not in {"true", "false", "yes", "no", "1", "0"}:
            errors.append("assets/ai/gateway.cfg: enabled must be true/false, yes/no or 1/0.")

        timeout = values.get("timeout_ms", "")
        if not timeout.isdigit() or int(timeout) <= 0:
            errors.append("assets/ai/gateway.cfg: timeout_ms must be a positive integer.")

    if FIELD_PROMPT.exists():
        prompt = FIELD_PROMPT.read_text(encoding="utf-8")
        if len(prompt.strip()) < 40:
            errors.append("assets/prompts/field_analysis.txt: prompt is too short.")
        if "physical confirmation" not in prompt.lower():
            errors.append("assets/prompts/field_analysis.txt: prompt must require physical confirmation.")

    for path in (GATEWAY_CONFIG, FIELD_PROMPT):
        if not path.exists():
            continue

        text = path.read_text(encoding="utf-8", errors="ignore")
        for marker in FORBIDDEN_SECRET_MARKERS:
            if marker in text:
                errors.append(f"{path.relative_to(ROOT)} contains secret-like marker '{marker}'.")

    if errors:
        print("AI validation failed:")
        for error in errors:
            print(f"- {error}")
        return 1

    print("AI validation passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
