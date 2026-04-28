#include <Arduino.h>
#include "logger.h"

struct AutomationRule {
  const char* name;
  const char* condition;
  const char* action;
  bool enabled;
};

static AutomationRule rules[] = {
  {
    "Boot Log",
    "System boot completed",
    "Write boot status to log",
    true
  },
  {
    "Weak WiFi Signal Flag",
    "RSSI below -80 dBm",
    "Flag as weak network",
    true
  },
  {
    "Module Detection",
    "Expansion module detected",
    "Load module profile",
    true
  }
};

static const int ruleCount = sizeof(rules) / sizeof(rules[0]);

void automationInit() {
  logInfo("Automation engine initialized.");
}

void runAutomationEngine() {
  Serial.println();
  Serial.println("========== AUTOMATION ENGINE ==========");

  Serial.print("Rules loaded: ");
  Serial.println(ruleCount);

  for (int i = 0; i < ruleCount; i++) {
    Serial.print("[");
    Serial.print(i + 1);
    Serial.print("] ");

    Serial.print(rules[i].name);
    Serial.print(" | Condition: ");
    Serial.print(rules[i].condition);
    Serial.print(" | Action: ");
    Serial.print(rules[i].action);
    Serial.print(" | Status: ");
    Serial.println(rules[i].enabled ? "ENABLED" : "DISABLED");
  }

  Serial.println("Automation runtime: ready");
  Serial.println("=======================================");
}
