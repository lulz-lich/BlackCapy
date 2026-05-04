#include "storage_policy.h"

#include "filesystem.h"
#include "logger.h"

static bool sdAvailable = false;

void storagePolicyInit() {
  sdAvailable = fileSystemAvailable();

  if (sdAvailable) {
    logInfo("Storage policy: microSD active for operational data.");
  } else {
    logWarn("Storage policy: internal-only fallback mode.");
  }
}

bool storagePolicyUseSD() {
  return sdAvailable;
}

bool storagePolicyUseInternal() {
  return true;
}

void storagePolicyPrint() {
  Serial.println();
  Serial.println("========== STORAGE POLICY ==========");

  Serial.print("Internal Preferences: ");
  Serial.println("enabled");

  Serial.print("microSD: ");
  Serial.println(sdAvailable ? "available" : "not available");

  Serial.println();
  Serial.println("Internal storage:");
  Serial.println("- critical settings");
  Serial.println("- boot profile");
  Serial.println("- fallback config");

  Serial.println();
  Serial.println("microSD storage:");
  Serial.println("- logs");
  Serial.println("- assets");
  Serial.println("- scripts");
  Serial.println("- captures");
  Serial.println("- signals");
  Serial.println("- ai");
  Serial.println("- reports");
  Serial.println("- prompts");
  Serial.println("- modules");
  Serial.println("- themes");
  Serial.println("- plugins");

  Serial.println("====================================");
}

String storagePolicyGetLogsPath() {
  return "/logs";
}

String storagePolicyGetAssetsPath() {
  return "/assets";
}

String storagePolicyGetScriptsPath() {
  return "/scripts";
}

String storagePolicyGetCapturesPath() {
  return "/captures";
}

String storagePolicyGetSignalsPath() {
  return "/signals";
}

String storagePolicyGetAIPath() {
  return "/ai";
}

String storagePolicyGetReportsPath() {
  return "/reports";
}

String storagePolicyGetPromptsPath() {
  return "/prompts";
}

String storagePolicyGetModulesPath() {
  return "/modules";
}

String storagePolicyGetThemesPath() {
  return "/themes";
}

String storagePolicyGetPluginsPath() {
  return "/plugins";
}
