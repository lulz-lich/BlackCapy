#ifndef STATUS_MANAGER_H
#define STATUS_MANAGER_H

#include <Arduino.h>

enum SystemStatus {
  STATUS_IDLE,
  STATUS_RUNNING_TOOL,
  STATUS_ERROR
};

void statusInit();
void statusSet(SystemStatus status);
SystemStatus statusGet();
const char* statusToString(SystemStatus status);
void statusPrint();

#endif