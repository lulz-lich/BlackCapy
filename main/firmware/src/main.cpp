#include <Arduino.h>
#include <WiFi.h>

#include "hardware_config.h"

#include "blackcapy.h"
#include "logger.h"
#include "menu.h"
#include "automation_engine.h"
#include "module_manager.h"
#include "ui.h"
#include "storage.h"
#include "log_viewer.h"
#include "settings_app.h"
#include "event_bus.h"
#include "status_manager.h"
#include "diagnostics_app.h"
#include "uart_monitor.h"
#include "input_manager.h"
#include "display_manager.h"
#include "screen_manager.h"
#include "navigation.h"
#include "system_monitor.h"
#include "wifi_scanner.h"
#include "gpio_console.h"
#include "ble_scanner.h"
#include "i2c_scanner.h"
#include "system_benchmark.h"
#include "tool_registry.h"
#include "command_shell.h"
#include "health_monitor.h"
#include "ui_controller.h"
#include "app_manager.h"

void rebootDevice() {
  Serial.println();
  Serial.println("Rebooting BlackCapy...");
  delay(500);
  ESP.restart();
}

AppEntry apps[] = {
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
    TOOL_BLE_SCANNER,
    "BLE Scanner",
    "Scans nearby BLE devices",
    runBLEScanner
  },
  {
    TOOL_I2C_SCANNER,
    "I2C Scanner",
    "Scans I2C bus for devices",
    runI2CScanner
  },
  {
    TOOL_BENCHMARK,
    "System Benchmark",
    "Runs CPU and memory performance test",
    runSystemBenchmark
  },
  {
    TOOL_AUTOMATION_ENGINE,
    "Automation Engine",
    "Shows loaded automation rules and runtime status",
    runAutomationEngine
  },
  {
    TOOL_LOG_VIEWER,
    "Log Viewer",
    "Shows recent system events and runtime messages",
    runLogViewer
  },
  {
    TOOL_SETTINGS,
    "Settings",
    "Shows and saves BlackCapy runtime settings",
    runSettingsApp
  },
  {
    TOOL_DIAGNOSTICS,
    "Diagnostics",
    "Runs system health checks and runtime diagnostics",
    runDiagnosticsApp
  },
  {
    TOOL_UART_MONITOR,
    "UART Monitor",
    "Listens to UART2 RX data for a short diagnostic session",
    runUARTMonitor
  },
  {
  TOOL_HEALTH_MONITOR,
  "Health Monitor",
  "Shows runtime health and memory status",
  runHealthMonitor
  },
  {
  TOOL_WIFI_MONITOR,
  "WiFi Monitor",
  "Monitors signal strength in real-time",
  runWiFiMonitor
  },
  {
    TOOL_PWM_GENERATOR,
    "PWM Generator",
    "Generates PWM signal on GPIO",
    runPWMGenerator
  },
  {
    TOOL_ANALOG_READER,
    "Analog Reader",
    "Reads ADC values from input pin",
    runAnalogReader
  },
  {
    TOOL_TIMER,
    "Timer Tool",
    "Measures execution time",
    runTimerTool
  },
  {
    TOOL_RANDOM,
    "Random Generator",
    "Generates random values",
    runRandomTool
  },
  {
    TOOL_REBOOT,
    "Reboot",
    "Restarts the device",
    rebootDevice
  }
};

const int appCount = sizeof(apps) / sizeof(apps[0]);

void setup() {
  Serial.begin(BLACKCAPY_SERIAL_BAUD);
  delay(1000);

  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);

  loggerInit();
  storageInit();
  eventBusInit();
  statusInit();
  registryInit();

  inputInit();
  displayInit();
  screenManagerInit();

  appManagerInit(apps, appCount);

  menuInit();
  automationInit();
  healthMonitorInit();

  uiInit();
  uiControllerInit();

  eventEmit(EVENT_BOOT, "BlackCapy boot completed");

  menuPrintHeader();

  logInfo("Boot completed.");
  logInfo("BlackCapy core loaded.");
  logInfo("Professional toolkit ready.");

  menuPrint();
}
void loop() {
  healthMonitorTick();

  // Novo sistema de input físico
  uiControllerUpdate();
  
  // Mantém serial como debug
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    if (!shellHandleCommand(input)) {
      menuHandleInput(input);
    }
  }
}