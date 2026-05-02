#include "logger.h"
#include "filesystem.h"

static bool fileLoggingEnabled = false;

static const char* levelToString(LogLevel level) {
  switch (level) {
    case LOG_INFO: return "INFO";
    case LOG_WARN: return "WARN";
    case LOG_ERROR: return "ERROR";
    case LOG_DEBUG: return "DEBUG";
    default: return "UNKNOWN";
  }
}

void loggerInit() {
  logInfo("Logger initialized.");
}

void loggerEnableFileLogging(bool enabled) {
  fileLoggingEnabled = enabled;

  if (enabled) {
    logInfo("File logging enabled.");
  } else {
    logWarn("File logging disabled.");
  }
}

bool loggerFileLoggingEnabled() {
  return fileLoggingEnabled;
}

void logMessage(LogLevel level, const String& message) {
  String line = "";

  line += "[";
  line += String(millis());
  line += " ms] ";

  line += "[";
  line += levelToString(level);
  line += "] ";

  line += message;

  Serial.println(line);

  if (fileLoggingEnabled && fileSystemAvailable()) {
    fileSystemAppend("/logs/system.log", line + "\n");
  }
}

void logInfo(const String& message) {
  logMessage(LOG_INFO, message);
}

void logWarn(const String& message) {
  logMessage(LOG_WARN, message);
}

void logError(const String& message) {
  logMessage(LOG_ERROR, message);
}

void logDebug(const String& message) {
  logMessage(LOG_DEBUG, message);
}