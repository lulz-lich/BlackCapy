#include "menu.h"

#include "logger.h"
#include "blackcapy.h"
#include "app_manager.h"

void menuInit() {
  logInfo("Menu initialized.");
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
  Serial.println("Available apps:");

  int total = appManagerCount();

  for (int i = 0; i < total; i++) {
    AppEntry* app = appManagerGet(i);

    if (app == nullptr) {
      continue;
    }

    appManagerPrintApp(app);
  }

  Serial.println();
  Serial.println("Commands:");
  Serial.println("help            - Show menu");
  Serial.println("clear           - Clear screen");
  Serial.println("registry        - Show app registry");
  Serial.println("status          - Show system status");
  Serial.println("heap            - Show heap memory");
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

  if (selectedId <= 0) {
    logWarn("Invalid command: " + command);
    menuPrint();
    return;
  }

  bool executed = appManagerRunById(selectedId);

  if (!executed) {
    logWarn("Invalid app id: " + String(selectedId));
  }

  menuPrint();
}