#include "logger.h"

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

void logMessage(LogLevel level, const String& message) {
  Serial.print("[");
  Serial.print(millis());
  Serial.print(" ms] ");
  Serial.print("[");
  Serial.print(levelToString(level));
  Serial.print("] ");
  Serial.println(message);
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
