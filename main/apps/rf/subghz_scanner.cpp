#include <Arduino.h>
#include <RadioLib.h>

#include "hardware_config.h"
#include "logger.h"
#include "module_manager.h"
#include "capture_writer.h"
#include "filesystem.h"
#include "storage_policy.h"
#include "ui.h"
#include "display_manager.h"

static CC1101 subghz = new Module(
  RF_CS_PIN,
  RF_GDO0_PIN,
  RF_RST_PIN,
  RF_GDO2_PIN
);

enum SubGHzLogFormat {
  LOG_FORMAT_HEX,
  LOG_FORMAT_BASE64,
  LOG_FORMAT_BOTH
};

struct SubGHzCaptureStats {
  float minFreq;
  float maxFreq;
  float strongestFreq;
  int packetCount;
  int timeoutCount;
  int errorCount;
  int totalSignalStrength;
  int strongestRSSI;
};

static String subGHzCapturePath() {
  return storagePolicyGetCapturesPath() + "/subghz.log";
}

static String subGHzDatabasePath() {
  return storagePolicyGetSignalsPath() + "/subghz_signals.csv";
}

static void resetSubGHzCaptureStats(SubGHzCaptureStats& stats) {
  stats.minFreq = 999.0f;
  stats.maxFreq = 0.0f;
  stats.strongestFreq = 0.0f;
  stats.packetCount = 0;
  stats.timeoutCount = 0;
  stats.errorCount = 0;
  stats.totalSignalStrength = 0;
  stats.strongestRSSI = -200;
}

static String escapeCSV(String value) {
  bool needsQuotes = false;
  String escaped;

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

static String bytesToHex(const uint8_t* data, size_t length) {
  static const char hexMap[] = "0123456789ABCDEF";
  String result;
  result.reserve(length * 2);

  for (size_t i = 0; i < length; i++) {
    uint8_t value = data[i];
    result += hexMap[(value >> 4) & 0x0F];
    result += hexMap[value & 0x0F];
  }

  return result;
}

static String bytesToBase64(const uint8_t* data, size_t length) {
  static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String result;
  result.reserve(((length + 2) / 3) * 4);

  for (size_t i = 0; i < length; i += 3) {
    uint32_t value = 0;
    int chunkSize = 0;

    for (int j = 0; j < 3; j++) {
      value <<= 8;
      if (i + j < length) {
        value |= data[i + j];
        chunkSize++;
      }
    }

    result += table[(value >> 18) & 0x3F];
    result += table[(value >> 12) & 0x3F];
    result += (chunkSize > 1) ? table[(value >> 6) & 0x3F] : '=';
    result += (chunkSize > 2) ? table[value & 0x3F] : '=';
  }

  return result;
}

static String getConfigValue(const String& config, const String& key) {
  int position = 0;
  while (position < config.length()) {
    int endOfLine = config.indexOf('\n', position);
    if (endOfLine < 0) {
      endOfLine = config.length();
    }

    String line = config.substring(position, endOfLine);
    line.trim();
    if (line.startsWith(key + "=")) {
      int equalsIndex = line.indexOf('=');
      if (equalsIndex >= 0) {
        String value = line.substring(equalsIndex + 1);
        value.trim();
        return value;
      }
    }

    position = endOfLine + 1;
  }

  return String();
}

static String getLineValue(const String& line, const String& key) {
  int start = line.indexOf(key + "=");
  if (start < 0) {
    return String();
  }

  start += key.length() + 1;
  int end = line.indexOf(' ', start);
  if (end < 0) {
    end = line.length();
  }

  String value = line.substring(start, end);
  value.trim();
  return value;
}

static uint32_t getCaptureTimestamp(const String& line) {
  if (!line.startsWith("[")) {
    return 0;
  }

  int end = line.indexOf(" ms]");
  if (end < 0) {
    return 0;
  }

  String timestamp = line.substring(1, end);
  return (uint32_t) timestamp.toInt();
}

static bool parseSubGHzCaptureLine(
    const String& line,
    float& frequency,
    int& rssi,
    String& hexPayload,
    String& base64Payload
) {
  String freqValue = getLineValue(line, "FREQ");
  if (freqValue.length() == 0) {
    return false;
  }

  frequency = freqValue.toFloat();
  if (frequency <= 0.0f) {
    return false;
  }

  String rssiValue = getLineValue(line, "RSSI");
  if (rssiValue.length() == 0) {
    return false;
  }

  rssi = rssiValue.toInt();
  hexPayload = getLineValue(line, "HEX");
  base64Payload = getLineValue(line, "B64");
  return hexPayload.length() > 0 || base64Payload.length() > 0;
}

static int payloadLengthFrom(const String& hexPayload, const String& base64Payload) {
  if (hexPayload.length() > 0) {
    return hexPayload.length() / 2;
  }

  int padding = 0;
  if (base64Payload.endsWith("==")) {
    padding = 2;
  } else if (base64Payload.endsWith("=")) {
    padding = 1;
  }

  return ((base64Payload.length() * 3) / 4) - padding;
}

static void updateSubGHzStatsFromLine(const String& line, SubGHzCaptureStats& stats) {
  if (line.indexOf("TIMEOUT") >= 0) {
    stats.timeoutCount++;
    return;
  }

  if (line.indexOf("ERR_") >= 0) {
    stats.errorCount++;
    return;
  }

  float frequency = 0.0f;
  int rssi = 0;
  String hexPayload;
  String base64Payload;

  if (!parseSubGHzCaptureLine(line, frequency, rssi, hexPayload, base64Payload)) {
    return;
  }

  if (frequency < stats.minFreq) {
    stats.minFreq = frequency;
  }
  if (frequency > stats.maxFreq) {
    stats.maxFreq = frequency;
  }

  stats.totalSignalStrength += rssi;
  stats.packetCount++;

  if (rssi > stats.strongestRSSI) {
    stats.strongestRSSI = rssi;
    stats.strongestFreq = frequency;
  }
}

static SubGHzCaptureStats summarizeSubGHzCapture(const String& logData) {
  SubGHzCaptureStats stats;
  resetSubGHzCaptureStats(stats);

  int lineStart = 0;
  while (lineStart < logData.length()) {
    int lineEnd = logData.indexOf('\n', lineStart);
    if (lineEnd < 0) {
      lineEnd = logData.length();
    }

    String line = logData.substring(lineStart, lineEnd);
    line.trim();
    if (line.length() > 0) {
      updateSubGHzStatsFromLine(line, stats);
    }

    lineStart = lineEnd + 1;
  }

  return stats;
}

static void loadSubGHzScanConfig(
    float& startMHz,
    float& endMHz,
    float& stepMHz,
    uint32_t& timeoutMs,
    SubGHzLogFormat& logFormat
) {
  String cfgPath = storagePolicyGetScriptsPath() + "/subghz_scan.cfg";
  if (!fileSystemAvailable() || !fileSystemExists(cfgPath)) {
    return;
  }

  String config = fileSystemRead(cfgPath);
  if (config.length() == 0) {
    return;
  }

  String value;

  value = getConfigValue(config, "start_mhz");
  if (value.length() > 0) {
    startMHz = value.toFloat();
  }

  value = getConfigValue(config, "end_mhz");
  if (value.length() > 0) {
    endMHz = value.toFloat();
  }

  value = getConfigValue(config, "step_mhz");
  if (value.length() > 0) {
    stepMHz = value.toFloat();
  }

  value = getConfigValue(config, "timeout_ms");
  if (value.length() > 0) {
    timeoutMs = (uint32_t) value.toInt();
  }

  value = getConfigValue(config, "log_format");
  if (value.equalsIgnoreCase("hex")) {
    logFormat = LOG_FORMAT_HEX;
  } else if (value.equalsIgnoreCase("base64")) {
    logFormat = LOG_FORMAT_BASE64;
  } else if (value.equalsIgnoreCase("both")) {
    logFormat = LOG_FORMAT_BOTH;
  }
}

static void logSubGHzPacket(
    const uint8_t* data,
    size_t length,
    float frequency,
    SubGHzLogFormat format
) {
  String logData = "FREQ=" + String(frequency, 2);
  logData += " RSSI=" + String(subghz.getRSSI());

  if (format == LOG_FORMAT_HEX || format == LOG_FORMAT_BOTH) {
    logData += " HEX=" + bytesToHex(data, length);
  }

  if (format == LOG_FORMAT_BASE64 || format == LOG_FORMAT_BOTH) {
    logData += " B64=" + bytesToBase64(data, length);
  }

  captureWriteLine("subghz", logData);
}

static void subghzScanFrequency(
    float frequency,
    int scanIndex,
    int totalScans,
    uint32_t timeoutMs,
    SubGHzLogFormat logFormat
) {
  Serial.print("[SubGHz] [");
  Serial.print(scanIndex);
  Serial.print("/");
  Serial.print(totalScans);
  Serial.print("] Tuning to ");
  Serial.print(frequency, 2);
  Serial.println(" MHz...");

  displayDrawStatusBar("Freq: " + String(frequency, 2) + " MHz");

  int16_t state = subghz.setFrequency(frequency);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("[SubGHz] Frequency set failed, code: ");
    Serial.println(state);
    captureWriteLine("subghz", "FREQ=" + String(frequency, 2) + " ERR_SET_FREQ=" + String(state));
    return;
  }

  uint8_t packet[128];
  int16_t packetLen = subghz.receive(packet, sizeof(packet), timeoutMs);

  if (packetLen > 0) {
    Serial.println("[SubGHz] Packet received!");
    Serial.print("[SubGHz] Length: ");
    Serial.println(packetLen);
    Serial.print("[SubGHz] RSSI: ");
    Serial.print(subghz.getRSSI());
    Serial.println(" dBm");
    logSubGHzPacket(packet, packetLen, frequency, logFormat);
  } else if (packetLen == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println("[SubGHz] No packet detected.");
    captureWriteLine("subghz", "FREQ=" + String(frequency, 2) + " TIMEOUT");
  } else {
    Serial.print("[SubGHz] Receive failed, code: ");
    Serial.println(packetLen);
    captureWriteLine("subghz", "FREQ=" + String(frequency, 2) + " ERR_RX=" + String(packetLen));
  }
}

void runSubGHzScanner() {
  Serial.println();
  Serial.println("========== SubGHz Scanner ==========");

  if (!moduleManagerHas(MODULE_RF)) {
    Serial.println("RF module not detected. Sub-GHz scan unavailable.");
    logWarn("SubGHz scanner blocked: RF module not detected.");
    return;
  }

  float startFreq = RF_FREQUENCY - 5.0f;
  float endFreq = RF_FREQUENCY + 5.0f;
  float stepHz = 0.5f;
  uint32_t timeoutMs = 800;
  SubGHzLogFormat logFormat = LOG_FORMAT_BOTH;

  loadSubGHzScanConfig(startFreq, endFreq, stepHz, timeoutMs, logFormat);

  if (startFreq < 300.0f) {
    startFreq = 300.0f;
  }
  if (endFreq > 928.0f) {
    endFreq = 928.0f;
  }
  if (stepHz < 0.1f) {
    stepHz = 0.1f;
  }

  int totalScans = (int)(((endFreq - startFreq) / stepHz) + 1.5f);
  if (totalScans < 1) {
    totalScans = 1;
  }

  int state = subghz.begin(RF_FREQUENCY);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("CC1101 init failed. Code: ");
    Serial.println(state);
    logError("SubGHz scanner initialization failed.");
    return;
  }

  uiShowIconSubGHz();
  displayDrawAsciiFrame(0, 0, 42, 7, "SubGHz Scanner");
  displayDrawText(0, 1, "Mode: " + String((logFormat == LOG_FORMAT_HEX) ? "HEX" : (logFormat == LOG_FORMAT_BASE64) ? "BASE64" : "BOTH"));
  displayDrawText(0, 2, "Range: " + String(startFreq, 2) + "-" + String(endFreq, 2) + " MHz");
  displayDrawStatusBar("Starting scan...");

  Serial.print("SubGHz scan from ");
  Serial.print(startFreq, 2);
  Serial.print(" to ");
  Serial.print(endFreq, 2);
  Serial.print(" MHz with step ");
  Serial.print(stepHz, 2);
  Serial.print(" MHz and timeout ");
  Serial.print(timeoutMs);
  Serial.println(" ms.");

  for (int i = 0; i < totalScans; i++) {
    float frequency = startFreq + i * stepHz;
    if (frequency > endFreq + 0.001f) {
      break;
    }
    subghzScanFrequency(frequency, i + 1, totalScans, timeoutMs, logFormat);
    displayDrawStatusBar("Progress " + String(i + 1) + "/" + String(totalScans));
    delay(150);
  }

  Serial.println();
  Serial.println("SubGHz scan complete.");
  Serial.println("Captured results stored to microSD if available.");
  logInfo("SubGHz scanner completed.");
}

void analyzeSubGHzCapture() {
  Serial.println("\n========== SubGHz Analysis ==========\n");

  if (!fileSystemAvailable()) {
    Serial.println("MicroSD not available for analysis.");
    return;
  }

  String capturePath = subGHzCapturePath();
  if (!fileSystemExists(capturePath)) {
    Serial.println("No SubGHz capture data found.");
    return;
  }

  String logData = fileSystemRead(capturePath);
  if (logData.length() == 0) {
    Serial.println("Capture data is empty.");
    return;
  }

  SubGHzCaptureStats stats = summarizeSubGHzCapture(logData);

  Serial.println("=== Frequency Analysis ===");
  if (stats.packetCount > 0) {
    Serial.print("Packets detected: ");
    Serial.println(stats.packetCount);
    Serial.print("Frequency range: ");
    Serial.print(stats.minFreq, 2);
    Serial.print(" - ");
    Serial.print(stats.maxFreq, 2);
    Serial.println(" MHz");
    Serial.print("Average RSSI: ");
    Serial.print(stats.totalSignalStrength / stats.packetCount);
    Serial.println(" dBm");
    Serial.print("Strongest signal: ");
    Serial.print(stats.strongestRSSI);
    Serial.print(" dBm at ");
    Serial.print(stats.strongestFreq, 2);
    Serial.println(" MHz");
  } else {
    Serial.println("No packets detected in capture.");
  }

  Serial.print("Timeouts: ");
  Serial.println(stats.timeoutCount);
  Serial.print("Errors: ");
  Serial.println(stats.errorCount);
  Serial.println("\nUse 'capture export subghz subghz.csv' to export to CSV.");
  Serial.println("Use 'subghz db' to build the signal database.");
  Serial.println();
}

void visualizeSubGHzSpectrum() {
  Serial.println("\n========== SubGHz Spectrum ==========\n");

  if (!fileSystemAvailable()) {
    Serial.println("MicroSD not available.");
    return;
  }

  String capturePath = subGHzCapturePath();
  if (!fileSystemExists(capturePath)) {
    Serial.println("No SubGHz capture data found.");
    return;
  }

  String logData = fileSystemRead(capturePath);
  if (logData.length() == 0) {
    Serial.println("Capture data is empty.");
    return;
  }

  // Build frequency-RSSI map (quantized to 1 MHz buckets)
  const int BUCKET_COUNT = 64; // 300-928 MHz in ~10 MHz buckets
  int buckets[BUCKET_COUNT];
  float bucketFreqs[BUCKET_COUNT];
  memset(buckets, 0, sizeof(buckets));

  float minFreq = 999.0f;
  float maxFreq = 0.0f;

  // Parse capture data
  int lineStart = 0;
  int lineEnd;
  int totalPackets = 0;

  while (lineStart < logData.length()) {
    lineEnd = logData.indexOf('\n', lineStart);
    if (lineEnd < 0) {
      lineEnd = logData.length();
    }

    String line = logData.substring(lineStart, lineEnd);
    line.trim();

    if (line.indexOf("FREQ=") >= 0) {
      int freqIdx = line.indexOf("FREQ=");
      int spaceIdx = line.indexOf(' ', freqIdx);
      if (spaceIdx < 0) {
        spaceIdx = line.length();
      }
      String freqStr = line.substring(freqIdx + 5, spaceIdx);
      float freq = freqStr.toFloat();

      if (freq > 0 && freq >= 300.0f && freq <= 928.0f) {
        if (freq < minFreq) {
          minFreq = freq;
        }
        if (freq > maxFreq) {
          maxFreq = freq;
        }

        // Map to bucket
        int bucketIdx = (int)((freq - 300.0f) / 10.0f);
        if (bucketIdx >= 0 && bucketIdx < BUCKET_COUNT) {
          buckets[bucketIdx]++;
          bucketFreqs[bucketIdx] = freq;
          totalPackets++;
        }
      }
    }

    lineStart = lineEnd + 1;
  }

  if (totalPackets == 0) {
    Serial.println("No valid packets in capture.");
    return;
  }

  // Find max bucket value for scaling
  int maxBucketVal = 0;
  for (int i = 0; i < BUCKET_COUNT; i++) {
    if (buckets[i] > maxBucketVal) {
      maxBucketVal = buckets[i];
    }
  }

  // Display spectrum visualization
  const int GRAPH_HEIGHT = 10;
  const int GRAPH_WIDTH = 64;

  Serial.println("Frequency Activity Spectrum:");
  Serial.println();

  for (int row = GRAPH_HEIGHT; row > 0; row--) {
    Serial.print("│ ");
    for (int col = 0; col < GRAPH_WIDTH; col++) {
      int bucketIdx = (col * BUCKET_COUNT) / GRAPH_WIDTH;
      int barHeight = (buckets[bucketIdx] * GRAPH_HEIGHT) / (maxBucketVal > 0 ? maxBucketVal : 1);
      Serial.print(barHeight >= row ? "█" : " ");
    }
    Serial.println("│");
  }

  Serial.print("└─");
  for (int i = 0; i < GRAPH_WIDTH; i++) {
    Serial.print("─");
  }
  Serial.println("┘");

  Serial.print("  300 MHz");
  for (int i = 0; i < GRAPH_WIDTH - 18; i++) {
    Serial.print(" ");
  }
  Serial.println("928 MHz");

  Serial.println();
  Serial.print("Total packets: ");
  Serial.println(totalPackets);
  Serial.print("Active range: ");
  Serial.print(minFreq, 1);
  Serial.print(" - ");
  Serial.print(maxFreq, 1);
  Serial.println(" MHz");
  Serial.println();
}

void buildSubGHzSignalDatabase() {
  Serial.println("\n========== SubGHz Signal Database ==========\n");

  if (!fileSystemAvailable()) {
    Serial.println("MicroSD not available.");
    return;
  }

  String capturePath = subGHzCapturePath();
  if (!fileSystemExists(capturePath)) {
    Serial.println("No SubGHz capture data found.");
    return;
  }

  String logData = fileSystemRead(capturePath);
  if (logData.length() == 0) {
    Serial.println("Capture data is empty.");
    return;
  }

  String database;
  database.reserve(1024);
  database += "timestamp_ms,frequency_mhz,rssi_dbm,length_bytes,hex,base64\n";

  int records = 0;
  int lineStart = 0;

  while (lineStart < logData.length()) {
    int lineEnd = logData.indexOf('\n', lineStart);
    if (lineEnd < 0) {
      lineEnd = logData.length();
    }

    String line = logData.substring(lineStart, lineEnd);
    line.trim();

    float frequency = 0.0f;
    int rssi = 0;
    String hexPayload;
    String base64Payload;

    if (parseSubGHzCaptureLine(line, frequency, rssi, hexPayload, base64Payload)) {
      database += String(getCaptureTimestamp(line));
      database += ",";
      database += String(frequency, 2);
      database += ",";
      database += String(rssi);
      database += ",";
      database += String(payloadLengthFrom(hexPayload, base64Payload));
      database += ",";
      database += escapeCSV(hexPayload);
      database += ",";
      database += escapeCSV(base64Payload);
      database += "\n";
      records++;
    }

    lineStart = lineEnd + 1;
  }

  if (records == 0) {
    Serial.println("No packet records found. Timeouts and errors are not stored in the database.");
    return;
  }

  String databasePath = subGHzDatabasePath();
  if (!fileSystemWrite(databasePath, database)) {
    Serial.println("Failed to write signal database.");
    logError("SubGHz signal database write failed.");
    return;
  }

  Serial.print("Signal records: ");
  Serial.println(records);
  Serial.print("Database path: ");
  Serial.println(databasePath);
  logInfo("SubGHz signal database updated.");
}
