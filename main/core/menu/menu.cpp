#include "menu.h"
#include "logger.h"
#include "blackcapy.h"

static ToolEntry* registeredTools = nullptr;
static int registeredToolCount = 0;

void menuInit() {
  logInfo("Menu initialized.");
}

void menuRegisterTools(ToolEntry* tools, int count) {
  registeredTools = tools;
  registeredToolCount = count;

  logInfo("Tools registered: " + String(count));
}

void menuPrintHeader() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("              BLACKCAPY OS");
  Serial.println("         Professional Toolkit");
  Serial.println("========================================");
  Serial.print("Version: ");
  Serial.println(BLACKCAPY_VERSION);
  Serial.print("Author: ");
  Serial.println(BLACKCAPY_AUTHOR);
  Serial.println("========================================");
  Serial.println();
}

void menuPrint() {
  Serial.println();
  Serial.println("Available tools:");

  for (int i = 0; i < registeredToolCount; i++) {
    Serial.print("[");
    Serial.print(registeredTools[i].id);
    Serial.print("] ");
    Serial.print(registeredTools[i].name);
    Serial.print(" - ");
    Serial.println(registeredTools[i].description);
  }

  Serial.println();
  Serial.println("Commands:");
  Serial.println("help  - Show menu");
  Serial.println("clear - Clear screen");
  Serial.println();
  Serial.print("blackcapy> ");
}

void menuHandleInput(const String& input) {
  String command = input;
  command.trim();

  if (command.length() == 0) {
    menuPrint();
    return;
  }

  if (command == "help") {
    menuPrint();
    return;
  }

  if (command == "clear") {
    Serial.write(27);
    Serial.print("[2J");
    Serial.write(27);
    Serial.print("[H");
    menuPrintHeader();
    menuPrint();
    return;
  }

  int selectedId = command.toInt();

  for (int i = 0; i < registeredToolCount; i++) {
    if (registeredTools[i].id == selectedId) {
      logInfo(String("Running tool: ") + registeredTools[i].name);
      registeredTools[i].callback();
      menuPrint();
      return;
    }
  }

  logWarn("Invalid command: " + command);
  menuPrint();
}
