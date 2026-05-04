#include "ai_client.h"

#include <HTTPClient.h>
#include <WiFi.h>

#include "ai_policy.h"
#include "filesystem.h"
#include "logger.h"
#include "storage_policy.h"

static AIGatewayConfig currentConfig;

static String aiGatewayConfigPath() {
  return storagePolicyGetAIPath() + "/gateway.cfg";
}

static String getConfigValue(const String& config, const String& key) {
  int position = 0;

  while (position < config.length()) {
    int endOfLine = config.indexOf('\n', position);
    if (endOfLine < 0) {
      endOfLine = config.length();
    }

    String line = config.substring(position, endOfLine);
    line.trim();

    if (!line.startsWith("#") && line.startsWith(key + "=")) {
      String value = line.substring(key.length() + 1);
      value.trim();
      return value;
    }

    position = endOfLine + 1;
  }

  return String();
}

void aiClientInit() {
  currentConfig = aiClientLoadGatewayConfig();

  if (currentConfig.enabled) {
    logInfo("AI gateway enabled.");
  } else {
    logInfo("AI gateway disabled.");
  }
}

AIGatewayConfig aiClientLoadGatewayConfig() {
  AIGatewayConfig config;
  config.enabled = false;
  config.endpoint = "";
  config.deviceToken = "";
  config.timeoutMs = 8000;

  if (!fileSystemAvailable()) {
    return config;
  }

  String path = aiGatewayConfigPath();
  if (!fileSystemExists(path)) {
    return config;
  }

  String data = fileSystemRead(path);
  if (data.length() == 0) {
    return config;
  }

  String enabled = getConfigValue(data, "enabled");
  enabled.toLowerCase();
  config.enabled = enabled == "true" || enabled == "yes" || enabled == "1";
  config.endpoint = getConfigValue(data, "endpoint");
  config.deviceToken = getConfigValue(data, "device_token");

  String timeout = getConfigValue(data, "timeout_ms");
  if (timeout.length() > 0 && timeout.toInt() > 0) {
    config.timeoutMs = (uint32_t)timeout.toInt();
  }

  if (config.endpoint.length() == 0) {
    config.enabled = false;
  }

  return config;
}

bool aiClientGatewayConfigured() {
  currentConfig = aiClientLoadGatewayConfig();
  return currentConfig.enabled && currentConfig.endpoint.length() > 0;
}

String aiClientBuildAnalysisPayload(const String& mode, const String& context) {
  String safeContext = aiPolicyLimitText(aiPolicyRedactSensitiveText(context), 5000);

  String payload = "{";
  payload += "\"device\":\"BlackCapy\",";
  payload += "\"mode\":\"" + aiPolicyEscapeJSON(mode) + "\",";
  payload += "\"policy\":{";
  payload += "\"firmware_executes_ai_actions\":false,";
  payload += "\"requires_physical_confirmation_for_tx\":true,";
  payload += "\"raw_secret_redaction\":true";
  payload += "},";
  payload += "\"context\":\"" + aiPolicyEscapeJSON(safeContext) + "\"";
  payload += "}";

  return payload;
}

bool aiClientSendAnalysis(const String& payload, String& response, String& error) {
  currentConfig = aiClientLoadGatewayConfig();

  if (!currentConfig.enabled || currentConfig.endpoint.length() == 0) {
    error = "AI gateway is disabled or missing endpoint.";
    return false;
  }

  if (WiFi.status() != WL_CONNECTED) {
    error = "WiFi is not connected.";
    return false;
  }

  HTTPClient http;
  http.setTimeout(currentConfig.timeoutMs);

  if (!http.begin(currentConfig.endpoint)) {
    error = "Failed to initialize HTTP client.";
    return false;
  }

  http.addHeader("Content-Type", "application/json");
  if (currentConfig.deviceToken.length() > 0) {
    http.addHeader("X-BlackCapy-Token", currentConfig.deviceToken);
  }

  int status = http.POST(payload);
  if (status <= 0) {
    error = "Gateway request failed: " + String(status);
    http.end();
    return false;
  }

  response = http.getString();
  http.end();

  if (status < 200 || status >= 300) {
    error = "Gateway returned HTTP " + String(status);
    return false;
  }

  return true;
}
