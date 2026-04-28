#include <Arduino.h>
#include <WiFi.h>

#include "blackcapy.h"

#include "logger.h"
#include "menu.h"
#include "automation_engine.h"
#include "module_manager.h"

#include "system_monitor.h"
#include "wifi_scanner.h"
#include "gpio_console.h"

void rebootDevice() {
  Serial.println();
  Serial.println("Rebooting BlackCapy...");
  delay(500);
  ESP.restart();
}

ToolEntry tools[] = {
  {
    TOOL_SYSTEM_MONITOR,
    "System Monitor",
    "Shows device, memory, chip and uptime information",
    runSystemMonitor
  },
  {
    TOOL_WIFI_SCANNER,
    "WiFi Scanner",
    "Scans nearby Wi-Fi networks and prints technical data",
    runWiFiScanner
  },
  {
    TOOL_GPIO_CONSOLE,
    "GPIO Console",
    "Runs basic GPIO status LED output test",
    runGPIOConsole
  },
  {
    TOOL_MODULE_MANAGER,
    "Module Manager",
    "Scans expansion interfaces and detects connected modules",
    runModuleManager
  },
  {
    TOOL_AUTOMATION_ENGINE,
    "Automation Engine",
    "Shows loaded automation rules and runtime status",
    runAutomationEngine
  },
  {
    TOOL_REBOOT,
    "Reboot",
    "Restarts the device",
    rebootDevice
  }
};

const int toolCount = sizeof(tools) / sizeof(tools[0]);

void setup() {
  Serial.begin(BLACKCAPY_SERIAL_BAUD);
  delay(1000);

  pinMode(BLACKCAPY_STATUS_LED, OUTPUT);
  digitalWrite(BLACKCAPY_STATUS_LED, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);

  loggerInit();
  menuInit();
  automationInit();

  menuRegisterTools(tools, toolCount);

  menuPrintHeader();

  logInfo("Boot completed.");
  logInfo("BlackCapy core loaded.");
  logInfo("Professional toolkit ready.");

  menuPrint();
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    menuHandleInput(input);
  }
}
