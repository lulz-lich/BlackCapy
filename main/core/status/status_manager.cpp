#include "status_manager.h"
#include "logger.h"

static SystemStatus currentStatus = STATUS_IDLE;

void statusInit() {
  currentStatus = STATUS_IDLE;
  logInfo("Status manager initialized.");
}

void statusSet(SystemStatus status) {
  currentStatus = status;
}

SystemStatus statusGet() {
  return currentStatus;
}

const char* statusToString(SystemStatus status) {
  switch (status) {
    case STATUS_IDLE: return "IDLE";
    case STATUS_RUNNING_TOOL: return "RUNNING_TOOL";
    case STATUS_ERROR: return "ERROR";
    default: return "UNKNOWN";
  }
}

void statusPrint() {
  Serial.print("System status: ");
  Serial.println(statusToString(currentStatus));
}