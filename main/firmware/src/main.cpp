#include <Arduino.h>
#include <WiFi.h>

#include "blackcapy.h"
#include "hardware_config.h"

#include "logger.h"
#include "menu.h"
#include "storage.h"
#include "event_bus.h"
#include "status_manager.h"
#include "command_shell.h"

#include "input_manager.h"
#include "display_manager.h"
#include "screen_manager.h"
#include "ui.h"
#include "ui_controller.h"

#include "app_manager.h"

#include "automation_engine.h"
#include "module_manager.h"

#include "system_monitor.h"
#include "wifi_scanner.h"
#include "wifi_monitor.h"
#include "module_id_diagnostics.h"
#include "gpio_console.h"
#include "pwm_generator.h"
#include "analog_reader.h"

#include "ble_scanner.h"

#include "i2c_scanner.h"
#include "system_benchmark.h"
#include "timer_tool.h"
#include "random_tool.h"

#include "log_viewer.h"
#include "module_manifest.h"
#include "module_manifest_viewer.h"
#include "settings_app.h"
#include "ir_console.h"
#include "rf_analyzer.h"
#include "subghz_scanner.h"
#include "rfid_reader.h"
#include "nfc_reader.h"
#include "can_monitor.h"
#include "gps_status.h"
#include "lora_status.h"
#include "diagnostics_app.h"
#include "storage_policy.h"
#include "storage_status.h"
#include "uart_monitor.h"
#include "filesystem.h"
#include "health_monitor.h"
#include "capture_viewer.h"
#include "signal_database_viewer.h"
#include "ai_client.h"
#include "ai_analyzer.h"
#include "ai_report_viewer.h"

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
    "system",
    "Shows device, memory, chip and uptime information",
    APP_PERMISSION_SYSTEM,
    APP_STATUS_STABLE,
    runSystemMonitor
  },
  {
    TOOL_WIFI_SCANNER,
    "WiFi Scanner",
    "wireless",
    "Scans nearby Wi-Fi networks and prints technical data",
    APP_PERMISSION_WIFI,
    APP_STATUS_STABLE,
    runWiFiScanner
  },
  {
    TOOL_GPIO_CONSOLE,
    "GPIO Console",
    "hardware",
    "Runs basic GPIO status LED output test",
    APP_PERMISSION_GPIO,
    APP_STATUS_STABLE,
    runGPIOConsole
  },
  {
    TOOL_MODULE_MANAGER,
    "Module Manager",
    "modules",
    "Scans expansion interfaces and detects connected modules",
    APP_PERMISSION_MODULES,
    APP_STATUS_STABLE,
    runModuleManager
  },
  {
    TOOL_MODULE_ID_DIAGNOSTICS,
    "Module ID Diagnostics",
    "modules",
    "Reads ADC ID and verifies module resistor range",
    APP_PERMISSION_MODULES,
    APP_STATUS_EXPERIMENTAL,
    runModuleIDDiagnostics
  },
  {
    TOOL_BLE_SCANNER,
    "BLE Scanner",
    "wireless",
    "Scans nearby BLE devices",
    APP_PERMISSION_BLE,
    APP_STATUS_EXPERIMENTAL,
    runBLEScanner
  },
  {
    TOOL_I2C_SCANNER,
    "I2C Scanner",
    "hardware",
    "Scans I2C bus for devices",
    APP_PERMISSION_GPIO,
    APP_STATUS_STABLE,
    runI2CScanner
  },
  {
    TOOL_BENCHMARK,
    "System Benchmark",
    "system",
    "Runs CPU and memory performance tests",
    APP_PERMISSION_SYSTEM,
    APP_STATUS_STABLE,
    runSystemBenchmark
  },
  {
    TOOL_AUTOMATION_ENGINE,
    "Automation Engine",
    "automation",
    "Shows loaded automation rules and runtime status",
    APP_PERMISSION_SYSTEM,
    APP_STATUS_EXPERIMENTAL,
    runAutomationEngine
  },
  {
    TOOL_LOG_VIEWER,
    "Log Viewer",
    "system",
    "Shows recent system events and runtime messages",
    APP_PERMISSION_STORAGE,
    APP_STATUS_STABLE,
    runLogViewer
  },
  {
    TOOL_SETTINGS,
    "Settings",
    "system",
    "Shows and saves BlackCapy runtime settings",
    APP_PERMISSION_SYSTEM,
    APP_STATUS_STABLE,
    runSettingsApp
  },
  {
    TOOL_DIAGNOSTICS,
    "Diagnostics",
    "system",
    "Runs system health checks and runtime diagnostics",
    APP_PERMISSION_SYSTEM,
    APP_STATUS_STABLE,
    runDiagnosticsApp
  },
  {
    TOOL_UART_MONITOR,
    "UART Monitor",
    "hardware",
    "Listens to UART RX data for diagnostics",
    APP_PERMISSION_GPIO,
    APP_STATUS_STABLE,
    runUARTMonitor
  },
  {
    TOOL_HEALTH_MONITOR,
    "Health Monitor",
    "system",
    "Shows runtime health and memory status",
    APP_PERMISSION_SYSTEM,
    APP_STATUS_STABLE,
    runHealthMonitor
  },
  {
    TOOL_WIFI_MONITOR,
    "WiFi Monitor",
    "wireless",
    "Monitors Wi-Fi signal strength in real time",
    APP_PERMISSION_WIFI,
    APP_STATUS_STABLE,
    runWiFiMonitor
  },
  {
    TOOL_PWM_GENERATOR,
    "PWM Generator",
    "hardware",
    "Generates PWM signals on GPIO pins",
    APP_PERMISSION_GPIO,
    APP_STATUS_STABLE,
    runPWMGenerator
  },
  {
    TOOL_ANALOG_READER,
    "Analog Reader",
    "hardware",
    "Reads analog values from ADC pins",
    APP_PERMISSION_GPIO,
    APP_STATUS_STABLE,
    runAnalogReader
  },
  {
    TOOL_TIMER,
    "Timer Tool",
    "system",
    "Measures execution time and delays",
    APP_PERMISSION_SYSTEM,
    APP_STATUS_STABLE,
    runTimerTool
  },
  {
    TOOL_RANDOM,
    "Random Generator",
    "system",
    "Generates hardware random values",
    APP_PERMISSION_SYSTEM,
    APP_STATUS_STABLE,
    runRandomTool
  },
  {
    TOOL_STORAGE_STATUS,
    "Storage Status",
    "system",
    "Shows internal and microSD storage policy",
    APP_PERMISSION_STORAGE,
    APP_STATUS_STABLE,
    runStorageStatus
  },
  {
  TOOL_IR_CONSOLE,
  "IR Console",
  "modules",
  "Reads infrared signals from an IR receiver",
  APP_PERMISSION_IR,
  APP_STATUS_EXPERIMENTAL,
  runIRConsole
  },
  {
    TOOL_RF_ANALYZER,
    "RF Analyzer",
    "modules",
    "Initializes CC1101 and samples RF RSSI",
    APP_PERMISSION_RF,
    APP_STATUS_EXPERIMENTAL,
    runRFAnalyzer
  },
  {
    TOOL_SUBGHZ_SCANNER,
    "SubGHz Scanner",
    "modules",
    "Performs a narrow Sub-GHz scan and logs discovered packets",
    APP_PERMISSION_RF,
    APP_STATUS_EXPERIMENTAL,
    runSubGHzScanner
  },
  {
    TOOL_RFID_READER,
    "RFID Reader",
    "modules",
    "Reads RFID card UID using RC522",
    APP_PERMISSION_RFID,
    APP_STATUS_EXPERIMENTAL,
    runRFIDReader
  },
  {
    TOOL_NFC_READER,
    "NFC Reader",
    "modules",
    "Reads NFC tag UID using PN532",
    APP_PERMISSION_NFC,
    APP_STATUS_EXPERIMENTAL,
    runNFCReader
  },
  {
    TOOL_CAN_MONITOR,
    "CAN Monitor",
    "modules",
    "Reads CAN frames using MCP2515",
    APP_PERMISSION_CAN,
    APP_STATUS_EXPERIMENTAL,
    runCANMonitor
  },
  {
    TOOL_GPS_STATUS,
    "GPS Status",
    "modules",
    "Reads GPS position and satellite status",
    APP_PERMISSION_GPS,
    APP_STATUS_EXPERIMENTAL,
    runGPSStatus
  },
  {
    TOOL_LORA_STATUS,
    "LoRa Status",
    "modules",
    "Initializes LoRa module and reads RSSI",
    APP_PERMISSION_LORA,
    APP_STATUS_EXPERIMENTAL,
    runLoRaStatus
  },
  {
    TOOL_CAPTURE_VIEWER,
    "Capture Viewer",
    "system",
    "Shows recent saved captures from microSD",
    APP_PERMISSION_STORAGE,
    APP_STATUS_STABLE,
    runCaptureViewer
  },
  {
    TOOL_SIGNAL_DATABASE,
    "Signal Database",
    "system",
    "Shows derived signal records stored on microSD",
    APP_PERMISSION_STORAGE,
    APP_STATUS_STABLE,
    runSignalDatabaseViewer
  },
  {
    TOOL_AI_ANALYZER,
    "AI Analyzer",
    "system",
    "Sends redacted field context to a configured AI gateway",
    APP_PERMISSION_WIFI,
    APP_STATUS_EXPERIMENTAL,
    runAIAnalyzer
  },
  {
    TOOL_AI_REPORT_VIEWER,
    "AI Report Viewer",
    "system",
    "Shows the latest AI companion report from microSD",
    APP_PERMISSION_STORAGE,
    APP_STATUS_STABLE,
    runAIReportViewer
  },
  {
    TOOL_MODULE_MANIFEST,
    "Module Manifest",
    "modules",
    "Shows supported external module types and interfaces",
    APP_PERMISSION_MODULES,
    APP_STATUS_STABLE,
    runModuleManifestViewer
  },
  {
    TOOL_REBOOT,
    "Reboot",
    "system",
    "Restarts the device",
    APP_PERMISSION_SYSTEM,
    APP_STATUS_STABLE,
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
  fileSystemInit();
  storagePolicyInit();
  if (fileSystemAvailable()) {
    loggerEnableFileLogging(true);
  }
  eventBusInit();
  statusInit();
  moduleManifestInit();
  moduleManagerInit();
#if BLACKCAPY_ENABLE_MODULE_MOCKS
  moduleManagerDetectMock();
#endif
  moduleManagerDetectByADC();
  inputInit();
  displayInit();
  screenManagerInit();

  appManagerInit(apps, appCount);

  menuInit();
  automationInit();
  healthMonitorInit();
  aiClientInit();

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
  displayUpdate();

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
