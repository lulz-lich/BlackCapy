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
void uiShowIconRF();
void uiShowIconSubGHz();
void uiShowIconSignalDatabase();
void uiShowIconAI();

// Hybrid UI functions (pixel art + ASCII fallback)
void uiShowIconWiFiHybrid();
void uiShowIconBLEHybrid();
void uiShowIconGPIOHybrid();
void uiShowIconRFHybrid();
void uiShowIconSubGHzHybrid();
void uiShowIconSignalDatabaseHybrid();
void uiShowIconAIHybrid();
void uiShowIconWiFiHybridAt(int x, int y);
void uiShowIconBLEHybridAt(int x, int y);
void uiShowIconGPIOHybridAt(int x, int y);
void uiShowIconRFHybridAt(int x, int y);
void uiShowIconSubGHzHybridAt(int x, int y);
void uiShowIconSignalDatabaseHybridAt(int x, int y);
void uiShowIconAIHybridAt(int x, int y);
void uiShowLogoHybrid();

// Progress and data visualization
void uiDrawProgressBar(int current, int total, int width = 40);
void uiDrawDataTable(const String* headers, int headerCount, const String** rows, int rowCount);

#endif
