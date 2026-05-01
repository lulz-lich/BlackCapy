#include <Arduino.h>
#include <WiFi.h>

#include "ui.h"
#include "status_manager.h"

void runDiagnosticsApp() {
  Serial.println();
  Serial.println("========== DIAGNOSTICS ==========");

  uiShowIconSystem();

  Serial.print("Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");

  Serial.print("Min heap: ");
  Serial.print(ESP.getMinFreeHeap());
  Serial.println(" bytes");

  Serial.print("CPU: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");

  Serial.print("Flash: ");
  Serial.print(ESP.getFlashChipSize());
  Serial.println(" bytes");

  Serial.print("WiFi mode: ");
  Serial.println(WiFi.getMode());

  Serial.print("WiFi MAC: ");
  Serial.println(WiFi.macAddress());

  statusPrint();

  Serial.println();
  Serial.println("Health check:");

  if (ESP.getFreeHeap() > 50000) {
    uiShowStatusOK("Memory level acceptable.");
  } else {
    uiShowStatusError("Low memory warning.");
  }

  uiShowStatusOK("Core runtime active.");
  uiShowStatusOK("Serial interface active.");

  Serial.println("=================================");
}