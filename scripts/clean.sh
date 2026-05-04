#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ENV_NAME="${BLACKCAPY_ENV:-esp32dev}"

cd "$ROOT_DIR"

if ! command -v pio >/dev/null 2>&1; then
  echo "PlatformIO CLI not found. Install PlatformIO or add 'pio' to PATH." >&2
  exit 127
fi

echo "Cleaning BlackCapy PlatformIO build for environment: ${ENV_NAME}"
pio run -e "$ENV_NAME" -t clean "$@"
