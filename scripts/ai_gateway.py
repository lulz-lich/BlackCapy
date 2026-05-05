#!/usr/bin/env python3
"""Reference BlackCapy AI gateway.

This server keeps provider API keys off the ESP32. The firmware posts redacted
field context here, and this gateway optionally forwards it to the OpenAI
Responses API.
"""

from __future__ import annotations

import argparse
import json
import os
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from typing import Any
from urllib import error, request


DEFAULT_HOST = "0.0.0.0"
DEFAULT_PORT = 8787
DEFAULT_MODEL = "gpt-5.2"
DEFAULT_PROMPT_FILE = Path("assets/prompts/field_analysis.txt")
OPENAI_RESPONSES_URL = "https://api.openai.com/v1/responses"


SYSTEM_INSTRUCTIONS = """\
You are BlackCapy AI Companion.
Analyze only the provided BlackCapy field context.
Do not recommend autonomous transmission, replay, bypass, persistence, credential theft, or unauthorized access.
For potentially sensitive hardware actions, require physical confirmation and mandatory logging.
Return concise, professional field notes with:
- summary
- notable signals or errors
- recommended next steps
- safety or authorization caveats
"""


def load_prompt_text(prompt_file: Path) -> str:
    if not prompt_file.exists():
        return SYSTEM_INSTRUCTIONS

    text = prompt_file.read_text(encoding="utf-8").strip()
    if not text:
        return SYSTEM_INSTRUCTIONS

    return text


def build_openai_payload(model: str, instructions: str, blackcapy_payload: dict[str, Any]) -> dict[str, Any]:
    return {
        "model": model,
        "instructions": instructions,
        "input": json.dumps(blackcapy_payload, ensure_ascii=False),
    }


def extract_output_text(response_body: dict[str, Any]) -> str:
    if isinstance(response_body.get("output_text"), str):
        return response_body["output_text"]

    chunks: list[str] = []
    for item in response_body.get("output", []):
        for content in item.get("content", []):
            text = content.get("text")
            if isinstance(text, str):
                chunks.append(text)

    return "\n".join(chunks).strip()


def call_openai(api_key: str, model: str, instructions: str, payload: dict[str, Any], timeout: int) -> str:
    body = json.dumps(build_openai_payload(model, instructions, payload)).encode("utf-8")
    req = request.Request(
        OPENAI_RESPONSES_URL,
        data=body,
        headers={
            "Authorization": f"Bearer {api_key}",
            "Content-Type": "application/json",
        },
        method="POST",
    )

    try:
        with request.urlopen(req, timeout=timeout) as resp:
            response_body = json.loads(resp.read().decode("utf-8"))
    except error.HTTPError as exc:
        detail = exc.read().decode("utf-8", errors="replace")
        raise RuntimeError(f"OpenAI HTTP {exc.code}: {detail}") from exc

    output = extract_output_text(response_body)
    if not output:
        raise RuntimeError("OpenAI response did not include output text.")

    return output


def mock_analysis(payload: dict[str, Any]) -> str:
    context = str(payload.get("context", ""))
    mode = payload.get("mode", "unknown")
    line_count = len([line for line in context.splitlines() if line.strip()])

    return (
        "BlackCapy AI Gateway mock response\n"
        f"Mode: {mode}\n"
        f"Context lines received: {line_count}\n"
        "Gateway is reachable. Set OPENAI_API_KEY to enable cloud analysis.\n"
        "No hardware action is authorized by this response."
    )


class BlackCapyGatewayHandler(BaseHTTPRequestHandler):
    server_version = "BlackCapyAIGateway/1.0"

    def _send_json(self, status: int, payload: dict[str, Any]) -> None:
        body = json.dumps(payload, ensure_ascii=False).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self) -> None:
        if self.path != "/health":
            self._send_json(404, {"ok": False, "error": "not found"})
            return

        self._send_json(200, {"ok": True, "service": "blackcapy-ai-gateway"})

    def do_POST(self) -> None:
        if self.path != "/analyze":
            self._send_json(404, {"ok": False, "error": "not found"})
            return

        content_length = int(self.headers.get("Content-Length", "0"))
        if content_length <= 0 or content_length > self.server.max_body_bytes:
            self._send_json(413, {"ok": False, "error": "invalid body size"})
            return

        try:
            payload = json.loads(self.rfile.read(content_length).decode("utf-8"))
        except json.JSONDecodeError:
            self._send_json(400, {"ok": False, "error": "invalid json"})
            return

        expected_token = self.server.device_token
        if expected_token:
            received_token = self.headers.get("X-BlackCapy-Token", "")
            if received_token != expected_token:
                self._send_json(401, {"ok": False, "error": "invalid device token"})
                return

        try:
            if self.server.openai_api_key:
                analysis = call_openai(
                    self.server.openai_api_key,
                    self.server.model,
                    self.server.prompt_text,
                    payload,
                    self.server.openai_timeout,
                )
                provider = "openai"
            else:
                analysis = mock_analysis(payload)
                provider = "mock"
        except Exception as exc:
            self._send_json(502, {"ok": False, "error": str(exc)})
            return

        self._send_json(
            200,
            {
                "ok": True,
                "provider": provider,
                "analysis": analysis,
                "requires_physical_confirmation_for_tx": True,
            },
        )

    def log_message(self, fmt: str, *args: Any) -> None:
        print(f"{self.address_string()} - {fmt % args}")


class BlackCapyGatewayServer(ThreadingHTTPServer):
    def __init__(
        self,
        server_address: tuple[str, int],
        handler_class: type[BaseHTTPRequestHandler],
        model: str,
        prompt_file: Path,
        device_token: str,
        max_body_bytes: int,
        openai_timeout: int,
    ) -> None:
        super().__init__(server_address, handler_class)
        self.model = model
        self.prompt_text = load_prompt_text(prompt_file)
        self.device_token = device_token
        self.max_body_bytes = max_body_bytes
        self.openai_timeout = openai_timeout
        self.openai_api_key = os.environ.get("OPENAI_API_KEY", "")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run the BlackCapy AI gateway.")
    parser.add_argument("--host", default=DEFAULT_HOST)
    parser.add_argument("--port", type=int, default=DEFAULT_PORT)
    parser.add_argument("--model", default=os.environ.get("OPENAI_MODEL", DEFAULT_MODEL))
    parser.add_argument("--prompt-file", type=Path, default=DEFAULT_PROMPT_FILE)
    parser.add_argument("--device-token", default=os.environ.get("BLACKCAPY_DEVICE_TOKEN", ""))
    parser.add_argument("--max-body-bytes", type=int, default=16_384)
    parser.add_argument("--openai-timeout", type=int, default=45)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    server = BlackCapyGatewayServer(
        (args.host, args.port),
        BlackCapyGatewayHandler,
        args.model,
        args.prompt_file,
        args.device_token,
        args.max_body_bytes,
        args.openai_timeout,
    )

    mode = "openai" if server.openai_api_key else "mock"
    print(f"BlackCapy AI gateway listening on http://{args.host}:{args.port}")
    print(f"Mode: {mode}")
    print(f"Prompt file: {args.prompt_file}")
    print("POST /analyze")
    print("GET  /health")

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down.")
    finally:
        server.server_close()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
