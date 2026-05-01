#include "event_bus.h"
#include "logger.h"

const char* eventTypeToString(EventType type) {
  switch (type) {
    case EVENT_BOOT: return "BOOT";
    case EVENT_TOOL_STARTED: return "TOOL_STARTED";
    case EVENT_TOOL_FINISHED: return "TOOL_FINISHED";
    case EVENT_MODULE_DETECTED: return "MODULE_DETECTED";
    case EVENT_ERROR: return "ERROR";
    default: return "UNKNOWN";
  }
}

void eventBusInit() {
  logInfo("Event bus initialized.");
}

void eventEmit(EventType type, const String& message) {
  Serial.print("[EVENT] ");
  Serial.print(eventTypeToString(type));
  Serial.print(" -> ");
  Serial.println(message);
}