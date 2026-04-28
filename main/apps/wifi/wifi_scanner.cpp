#include <Arduino.h>
#include <WiFi.h>

static String encryptionToString(wifi_auth_mode_t encryption) {
  switch (encryption) {
    case WIFI_AUTH_OPEN: return "OPEN";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA";
    case WIFI_AUTH_WPA2_PSK: return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-ENT";
    case WIFI_AUTH_WPA3_PSK: return "WPA3";
    case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
    default: return "UNKNOWN";
  }
}

void runWiFiScanner() {
  Serial.println();
  Serial.println("========== WIFI SCANNER ==========");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(300);

  Serial.println("Scanning networks...");
  int count = WiFi.scanNetworks(false, true);

  if (count <= 0) {
    Serial.println("No networks found.");
    Serial.println("==================================");
    return;
  }

  Serial.print("Networks found: ");
  Serial.println(count);
  Serial.println();

  for (int i = 0; i < count; i++) {
    Serial.print("[");
    Serial.print(i + 1);
    Serial.print("] ");

    Serial.print("SSID: ");
    Serial.print(WiFi.SSID(i));

    Serial.print(" | BSSID: ");
    Serial.print(WiFi.BSSIDstr(i));

    Serial.print(" | RSSI: ");
    Serial.print(WiFi.RSSI(i));
    Serial.print(" dBm");

    Serial.print(" | Channel: ");
    Serial.print(WiFi.channel(i));

    Serial.print(" | Encryption: ");
    Serial.println(encryptionToString(WiFi.encryptionType(i)));
  }

  WiFi.scanDelete();

  Serial.println("==================================");
}
