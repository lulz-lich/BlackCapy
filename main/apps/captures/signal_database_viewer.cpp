#include <Arduino.h>

#include "filesystem.h"
#include "logger.h"
#include "storage_policy.h"
#include "ui.h"

static String signalDatabasePath() {
  return storagePolicyGetSignalsPath() + "/subghz_signals.csv";
}

static int countSignalRecords(const String& data) {
  int records = 0;
  int lineStart = 0;
  bool firstLine = true;

  while (lineStart < data.length()) {
    int lineEnd = data.indexOf('\n', lineStart);
    if (lineEnd < 0) {
      lineEnd = data.length();
    }

    String line = data.substring(lineStart, lineEnd);
    line.trim();

    if (line.length() > 0) {
      if (firstLine) {
        firstLine = false;
      } else {
        records++;
      }
    }

    lineStart = lineEnd + 1;
  }

  return records;
}

static String lastSignalRecords(const String& data, int maxRecords) {
  int recordCount = countSignalRecords(data);
  int startRecord = recordCount - maxRecords;
  if (startRecord < 0) {
    startRecord = 0;
  }

  String output;
  int currentRecord = -1;
  int lineStart = 0;

  while (lineStart < data.length()) {
    int lineEnd = data.indexOf('\n', lineStart);
    if (lineEnd < 0) {
      lineEnd = data.length();
    }

    String line = data.substring(lineStart, lineEnd);
    line.trim();

    if (line.length() > 0) {
      if (currentRecord >= startRecord) {
        output += line;
        output += "\n";
      }
      currentRecord++;
    }

    lineStart = lineEnd + 1;
  }

  return output;
}

void runSignalDatabaseViewer() {
  Serial.println();
  Serial.println("========== SIGNAL DATABASE ==========");

  if (!fileSystemAvailable()) {
    uiShowStatusError("microSD unavailable.");
    logWarn("Signal database viewer failed: microSD unavailable.");
    return;
  }

  String path = signalDatabasePath();
  if (!fileSystemExists(path)) {
    Serial.println("No signal database found.");
    Serial.println("Run 'subghz db' after collecting Sub-GHz captures.");
    Serial.println("=====================================");
    return;
  }

  String data = fileSystemRead(path);
  if (data.length() == 0) {
    Serial.println("Signal database is empty.");
    Serial.println("=====================================");
    return;
  }

  int records = countSignalRecords(data);
  Serial.print("Path: ");
  Serial.println(path);
  Serial.print("Records: ");
  Serial.println(records);
  Serial.println();
  Serial.println("Recent records:");
  Serial.println(lastSignalRecords(data, 8));
  Serial.println("=====================================");
}
