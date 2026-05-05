#include <Arduino.h>
#include "display_manager.h"
#include "storage.h"
#include "ui.h"

void runSettingsApp() {
  Serial.println();
  Serial.println("========== SETTINGS ==========");

  String profile = storageGetString("profile", "operator");
  bool animations = storageGetBool("animations", true);
  int brightness = storageGetInt("brightness", 80);
  const ThemeConfig* theme = displayGetThemeConfig();

  Serial.print("Profile: ");
  Serial.println(profile);

  Serial.print("Hybrid UI animations: ");
  Serial.println(animations ? "enabled" : "disabled");

  Serial.print("Display brightness: ");
  Serial.print(brightness);
  Serial.println("%");

  Serial.print("Display theme: ");
  Serial.println(theme->name);

  Serial.println();
  Serial.println("Applying default professional profile...");

  storageSetString("profile", "operator");
  storageSetBool("animations", true);
  storageSetInt("brightness", 80);

  uiShowStatusOK("Settings saved.");

  Serial.println("==============================");
}
