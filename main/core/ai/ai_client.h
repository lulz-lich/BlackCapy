#ifndef AI_CLIENT_H
#define AI_CLIENT_H

#include <Arduino.h>

struct AIGatewayConfig {
  bool enabled;
  String endpoint;
  String deviceToken;
  uint32_t timeoutMs;
};

void aiClientInit();
AIGatewayConfig aiClientLoadGatewayConfig();
bool aiClientGatewayConfigured();
void aiClientPrintStatus();
String aiClientBuildAnalysisPayload(const String& mode, const String& context);
bool aiClientSendAnalysis(const String& payload, String& response, String& error);

#endif
