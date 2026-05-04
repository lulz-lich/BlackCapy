#ifndef MODULE_MANIFEST_H
#define MODULE_MANIFEST_H

#include <Arduino.h>

enum ModuleType {
  MODULE_UNKNOWN,
  MODULE_IR,
  MODULE_RF,
  MODULE_RFID,
  MODULE_NFC,
  MODULE_CAN,
  MODULE_GPS,
  MODULE_LORA
};

struct ModuleManifest {
  ModuleType type;
  const char* name;
  const char* interface;
  const char* status;
  bool requiredForApp;
};

void moduleManifestInit();

const ModuleManifest* moduleManifestGet(ModuleType type);
bool moduleManifestIsSupported(ModuleType type);

const char* moduleTypeToString(ModuleType type);
ModuleType moduleTypeFromString(const String& type);

void moduleManifestPrintAll();
void moduleManifestPrint(ModuleManifest manifest);

#endif