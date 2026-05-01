#include <Arduino.h>
#include <WiFi.h>
#include "ui.h"

void runWiFiMonitor() {
  Serial.println();
  uiShowIconWiFi();
  Serial.println("========== WIFI MONITOR ==========");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(200);

  Serial.println("Scanning networks...");

  int count = WiFi.scanNetworks();

  if (count <= 0) {
    Serial.println("No networks found.");
    return;
  }

  Serial.println("Monitoring strongest network for 10s...");
  int bestIndex = 0;

  for (int i = 1; i < count; i++) {
    if (WiFi.RSSI(i) > WiFi.RSSI(bestIndex)) {
      bestIndex = i;
    }
  }

  String ssid = WiFi.SSID(bestIndex);

  Serial.print("Target: ");
  Serial.println(ssid);

  WiFi.begin(ssid.c_str());

  unsigned long start = millis();

  while (millis() - start < 10000) {
    int rssi = WiFi.RSSI();

    Serial.print("RSSI: ");
    Serial.print(rssi);
    Serial.println(" dBm");

    delay(1000);
  }

  WiFi.disconnect(true);

  Serial.println("==================================");
}