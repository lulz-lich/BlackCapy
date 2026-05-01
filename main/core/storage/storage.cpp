#include "storage.h"
#include "logger.h"
#include <Preferences.h>

static Preferences preferences;

void storageInit() {
  preferences.begin("blackcapy", false);
  logInfo("Storage initialized.");
}

void storageSetString(const String& key, const String& value) {
  preferences.putString(key.c_str(), value);
}

String storageGetString(const String& key, const String& defaultValue) {
  return preferences.getString(key.c_str(), defaultValue);
}

void storageSetBool(const String& key, bool value) {
  preferences.putBool(key.c_str(), value);
}

bool storageGetBool(const String& key, bool defaultValue) {
  return preferences.getBool(key.c_str(), defaultValue);
}

void storageSetInt(const String& key, int value) {
  preferences.putInt(key.c_str(), value);
}

int storageGetInt(const String& key, int defaultValue) {
  return preferences.getInt(key.c_str(), defaultValue);
}

void storageClear() {
  preferences.clear();
  logWarn("Storage cleared.");
}
