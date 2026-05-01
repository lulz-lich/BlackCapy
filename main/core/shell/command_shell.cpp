#include "command_shell.h"
#include "tool_registry.h"
#include "status_manager.h"
#include "storage.h"
#include "logger.h"
#include "ui.h"

bool shellHandleCommand(const String& rawCommand) {
  String command = rawCommand;
  command.trim();

  if (command == "registry") {
    registryPrintAll();
    return true;
  }

  if (command == "status") {
    statusPrint();
    return true;
  }

  if (command == "profile") {
    String profile = storageGetString("profile", "operator");
    Serial.print("Current profile: ");
    Serial.println(profile);
    return true;
  }

  if (command == "heap") {
    Serial.print("Free heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
    return true;
  }

  if (command == "about") {
    uiShowSplash();
    return true;
  }

  if (command == "clear_storage") {
    storageClear();
    logWarn("Storage cleared by shell command.");
    return true;
  }

  return false;
}