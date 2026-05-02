#include <Arduino.h>

#include "module_manifest.h"
#include "logger.h"

void runModuleManifestViewer() {
  moduleManifestPrintAll();
  logInfo("Module manifest viewed.");
}