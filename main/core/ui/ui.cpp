#include "ui.h"
#include "blackcapy.h"
#include "display_manager.h"

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

void uiShowIconRF() {
  Serial.println("[ RF ]");
  Serial.println("  /|");
  Serial.println(" / |");
  Serial.println("|++|");
  Serial.println(" \\ |");
  Serial.println("  \\|");
}

void uiShowIconSubGHz() {
  Serial.println("[ SubGHz ]");
  Serial.println("  .-.");
  Serial.println(" (   )");
  Serial.println("  `-'");
  Serial.println("  /|\\\\");
  Serial.println(" /_|_\\\\");
}

// Hybrid UI functions - pixel art with ASCII fallback
void uiShowIconWiFiHybrid() {
  displayDrawIconFromFile(0, 0, "icons/wifi_icon.bmp");
  uiShowIconWiFi(); // ASCII fallback
}

void uiShowIconBLEHybrid() {
  displayDrawIconFromFile(0, 0, "icons/ble_icon.bmp");
  uiShowIconBLE(); // ASCII fallback
}

void uiShowIconGPIOHybrid() {
  displayDrawIconFromFile(0, 0, "icons/gpio_icon.bmp");
  uiShowIconGPIO(); // ASCII fallback
}

void uiShowIconRFHybrid() {
  displayDrawIconFromFile(0, 0, "icons/rf_icon.bmp");
  uiShowIconRF(); // ASCII fallback
}

void uiShowIconSubGHzHybrid() {
  displayDrawIconFromFile(0, 0, "icons/subghz_icon.bmp");
  uiShowIconSubGHz(); // ASCII fallback
}

void uiShowLogoHybrid() {
  displayDrawIconFromFile(0, 0, "logo/blackcapy_logo.bmp");
  uiShowSplash(); // ASCII fallback
}

void uiDrawProgressBar(int current, int total, int width) {
  if (total <= 0) {
    return;
  }

  int filled = (current * width) / total;
  int percentage = (current * 100) / total;

  Serial.print("[");
  for (int i = 0; i < width; i++) {
    Serial.print(i < filled ? "=" : " ");
  }
  Serial.print("] ");
  Serial.print(percentage);
  Serial.println("%");
}

void uiDrawDataTable(const String* headers, int headerCount, const String** rows, int rowCount) {
  if (headerCount <= 0 || headers == nullptr) {
    return;
  }

  // Calculate column widths
  int* colWidths = new int[headerCount];
  for (int i = 0; i < headerCount; i++) {
    colWidths[i] = headers[i].length();
  }

  // Check row widths
  for (int r = 0; r < rowCount; r++) {
    if (rows == nullptr || rows[r] == nullptr) {
      continue;
    }
    const String* row = rows[r];
    for (int c = 0; c < headerCount; c++) {
      int len = row[c].length();
      if (len > colWidths[c]) {
        colWidths[c] = len;
      }
    }
  }

  // Print separator line
  Serial.print("┌");
  for (int i = 0; i < headerCount; i++) {
    for (int j = 0; j < colWidths[i] + 2; j++) {
      Serial.print("─");
    }
    Serial.print(i < headerCount - 1 ? "┬" : "┐");
  }
  Serial.println();

  // Print header
  Serial.print("│");
  for (int i = 0; i < headerCount; i++) {
    Serial.print(" ");
    Serial.print(headers[i]);
    for (int j = headers[i].length(); j < colWidths[i]; j++) {
      Serial.print(" ");
    }
    Serial.print(" │");
  }
  Serial.println();

  // Print header separator
  Serial.print("├");
  for (int i = 0; i < headerCount; i++) {
    for (int j = 0; j < colWidths[i] + 2; j++) {
      Serial.print("─");
    }
    Serial.print(i < headerCount - 1 ? "┼" : "┤");
  }
  Serial.println();

  // Print rows
  for (int r = 0; r < rowCount; r++) {
    if (rows == nullptr || rows[r] == nullptr) {
      continue;
    }
    const String* row = rows[r];
    Serial.print("│");
    for (int c = 0; c < headerCount; c++) {
      Serial.print(" ");
      Serial.print(row[c]);
      for (int j = row[c].length(); j < colWidths[c]; j++) {
        Serial.print(" ");
      }
      Serial.print(" │");
    }
    Serial.println();
  }

  // Print bottom separator
  Serial.print("└");
  for (int i = 0; i < headerCount; i++) {
    for (int j = 0; j < colWidths[i] + 2; j++) {
      Serial.print("─");
    }
    Serial.print(i < headerCount - 1 ? "┴" : "┘");
  }
  Serial.println();

  delete[] colWidths;
}
