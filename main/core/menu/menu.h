#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

void menuInit();
void menuPrintHeader();
void menuPrint();
void menuHandleInput(const String& input);

#endif