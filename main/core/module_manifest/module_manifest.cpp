#include "module_manifest.h"

#include "logger.h"

static ModuleManifest manifests[] = {
  {
    MODULE_IR,
    "IR Module",
    "GPIO",
    "external",
    true
  },
  {
    MODULE_RF,
    "RF Module",
    "SPI",
    "external",
    true
  },
  {
    MODULE_RFID,
    "RFID-LF/HF Module",
    "SPI/UART",
    "external",
    true
  },
  {
    MODULE_NFC,
    "NFC-HF Module",
    "I2C/SPI",
    "external",
    true
  },
  {
    MODULE_CAN,
    "CAN Bus Module",
    "SPI",
    "external",
    true
  },
  {
    MODULE_GPS,
    "GPS Module",
    "UART",
    "external",
    true
  },
  {
    MODULE_LORA,
    "LoRa Module",
    "SPI",
    "external",
    true
  }
};

static const int manifestCount = sizeof(manifests) / sizeof(manifests[0]);

void moduleManifestInit() {
  logInfo("Module manifest initialized.");
}

const char* moduleTypeToString(ModuleType type) {
  switch (type) {
    case MODULE_IR:
      return "ir";

    case MODULE_RF:
      return "rf";

    case MODULE_RFID:
      return "rfid";

    case MODULE_NFC:
      return "nfc";

    case MODULE_CAN:
      return "can";

    case MODULE_GPS:
      return "gps";

    case MODULE_LORA:
      return "lora";

    default:
      return "unknown";
  }
}

ModuleType moduleTypeFromString(const String& type) {
  if (type == "ir") return MODULE_IR;
  if (type == "rf") return MODULE_RF;
  if (type == "rfid") return MODULE_RFID;
  if (type == "nfc") return MODULE_NFC;
  if (type == "can") return MODULE_CAN;
  if (type == "gps") return MODULE_GPS;
  if (type == "lora") return MODULE_LORA;

  return MODULE_UNKNOWN;
}

void moduleManifestPrint(ModuleManifest manifest) {
  Serial.print(moduleTypeToString(manifest.type));
  Serial.print(" | ");
  Serial.print(manifest.name);
  Serial.print(" | interface: ");
  Serial.print(manifest.interface);
  Serial.print(" | status: ");
  Serial.println(manifest.status);
}

void moduleManifestPrintAll() {
  Serial.println();
  Serial.println("========== MODULE MANIFEST ==========");

  for (int i = 0; i < manifestCount; i++) {
    moduleManifestPrint(manifests[i]);
  }

  Serial.println("=====================================");
}