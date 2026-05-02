#ifndef STORAGE_POLICY_H
#define STORAGE_POLICY_H

#include <Arduino.h>

void storagePolicyInit();

bool storagePolicyUseSD();
bool storagePolicyUseInternal();

void storagePolicyPrint();

String storagePolicyGetLogsPath();
String storagePolicyGetAssetsPath();
String storagePolicyGetScriptsPath();
String storagePolicyGetCapturesPath();
String storagePolicyGetModulesPath();

#endif