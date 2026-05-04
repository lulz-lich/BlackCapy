#include <Arduino.h>
#include "filesystem.h"
#include "logger.h"
#include "storage_policy.h"

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

// Forward declarations
static bool automationExecuteCommand(String command, int lineNumber);
static bool automationExecuteGpioCommand(const String& args);
static bool automationExecuteUartCommand(const String& args);
static void automationPrintScriptError(int lineNumber, const String& message);

// Simple script execution engine
bool automationExecuteScript(const char* scriptName) {
  String scriptsPath = storagePolicyGetScriptsPath();
  String scriptPath = scriptsPath + "/" + String(scriptName);

  if (!fileSystemExists(scriptPath)) {
    Serial.println("Script not found: " + scriptPath);
    return false;
  }

  String scriptData = fileSystemRead(scriptPath);
  if (scriptData.length() == 0) {
    Serial.println("Script is empty or unreadable: " + scriptPath);
    return false;
  }

  Serial.println("Executing script: " + String(scriptName));
  Serial.println("=======================================");

  int lineNumber = 0;
  int lineStart = 0;

  while (lineStart < scriptData.length()) {
    int lineEnd = scriptData.indexOf('\n', lineStart);

    if (lineEnd < 0) {
      lineEnd = scriptData.length();
    }

    String line = scriptData.substring(lineStart, lineEnd);
    line.trim();
    lineNumber++;

    if (line.length() > 0) {
      if (!automationExecuteCommand(line, lineNumber)) {
        return false;
      }
    }

    lineStart = lineEnd + 1;
  }

  Serial.println("=======================================");
  Serial.println("Script execution completed.");
  return true;
}

static bool automationExecuteCommand(String command, int lineNumber) {
  command.trim();
  if (command.length() == 0 || command.startsWith("#")) {
    return true; // Skip empty lines and comments
  }

  Serial.print("[");
  Serial.print(lineNumber);
  Serial.print("] ");
  Serial.println(command);

  // Parse command
  int spaceIndex = command.indexOf(' ');
  String cmd;
  String args;

  if (spaceIndex >= 0) {
    cmd = command.substring(0, spaceIndex);
    args = command.substring(spaceIndex + 1);
  } else {
    cmd = command;
  }

  cmd.toLowerCase();

  if (cmd == "delay") {
    int ms = args.toInt();
    if (ms > 0) {
      delay(ms);
      return true;
    }
    automationPrintScriptError(lineNumber, "Invalid delay value.");
    return false;
  } else if (cmd == "print") {
    Serial.println(args);
    return true;
  } else if (cmd == "log") {
    logInfo(args.c_str());
    return true;
  } else if (cmd == "gpio") {
    return automationExecuteGpioCommand(args);
  } else if (cmd == "uart") {
    return automationExecuteUartCommand(args);
  }

  logWarn("Unknown automation command on line " + String(lineNumber) + ": " + cmd);
  Serial.println("Unknown command ignored: " + cmd);
  return true;
}

static void automationPrintScriptError(int lineNumber, const String& message) {
  String output = "Script error on line " + String(lineNumber) + ": " + message;
  Serial.println(output);
  logWarn(output);
}

static bool automationExecuteGpioCommand(const String& args) {
  // Parse: set <pin> <high|low> or read <pin>
  int space1 = args.indexOf(' ');
  if (space1 < 0) return false;

  String subcmd = args.substring(0, space1);
  String rest = args.substring(space1 + 1);

  subcmd.toLowerCase();

  if (subcmd == "set") {
    int space2 = rest.indexOf(' ');
    if (space2 < 0) return false;

    int pin = rest.substring(0, space2).toInt();
    String state = rest.substring(space2 + 1);
    state.toLowerCase();

    pinMode(pin, OUTPUT);
    if (state == "high" || state == "1") {
      digitalWrite(pin, HIGH);
    } else if (state == "low" || state == "0") {
      digitalWrite(pin, LOW);
    } else {
      return false;
    }
    return true;
  } else if (subcmd == "read") {
    int pin = rest.toInt();
    pinMode(pin, INPUT);
    int value = digitalRead(pin);
    Serial.print("GPIO ");
    Serial.print(pin);
    Serial.print(" = ");
    Serial.println(value);
    return true;
  }

  return false;
}

static bool automationExecuteUartCommand(const String& args) {
  // Parse: write <port> <data>
  int space1 = args.indexOf(' ');
  if (space1 < 0) return false;

  String subcmd = args.substring(0, space1);
  String rest = args.substring(space1 + 1);

  subcmd.toLowerCase();

  if (subcmd == "write") {
    int space2 = rest.indexOf(' ');
    if (space2 < 0) return false;

    int port = rest.substring(0, space2).toInt();
    String data = rest.substring(space2 + 1);

    // For now, just use Serial (port 0)
    if (port == 0) {
      Serial.println(data);
      return true;
    }
  }

  return false;
}

static void automationListScripts() {
  Serial.println("\nAvailable automation scripts:");

  if (!fileSystemAvailable()) {
    Serial.println("  microSD not available; scripts cannot be loaded.");
    return;
  }

  String scriptsPath = storagePolicyGetScriptsPath();
  if (!fileSystemExists(scriptsPath)) {
    Serial.println("  Scripts folder not found: " + scriptsPath);
    return;
  }

  String scripts = fileSystemListDirectory(scriptsPath);
  if (scripts.length() == 0) {
    Serial.println("  No script files found in " + scriptsPath);
    return;
  }

  int start = 0;
  int end = scripts.indexOf('\n', start);

  while (end >= 0) {
    String filename = scripts.substring(start, end);
    if (filename.length() > 0) {
      Serial.println("  - " + filename);
    }
    start = end + 1;
    end = scripts.indexOf('\n', start);
  }
}

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

  automationListScripts();

  Serial.println();
  Serial.println("Script execution commands:");
  Serial.println("  automationExecuteScript(\"script_name.txt\")");
  Serial.println("  Example: automationExecuteScript(\"blink_led.txt\")");

  Serial.println("Automation runtime: ready");
  Serial.println("=======================================");
}
