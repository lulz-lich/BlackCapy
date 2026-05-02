#include "capture_writer.h"

#include "filesystem.h"
#include "logger.h"

String capturePathFor(const String& category) {
  return "/captures/" + category + ".log";
}

void captureWriteLine(const String& category, const String& line) {
  if (!fileSystemAvailable()) {
    logWarn("Capture skipped: microSD unavailable.");
    return;
  }

  String path = capturePathFor(category);

  String output = "";
  output += "[";
  output += String(millis());
  output += " ms] ";
  output += line;
  output += "\n";

  if (!fileSystemAppend(path, output)) {
    logError("Failed to write capture: " + path);
    return;
  }

  logInfo("Capture saved: " + path);
}