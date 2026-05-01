#include "navigation.h"
#include "input_manager.h"
#include "ui.h"

static int selectedIndex = 0;
static const int maxItems = 5;

void navigationInit() {
  selectedIndex = 0;
}

void drawMenu() {
  uiShowSeparator();

  for (int i = 0; i < maxItems; i++) {
    if (i == selectedIndex) {
      Serial.print("> ");
    } else {
      Serial.print("  ");
    }

    Serial.print("Item ");
    Serial.println(i + 1);
  }

  uiShowSeparator();
}

void navigationUpdate() {
  InputEvent ev = inputRead();

  if (ev == INPUT_NONE) return;

  switch (ev) {
    case INPUT_UP:
      if (selectedIndex > 0) selectedIndex--;
      break;

    case INPUT_DOWN:
      if (selectedIndex < maxItems - 1) selectedIndex++;
      break;

    case INPUT_OK:
      Serial.print("Selected: ");
      Serial.println(selectedIndex + 1);
      break;

    case INPUT_BACK:
      Serial.println("Back pressed");
      break;

    default:
      break;
  }

  drawMenu();
}