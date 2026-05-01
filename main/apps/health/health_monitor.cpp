#include <Arduino.h>
#include "logger.h"
#include "ui.h"

static unsigned long lastHealthCheck = 0;
static const unsigned long healthInterval = 10000;

void healthMonitorInit() {
  lastHealthCheck = millis();
  logInfo("Health monitor initialized.");
}

void healthMonitorTick() {
  if (millis() - lastHealthCheck < healthInterval) {
    return;
  }

  lastHealthCheck = millis();

  if (ESP.getFreeHeap() < 30000) {
    logWarn("Low heap detected.");
  }
}

void runHealthMonitor() {
  Serial.println();
  Serial.println("========== HEALTH MONITOR ==========");

  Serial.print("Free heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");

  Serial.print("Minimum free heap: ");
  Serial.print(ESP.getMinFreeHeap());
  Serial.println(" bytes");

  Serial.print("Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" seconds");

  if (ESP.getFreeHeap() >= 50000) {
    uiShowStatusOK("Memory status normal.");
  } else {
    uiShowStatusError("Memory level is low.");
  }

  uiShowStatusOK("Runtime active.");
  uiShowStatusOK("Core services responding.");

  Serial.println("====================================");
}