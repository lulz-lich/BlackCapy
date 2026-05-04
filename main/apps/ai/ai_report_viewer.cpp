#include <Arduino.h>

#include "filesystem.h"
#include "logger.h"
#include "storage_policy.h"
#include "ui.h"

static String aiReportPath() {
  return storagePolicyGetReportsPath() + "/ai_last_report.txt";
}

static String tailText(const String& text, int maxChars) {
  if (text.length() <= (unsigned)maxChars) {
    return text;
  }

  return text.substring(text.length() - maxChars);
}

void runAIReportViewer() {
  Serial.println();
  Serial.println("========== AI REPORT ==========");

  if (!fileSystemAvailable()) {
    uiShowStatusError("microSD unavailable.");
    logWarn("AI report viewer blocked: microSD unavailable.");
    return;
  }

  String path = aiReportPath();
  if (!fileSystemExists(path)) {
    Serial.println("No AI report found.");
    Serial.println("Run 'ai analyze' first.");
    Serial.println("===============================");
    return;
  }

  String report = fileSystemRead(path);
  if (report.length() == 0) {
    Serial.println("AI report is empty.");
    Serial.println("===============================");
    return;
  }

  Serial.print("Path: ");
  Serial.println(path);
  Serial.println();
  Serial.println(tailText(report, 2400));
  Serial.println("===============================");
}
