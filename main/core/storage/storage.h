#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

void storageInit();

void storageSetString(const String& key, const String& value);
String storageGetString(const String& key, const String& defaultValue = "");

void storageSetBool(const String& key, bool value);
bool storageGetBool(const String& key, bool defaultValue = false);

void storageSetInt(const String& key, int value);
int storageGetInt(const String& key, int defaultValue = 0);

void storageClear();

#endif
