#include "ui.h"
#include "blackcapy.h"

void uiInit() {
  uiShowSplash();
}

void uiPrint(const String& text) {
  Serial.print(text);
}

void uiPrintLine(const String& text) {
  Serial.println(text);
}

void uiShowSplash() {
  Serial.println();
  Serial.println("########################################");
  Serial.println("#              BLACKCAPY               #");
  Serial.println("#        Professional Toolkit          #");
  Serial.println("########################################");
  Serial.println("        __");
  Serial.println("    ___/  \\___");
  Serial.println("   /  o    o  \\");
  Serial.println("  |     __     |");
  Serial.println("   \\___/  \\___/");
  Serial.println();
  Serial.print("Version: ");
  Serial.println(BLACKCAPY_VERSION);
  Serial.println();
}

void uiShowSeparator() {
  Serial.println("----------------------------------------");
}

void uiShowLoading(const String& label) {
  Serial.print(label);

  for (int i = 0; i < 3; i++) {
    Serial.print(".");
    delay(180);
  }

  Serial.println();
}

void uiShowStatusOK(const String& message) {
  Serial.print("[ OK ] ");
  Serial.println(message);
}

void uiShowStatusError(const String& message) {
  Serial.print("[FAIL] ");
  Serial.println(message);
}

void uiShowIconWiFi() {
  Serial.println("[ WiFi ]");
  Serial.println("   )))");
  Serial.println("  )))");
  Serial.println("  )))");
}

void uiShowIconBLE() {
  Serial.println("[ BLE ]");
  Serial.println("   /\\");
  Serial.println("  /  \\");
  Serial.println("  \\  /");
  Serial.println("   \\/");
}

void uiShowIconGPIO() {
  Serial.println("[ GPIO ]");
  Serial.println(" [==][==]");
  Serial.println(" [==][==]");
}

void uiShowIconSystem() {
  Serial.println("[ SYSTEM ]");
  Serial.println("  ______");
  Serial.println(" | ESP |");
  Serial.println(" |_____|");
}

void uiShowIconLogs() {
  Serial.println("[ LOGS ]");
  Serial.println("  ____");
  Serial.println(" | ## |");
  Serial.println(" |____|");
}
