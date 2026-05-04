#include <Arduino.h>
#include "core/display/display_manager.h"
#include "core/logging/logger.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  loggerInit();
  displayInit();

  Serial.println("\n=== BlackCapy Theme Demonstration ===\n");

  // Test each theme
  for (int i = 0; i < 5; i++) {
    displaySetTheme((DisplayTheme)i);
    const ThemeConfig* theme = displayGetThemeConfig();

    Serial.print("Theme: ");
    Serial.println(theme->name);

    displayDrawTitle("BlackCapy v1.0");
    displayDrawAsciiFrame(0, 0, 30, 5, "Test Frame");

    Serial.println();
    delay(500);
  }

  Serial.println("Theme demonstration complete!");
}

void loop() {
  // Nothing to do
}
