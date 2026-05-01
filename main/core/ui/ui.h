#ifndef UI_H
#define UI_H

#include <Arduino.h>

void uiInit();

void uiPrint(const String& text);
void uiPrintLine(const String& text);

void uiShowSplash();
void uiShowSeparator();
void uiShowLoading(const String& label);
void uiShowStatusOK(const String& message);
void uiShowStatusError(const String& message);

void uiShowIconWiFi();
void uiShowIconBLE();
void uiShowIconGPIO();
void uiShowIconSystem();
void uiShowIconLogs();

#endif
