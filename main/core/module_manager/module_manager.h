#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <Arduino.h>
#include "module_manifest.h"

void moduleManagerInit();

void moduleManagerRegister(ModuleType type);

bool moduleManagerHas(ModuleType type);
bool moduleManagerHas(const String& type);

void moduleManagerClearDetected();

void moduleManagerDetectMock();
void moduleManagerDetectByADC();

ModuleType moduleManagerTypeFromADC(int value);

int moduleManagerReadADCStable();

void moduleManagerPrintDetected();

void runModuleManager();

#endif