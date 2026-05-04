#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ENV_NAME="${BLACKCAPY_ENV:-esp32dev}"
PORT="${BLACKCAPY_PORT:-}"

cd "$ROOT_DIR"

if ! command -v pio >/dev/null 2>&1; then
  echo "PlatformIO CLI not found. Install PlatformIO or add 'pio' to PATH." >&2
  exit 127
fi

cmd=(pio run -e "$ENV_NAME" -t upload)

if [[ -n "$PORT" ]]; then
  cmd+=(--upload-port "$PORT")
fi

echo "Flashing BlackCapy firmware for environment: ${ENV_NAME}"
"${cmd[@]}" "$@"
