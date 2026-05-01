#include "tool_registry.h"
#include "logger.h"

static ToolMetadata registeredTools[] = {
  {1, "System Monitor", "system", "Shows device information", true},
  {2, "WiFi Scanner", "wireless", "Scans nearby Wi-Fi networks", true},
  {3, "GPIO Console", "hardware", "Tests GPIO output", true},
  {4, "Module Manager", "modules", "Detects expansion modules", true},
  {5, "Automation Engine", "automation", "Shows automation rules", true},
  {6, "BLE Scanner", "wireless", "Scans BLE devices", true},
  {7, "I2C Scanner", "hardware", "Scans I2C devices", true},
  {8, "System Benchmark", "system", "Runs performance tests", true},
  {9, "Log Viewer", "system", "Shows recent runtime logs", true},
  {10, "Settings", "system", "Shows device settings", true},
  {11, "Diagnostics", "system", "Runs health checks", true},
  {12, "UART Monitor", "hardware", "Monitors UART input", true}
};

static const int registeredToolCount =
  sizeof(registeredTools) / sizeof(registeredTools[0]);

void registryInit() {
  logInfo("Tool registry initialized.");
}

void registryPrintTool(const ToolMetadata& tool) {
  Serial.print("[");
  Serial.print(tool.id);
  Serial.print("] ");

  Serial.print(tool.name);
  Serial.print(" | Category: ");
  Serial.print(tool.category);
  Serial.print(" | Status: ");
  Serial.println(tool.enabled ? "enabled" : "disabled");

  Serial.print("    ");
  Serial.println(tool.description);
}

void registryPrintAll() {
  Serial.println();
  Serial.println("========== TOOL REGISTRY ==========");

  for (int i = 0; i < registeredToolCount; i++) {
    registryPrintTool(registeredTools[i]);
  }

  Serial.println("===================================");
}