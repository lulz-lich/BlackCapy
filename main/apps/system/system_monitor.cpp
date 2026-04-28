#include <Arduino.h>
#include <WiFi.h>
#include "blackcapy.h"

void runSystemMonitor() {
  Serial.println();
  Serial.println("========== SYSTEM MONITOR ==========");

  Serial.print("Device: ");
  Serial.println(BLACKCAPY_NAME);

  Serial.print("Version: ");
  Serial.println(BLACKCAPY_VERSION);

  Serial.print("Chip model: ");
  Serial.println(ESP.getChipModel());

  Serial.print("CPU frequency: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");

  Serial.print("Flash size: ");
  Serial.print(ESP.getFlashChipSize());
  Serial.println(" bytes");

  Serial.print("Free heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");

  Serial.print("Minimum free heap: ");
  Serial.print(ESP.getMinFreeHeap());
  Serial.println(" bytes");

  Serial.print("WiFi MAC: ");
  Serial.println(WiFi.macAddress());

  Serial.print("Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" seconds");

  Serial.println("====================================");
}
