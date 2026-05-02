#include <Arduino.h>

#include "module_manager.h"
#include "module_manifest.h"
#include "hardware_config.h"

void runModuleIDDiagnostics() {
  Serial.println();
  Serial.println("========== MODULE ID DIAGNOSTICS ==========");

  int value = moduleManagerReadADCStable();
  ModuleType type = moduleManagerTypeFromADC(value);

  Serial.print("ADC pin: GPIO ");
  Serial.println(MODULE_ID_ADC_PIN);

  Serial.print("Stable ADC value: ");
  Serial.println(value);

  Serial.print("Detected type: ");
  Serial.println(moduleTypeToString(type));

  Serial.println();
  Serial.println("Expected ranges:");
  Serial.println("IR    : 300  - 600");
  Serial.println("RF    : 750  - 1050");
  Serial.println("RFID  : 1200 - 1450");
  Serial.println("NFC   : 1600 - 1850");
  Serial.println("CAN   : 2000 - 2250");
  Serial.println("GPS   : 2400 - 2650");
  Serial.println("LoRa  : 2800 - 3100");

  Serial.println("===========================================");
}