#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

struct ToolEntry {
  int id;
  const char* name;
  const char* description;
  void (*callback)();
};

void menuInit();
void menuRegisterTools(ToolEntry* tools, int count);
void menuPrintHeader();
void menuPrint();
void menuHandleInput(const String& input);

#endif
