#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <Arduino.h>

enum EventType {
  EVENT_BOOT,
  EVENT_TOOL_STARTED,
  EVENT_TOOL_FINISHED,
  EVENT_MODULE_DETECTED,
  EVENT_ERROR
};

void eventBusInit();
void eventEmit(EventType type, const String& message);
const char* eventTypeToString(EventType type);

#endif