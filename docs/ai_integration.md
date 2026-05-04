# BlackCapy AI Integration

BlackCapy prepares data for AI analysis without placing provider secrets or autonomous decision-making inside the firmware.

The firmware remains responsible for hardware control, logging, captures and UI. The AI layer is a companion analysis path reached through a gateway server.

---

## Future AI Use Cases

Potential AI-assisted workflows:

* Log summarization
* Error diagnosis
* Capture summarization
* Automation suggestions
* Contextual help per app
* Module troubleshooting
* Pattern recognition in exported evidence

---

## Firmware Boundary

Do not embed a coupled AI runtime or provider API key into the ESP32 firmware.

Firmware should provide:

* Structured logs
* Capture files
* Signal databases
* Clear app metadata
* Module metadata
* Exportable data
* Stable file paths on microSD

The ESP32 can send a redacted context bundle to a configured gateway. The gateway owns OpenAI or other provider credentials.

---

## Data Sources

Useful future inputs:

```txt
/logs/system.log
/captures/*.log
/signals/*.csv
/modules/*
/scripts/*
/ai/gateway.cfg
/prompts/*.txt
/reports/*.txt
AppManager metadata
ModuleManifest metadata
```

---

## Current Firmware Support

AppManager registers `AI Analyzer`.

Serial shell:

```txt
ai
ai status
ai analyze
ai report
analyze ai
run 30
```

microSD configuration:

```txt
/ai/gateway.cfg
```

Example:

```txt
enabled=false
endpoint=http://blackcapy-gateway.local/analyze
device_token=
timeout_ms=8000
```

When disabled, the app prints a payload preview only. When enabled, it sends a redacted JSON payload to the gateway and stores the extracted `analysis` text in:

```txt
/reports/ai_last_report.txt
```

The payload includes policy flags stating that AI does not execute firmware actions and that transmission-like actions require physical confirmation.

When the gateway is enabled, firmware also requires a recent physical OK button press before sending context to the gateway. Serial-only execution without recent physical confirmation is blocked.

---

## Reference Gateway

The repository includes a no-dependency reference gateway:

```bash
scripts/ai_gateway.py --host 0.0.0.0 --port 8787
```

Without `OPENAI_API_KEY`, it runs in mock mode and proves that the ESP32 can reach the gateway.

With OpenAI enabled:

```bash
export OPENAI_API_KEY="sk-..."
export OPENAI_MODEL="gpt-5.2"
scripts/ai_gateway.py --host 0.0.0.0 --port 8787
```

Optional device token:

```bash
export BLACKCAPY_DEVICE_TOKEN="change-me"
scripts/ai_gateway.py --device-token "$BLACKCAPY_DEVICE_TOKEN"
```

Configure the microSD file:

```txt
enabled=true
endpoint=http://<gateway-ip>:8787/analyze
device_token=change-me
timeout_ms=12000
```

Health check:

```bash
curl http://<gateway-ip>:8787/health
```

Manual test:

```bash
curl -X POST http://<gateway-ip>:8787/analyze \
  -H "Content-Type: application/json" \
  -d '{"device":"BlackCapy","mode":"field_analysis","context":"test"}'
```

The gateway response is JSON. Firmware extracts the `analysis` field for display and stores that text in `/reports/ai_last_report.txt`.

`AI Report Viewer` and the `ai report` shell command show the latest saved report without contacting the gateway.

`ai status` shows gateway configuration without printing the device token value.

---

## Design Rule

AI is an analysis layer, not a replacement for deterministic firmware behavior.

Critical hardware actions must remain explicit, auditable and physically confirmed when risk warrants it.
