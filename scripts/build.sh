#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ENV_NAME="${BLACKCAPY_ENV:-esp32dev}"

cd "$ROOT_DIR"

if ! command -v pio >/dev/null 2>&1; then
  echo "PlatformIO CLI not found. Install PlatformIO or add 'pio' to PATH." >&2
  exit 127
fi

echo "Building BlackCapy firmware for environment: ${ENV_NAME}"
python3 scripts/validate_apps.py
python3 scripts/validate_assets.py
python3 scripts/validate_ai.py
python3 scripts/validate_docs.py
python3 scripts/validate_architecture.py
python3 scripts/validate_platformio.py
python3 scripts/validate_gitignore.py
pio run -e "$ENV_NAME" "$@"
