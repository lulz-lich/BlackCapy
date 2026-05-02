#include <Arduino.h>

#include "filesystem.h"
#include "storage_policy.h"
#include "ui.h"

void runStorageStatus() {
  Serial.println();
  Serial.println("========== STORAGE STATUS ==========");

  Serial.print("microSD available: ");
  Serial.println(fileSystemAvailable() ? "yes" : "no");

  storagePolicyPrint();

  if (fileSystemAvailable()) {
    uiShowStatusOK("Operational storage ready.");
  } else {
    uiShowStatusError("microSD unavailable. Running fallback mode.");
  }

  Serial.println("====================================");
}