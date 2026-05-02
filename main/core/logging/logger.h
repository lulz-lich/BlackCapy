#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

enum LogLevel {
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_DEBUG
};

void loggerInit();

void loggerEnableFileLogging(bool enabled);
bool loggerFileLoggingEnabled();

void logMessage(LogLevel level, const String& message);

void logInfo(const String& message);
void logWarn(const String& message);
void logError(const String& message);
void logDebug(const String& message);

#endif