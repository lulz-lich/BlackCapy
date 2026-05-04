#include <Arduino.h>

#include "ai_client.h"
#include "filesystem.h"
#include "input_manager.h"
#include "logger.h"
#include "storage_policy.h"
#include "ui.h"

static const unsigned long AI_PHYSICAL_CONFIRM_WINDOW_MS = 15000;

static String readOptionalFile(const String& path, int maxChars) {
  if (!fileSystemAvailable() || !fileSystemExists(path)) {
    return "";
  }

  String data = fileSystemRead(path);
  if (data.length() <= (unsigned)maxChars) {
    return data;
  }

  return data.substring(data.length() - maxChars);
}

static String buildAIContext() {
  String context;
  context.reserve(4096);

  context += "=== System Log Tail ===\n";
  context += readOptionalFile(storagePolicyGetLogsPath() + "/system.log", 1600);
  context += "\n\n=== SubGHz Signal Database Tail ===\n";
  context += readOptionalFile(storagePolicyGetSignalsPath() + "/subghz_signals.csv", 1800);
  context += "\n\n=== SubGHz Capture Tail ===\n";
  context += readOptionalFile(storagePolicyGetCapturesPath() + "/subghz.log", 1200);

  return context;
}

static void saveAIReport(const String& report) {
  if (!fileSystemAvailable()) {
    return;
  }

  String path = storagePolicyGetReportsPath() + "/ai_last_report.txt";
  if (!fileSystemWrite(path, report + "\n")) {
    logWarn("AI report save failed.");
  }
}

static int findJSONStringEnd(const String& text, int start) {
  bool escaped = false;

  for (int i = start; i < text.length(); i++) {
    char c = text[i];

    if (escaped) {
      escaped = false;
      continue;
    }

    if (c == '\\') {
      escaped = true;
      continue;
    }

    if (c == '"') {
      return i;
    }
  }

  return -1;
}

static String unescapeJSONString(const String& text) {
  String output;

  for (int i = 0; i < text.length(); i++) {
    char c = text[i];

    if (c != '\\' || i + 1 >= text.length()) {
      output += c;
      continue;
    }

    char next = text[++i];
    switch (next) {
      case 'n':
        output += '\n';
        break;
      case 'r':
        output += '\r';
        break;
      case 't':
        output += '\t';
        break;
      case '"':
      case '\\':
      case '/':
        output += next;
        break;
      default:
        output += next;
        break;
    }
  }

  return output;
}

static String extractAnalysisFromGatewayResponse(const String& response) {
  int key = response.indexOf("\"analysis\"");
  if (key < 0) {
    return response;
  }

  int colon = response.indexOf(':', key);
  if (colon < 0) {
    return response;
  }

  int quote = response.indexOf('"', colon + 1);
  if (quote < 0) {
    return response;
  }

  int end = findJSONStringEnd(response, quote + 1);
  if (end < 0) {
    return response;
  }

  return unescapeJSONString(response.substring(quote + 1, end));
}

void runAIAnalyzer() {
  Serial.println();
  Serial.println("========== AI ANALYZER ==========");

  if (!fileSystemAvailable()) {
    uiShowStatusError("microSD unavailable.");
    logWarn("AI analyzer blocked: microSD unavailable.");
    return;
  }

  String context = buildAIContext();
  if (context.length() == 0) {
    Serial.println("No local context found for AI analysis.");
    Serial.println("Collect logs, captures or signal records first.");
    Serial.println("=================================");
    return;
  }

  String payload = aiClientBuildAnalysisPayload("field_analysis", context);

  Serial.print("Gateway configured: ");
  Serial.println(aiClientGatewayConfigured() ? "yes" : "no");
  Serial.print("Payload bytes: ");
  Serial.println(payload.length());

  if (!aiClientGatewayConfigured()) {
    Serial.println();
    Serial.println("AI gateway disabled. Preview payload:");
    Serial.println(payload.substring(0, payload.length() > 1200 ? 1200 : payload.length()));
    Serial.println();
    Serial.println("Enable /ai/gateway.cfg to send analysis requests.");
    Serial.println("=================================");
    return;
  }

  if (!inputPhysicalOKRecentlyPressed(AI_PHYSICAL_CONFIRM_WINDOW_MS)) {
    Serial.println();
    Serial.println("Physical confirmation required before cloud analysis.");
    Serial.println("Press OK on the device, then run 'ai analyze' within 15 seconds.");
    Serial.println("No data was sent.");
    logWarn("AI analyzer blocked: missing physical confirmation.");
    Serial.println("=================================");
    return;
  }

  String response;
  String error;

  if (!aiClientSendAnalysis(payload, response, error)) {
    Serial.print("AI request failed: ");
    Serial.println(error);
    logWarn("AI analyzer request failed: " + error);
    Serial.println("=================================");
    return;
  }

  Serial.println();
  Serial.println("AI response:");
  String report = extractAnalysisFromGatewayResponse(response);
  Serial.println(report);
  saveAIReport(report);
  logInfo("AI analysis completed.");
  Serial.println("=================================");
}
