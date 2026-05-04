#include "filesystem.h"
#include "storage_policy.h"

#include <SPI.h>
#include <SD.h>

#include "logger.h"
#include "hardware_config.h"

static bool initialized = false;

bool fileSystemInit() {
  if (!SD.begin(SD_CS_PIN)) {
    logWarn("MicroSD not detected.");
    initialized = false;
    return false;
  }

  initialized = true;

  logInfo("MicroSD initialized.");

  fileSystemCreateDirectory(storagePolicyGetLogsPath());
  fileSystemCreateDirectory(storagePolicyGetAssetsPath());
  fileSystemCreateDirectory(storagePolicyGetScriptsPath());
  fileSystemCreateDirectory(storagePolicyGetCapturesPath());
  fileSystemCreateDirectory(storagePolicyGetModulesPath());
  fileSystemCreateDirectory(storagePolicyGetThemesPath());
  fileSystemCreateDirectory(storagePolicyGetPluginsPath());

  return true;
}

bool fileSystemAvailable() {
  return initialized;
}

bool fileSystemCreateDirectory(
    const String& path
) {
  if (!initialized) {
    return false;
  }

  if (SD.exists(path)) {
    return true;
  }

  return SD.mkdir(path);
}

bool fileSystemWrite(
    const String& file,
    const String& content
) {
  if (!initialized) {
    return false;
  }

  File f = SD.open(file, FILE_WRITE);

  if (!f) {
    return false;
  }

  f.print(content);

  f.close();

  return true;
}

bool fileSystemAppend(
    const String& file,
    const String& content
) {
  if (!initialized) {
    return false;
  }

  File f = SD.open(file, FILE_APPEND);

  if (!f) {
    return false;
  }

  f.print(content);

  f.close();

  return true;
}

String fileSystemRead(
    const String& file
) {
  if (!initialized) {
    return "";
  }

  File f = SD.open(file);

  if (!f) {
    return "";
  }

  String data = "";

  while (f.available()) {
    data += (char) f.read();
  }

  f.close();

  return data;
}

bool fileSystemExists(const String& path) {
  if (!initialized) {
    return false;
  }

  return SD.exists(path);
}

String fileSystemListDirectory(const String& path) {
  if (!initialized) {
    return "";
  }

  File dir = SD.open(path);
  if (!dir || !dir.isDirectory()) {
    if (dir) {
      dir.close();
    }
    return "";
  }

  String output;
  File entry = dir.openNextFile();

  while (entry) {
    output += String(entry.name());
    output += "\n";
    entry.close();
    entry = dir.openNextFile();
  }

  dir.close();
  return output;
}
