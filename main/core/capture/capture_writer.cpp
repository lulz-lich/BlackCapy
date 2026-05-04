#include "capture_writer.h"

#include "filesystem.h"
#include "logger.h"
#include "storage_policy.h"

static const char* supportedCaptureCategories[] = {
  "ir",
  "rf",
  "subghz",
  "rfid",
  "nfc",
  "can",
  "gps",
  "lora"
};

static const int supportedCaptureCategoryCount =
    sizeof(supportedCaptureCategories) / sizeof(supportedCaptureCategories[0]);

static String escapeCSVValue(String value) {
  bool needsQuotes = false;
  String escaped = "";

  for (int i = 0; i < value.length(); i++) {
    char c = value[i];

    if (c == '"' || c == ',' || c == '\n' || c == '\r') {
      needsQuotes = true;
    }

    if (c == '"') {
      escaped += "\"\"";
    } else {
      escaped += c;
    }
  }

  if (needsQuotes) {
    return "\"" + escaped + "\"";
  }

  return escaped;
}

bool captureCategoryIsSupported(const String& category) {
  for (int i = 0; i < supportedCaptureCategoryCount; i++) {
    if (category == supportedCaptureCategories[i]) {
      return true;
    }
  }

  return false;
}

int captureCategoryCount() {
  return supportedCaptureCategoryCount;
}

const char* captureCategoryAt(int index) {
  if (index < 0 || index >= supportedCaptureCategoryCount) {
    return "";
  }

  return supportedCaptureCategories[index];
}

String capturePathFor(const String& category) {
  return storagePolicyGetCapturesPath() + "/" + category + ".log";
}

void captureWriteLine(const String& category, const String& line) {
  if (!captureCategoryIsSupported(category)) {
    logWarn("Capture skipped: unsupported category " + category);
    return;
  }

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

bool captureExportToCSV(const String& category, const String& outputPath) {
  if (!captureCategoryIsSupported(category)) {
    logWarn("CSV export failed: unsupported category " + category);
    return false;
  }

  if (!fileSystemAvailable()) {
    logWarn("CSV export failed: microSD unavailable.");
    return false;
  }

  String logPath = capturePathFor(category);
  if (!fileSystemExists(logPath)) {
    logWarn("Capture file not found: " + logPath);
    return false;
  }

  String logData = fileSystemRead(logPath);
  if (logData.length() == 0) {
    logWarn("Capture file is empty: " + logPath);
    return false;
  }

  // Write CSV header
  String csvContent = "Timestamp_ms,Data\n";

  // Parse log format: [timestamp ms] data
  int lineStart = 0;
  int lineEnd;
  int lineCount = 0;

  while (lineStart < logData.length()) {
    lineEnd = logData.indexOf('\n', lineStart);
    if (lineEnd < 0) {
      lineEnd = logData.length();
    }

    String line = logData.substring(lineStart, lineEnd);
    line.trim();

    if (line.length() > 0) {
      // Parse [timestamp] and data
      int bracketEnd = line.indexOf(']');
      if (bracketEnd > 0 && line.charAt(0) == '[') {
        String timestampPart = line.substring(1, bracketEnd);
        int msIndex = timestampPart.indexOf(" ms");
        if (msIndex > 0) {
          String timestamp = timestampPart.substring(0, msIndex);
          String data = line.substring(bracketEnd + 2); // Skip "] "

          csvContent += timestamp + "," + escapeCSVValue(data) + "\n";
          lineCount++;
        }
      }
    }

    lineStart = lineEnd + 1;
  }

  // Write CSV file
  if (!fileSystemWrite(outputPath, csvContent)) {
    logError("Failed to write CSV export: " + outputPath);
    return false;
  }

  logInfo("CSV export complete: " + outputPath + " (" + String(lineCount) + " rows)");
  return true;
}

void capturePrintStats(const String& category) {
  if (!captureCategoryIsSupported(category)) {
    Serial.println("[STATS] Unsupported capture category: " + category);
    return;
  }

  if (!fileSystemAvailable()) {
    Serial.println("[STATS] MicroSD unavailable.");
    return;
  }

  String logPath = capturePathFor(category);
  if (!fileSystemExists(logPath)) {
    Serial.println("[STATS] Capture file not found: " + logPath);
    return;
  }

  String logData = fileSystemRead(logPath);
  if (logData.length() == 0) {
    Serial.println("[STATS] Capture file is empty.");
    return;
  }

  int lineCount = 0;
  int minTimestamp = -1;
  int maxTimestamp = 0;

  // Parse log data
  int lineStart = 0;
  int lineEnd;

  while (lineStart < logData.length()) {
    lineEnd = logData.indexOf('\n', lineStart);
    if (lineEnd < 0) {
      lineEnd = logData.length();
    }

    String line = logData.substring(lineStart, lineEnd);
    if (line.length() > 0) {
      lineCount++;

      // Extract timestamp
      int bracketEnd = line.indexOf(']');
      if (bracketEnd > 0 && line.charAt(0) == '[') {
        String timestampPart = line.substring(1, bracketEnd);
        int msIndex = timestampPart.indexOf(" ms");
        if (msIndex > 0) {
          String timestamp = timestampPart.substring(0, msIndex);
          int ts = timestamp.toInt();
          if (minTimestamp < 0 || ts < minTimestamp) {
            minTimestamp = ts;
          }
          if (ts > maxTimestamp) {
            maxTimestamp = ts;
          }
        }
      }
    }

    lineStart = lineEnd + 1;
  }

  Serial.println("\n=== Capture Statistics ===");
  Serial.print("Category: ");
  Serial.println(category);
  Serial.print("Total entries: ");
  Serial.println(lineCount);
  if (minTimestamp >= 0) {
    Serial.print("Duration: ");
    Serial.print(maxTimestamp - minTimestamp);
    Serial.println(" ms");
  }
  Serial.println();
}
