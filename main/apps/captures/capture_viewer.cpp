#include <Arduino.h>

#include "filesystem.h"
#include "ui.h"
#include "logger.h"

static void printCaptureFile(const String& path) {
  Serial.println();
  Serial.print("----- ");
  Serial.print(path);
  Serial.println(" -----");

  String data = fileSystemRead(path);

  if (data.length() == 0) {
    Serial.println("No data.");
    return;
  }

  int start = data.length() - 1000;

  if (start < 0) {
    start = 0;
  }

  Serial.println(data.substring(start));
}

void runCaptureViewer() {
  Serial.println();
  Serial.println("========== CAPTURE VIEWER ==========");

  if (!fileSystemAvailable()) {
    uiShowStatusError("microSD unavailable.");
    logWarn("Capture viewer failed: microSD unavailable.");
    return;
  }

  printCaptureFile("/captures/ir.log");
  printCaptureFile("/captures/rf.log");
  printCaptureFile("/captures/rfid.log");
  printCaptureFile("/captures/nfc.log");
  printCaptureFile("/captures/can.log");
  printCaptureFile("/captures/gps.log");
  printCaptureFile("/captures/lora.log");

  Serial.println("====================================");
}