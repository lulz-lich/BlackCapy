#ifndef TOOL_REGISTRY_H
#define TOOL_REGISTRY_H

#include <Arduino.h>

struct ToolMetadata {
  int id;
  const char* name;
  const char* category;
  const char* description;
  bool enabled;
};

void registryInit();
void registryPrintTool(const ToolMetadata& tool);
void registryPrintAll();

#endif