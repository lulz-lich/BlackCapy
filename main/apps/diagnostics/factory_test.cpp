#include <Arduino.h>
#include <WiFi.h>

#include "factory_test.h"

#include "ai_client.h"
#include "app_manager.h"
#include "blackcapy.h"
#include "display_manager.h"
#include "filesystem.h"
#include "hardware_config.h"
#include "module_manager.h"
#include "storage_policy.h"
#include "ui.h"

struct FactoryCheck {
  const char* label;
  bool passed;
  String detail;
};

static void printFactoryCheck(const FactoryCheck& check) {
  Serial.print(check.passed ? "[PASS] " : "[FAIL] ");
  Serial.print(check.label);
  if (check.detail.length() > 0) {
    Serial.print(": ");
    Serial.print(check.detail);
  }
  Serial.println();
}

static String reportFactoryCheck(const FactoryCheck& check) {
  String line = check.passed ? "PASS," : "FAIL,";
  line += check.label;
  line += ",";
  line += check.detail;
  line += "\n";
  return line;
}

void runFactoryTest() {
  Serial.println();
  Serial.println("========== FACTORY TEST ==========");

  displayClear();
  displayDrawTitle("Factory Test");
  displayDrawText(8, 32, "Running bench checks...");
  displayStartAnimationLoopFromFileScaled(248, 118, "animations/boot_pulse.anim", 120, 2);

  pinMode(BTN_ADC_PIN, INPUT);

  int moduleADC = moduleManagerReadADCStable();
  int buttonADC = analogRead(BTN_ADC_PIN);
  AIGatewayConfig aiConfig = aiClientLoadGatewayConfig();

  bool sdAvailable = fileSystemAvailable();
  String reportPath = storagePolicyGetReportsPath() + "/factory_test.txt";
  String report = "BlackCapy Factory Test\n";
  report += "version=";
  report += BLACKCAPY_VERSION;
  report += "\n";

  FactoryCheck checks[] = {
    {
      "Firmware version",
      String(BLACKCAPY_VERSION).length() > 0,
      BLACKCAPY_VERSION
    },
    {
      "AppManager registry",
      appManagerCount() >= 30,
      String(appManagerCount()) + " apps"
    },
    {
      "Heap reserve",
      ESP.getFreeHeap() > 50000,
      String(ESP.getFreeHeap()) + " bytes"
    },
    {
      "WiFi STA mode",
      WiFi.getMode() == WIFI_STA,
      String((int)WiFi.getMode())
    },
    {
      "microSD available",
      sdAvailable,
      sdAvailable ? "ready" : "not detected"
    },
    {
      "Logo asset",
      fileSystemExists(storagePolicyGetAssetsPath() + "/logo/blackcapy_logo.bmp"),
      "assets/logo/blackcapy_logo.bmp"
    },
    {
      "Icon asset",
      fileSystemExists(storagePolicyGetAssetsPath() + "/icons/wifi_icon.bmp"),
      "assets/icons/wifi_icon.bmp"
    },
    {
      "Animation asset",
      fileSystemExists(storagePolicyGetAssetsPath() + "/animations/boot_pulse.anim"),
      "assets/animations/boot_pulse.anim"
    },
    {
      "Module ADC range",
      moduleADC >= 0 && moduleADC <= 4095,
      String(moduleADC)
    },
    {
      "Button ADC range",
      buttonADC >= 0 && buttonADC <= 4095,
      String(buttonADC)
    },
    {
      "AI gateway config",
      aiConfig.timeoutMs > 0,
      aiConfig.enabled ? "enabled" : "disabled"
    }
  };

  const int checkCount = sizeof(checks) / sizeof(checks[0]);
  int passed = 0;

  for (int i = 0; i < checkCount; i++) {
    printFactoryCheck(checks[i]);
    report += reportFactoryCheck(checks[i]);

    if (checks[i].passed) {
      passed++;
    }

    displayDrawText(8, 54 + (i % 8) * 18, String(i + 1) + "/" + String(checkCount) + " " + checks[i].label);
    displayDelay(80);
  }

  bool reportWritten = false;
  if (sdAvailable) {
    report += "summary=";
    report += String(passed);
    report += "/";
    report += String(checkCount);
    report += "\n";
    reportWritten = fileSystemWrite(reportPath, report);
  }

  displayStopAnimation();
  displayDrawStatusBar(String("Factory ") + String(passed) + "/" + String(checkCount));

  Serial.print("Factory checks passed: ");
  Serial.print(passed);
  Serial.print("/");
  Serial.println(checkCount);
  Serial.print("Report write: ");
  Serial.println(reportWritten ? reportPath : "not written");

  if (passed == checkCount && (!sdAvailable || reportWritten)) {
    uiShowStatusOK("Factory test passed.");
  } else {
    uiShowStatusError("Factory test needs attention.");
  }

  Serial.println("==================================");
}
