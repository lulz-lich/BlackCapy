#include <Arduino.h>

#include "filesystem.h"
#include "ui.h"

void runLogViewer() {
  Serial.println();
  uiShowIconLogs();
  Serial.println("========== LOG VIEWER ==========");

  Serial.println("[INFO] BlackCapy boot completed");
  Serial.println("[INFO] Core services loaded");
  Serial.println("[INFO] AppManager initialized");
  Serial.print("[INFO] microSD ");
  Serial.println(fileSystemAvailable() ? "available" : "unavailable");
  Serial.println("[INFO] Hybrid UI enabled");
  Serial.println("[INFO] Waiting for operator command");

  Serial.println("================================");
}
