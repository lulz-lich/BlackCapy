#include "command_shell.h"
#include "app_manager.h"
#include "status_manager.h"
#include "storage.h"
#include "logger.h"
#include "ui.h"
#include "menu.h"
#include "automation_engine.h"
#include "display_manager.h"
#include "capture_writer.h"
#include "storage_policy.h"
#include "subghz_scanner.h"
#include "ai_client.h"
#include "ai_analyzer.h"
#include "ai_report_viewer.h"
#include "blackcapy.h"

static int parseIdFromCommand(const String& command, const String& prefix) {
  String value = command.substring(prefix.length());
  value.trim();
  return value.toInt();
}

static bool shellIsSafeFileName(const String& filename) {
  if (filename.length() == 0 || filename.length() > 48) {
    return false;
  }

  if (filename.indexOf("..") >= 0 || filename.indexOf('/') >= 0 || filename.indexOf('\\') >= 0) {
    return false;
  }

  for (int i = 0; i < filename.length(); i++) {
    char c = filename[i];
    bool valid =
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') ||
        c == '_' ||
        c == '-' ||
        c == '.';

    if (!valid) {
      return false;
    }
  }

  return true;
}

bool shellHandleCommand(const String& rawCommand) {
  String command = rawCommand;
  command.trim();

  if (command == "registry") {
    appManagerPrintAll();
    return true;
  }

  if (command == "help") {
    menuPrint();
    return true;
  }

  if (command == "clear") {
    Serial.write(27);
    Serial.print("[2J");
    Serial.write(27);
    Serial.print("[H");
    menuPrintHeader();
    menuPrint();
    return true;
  }

  if (command == "apps" || command == "list") {
    appManagerPrintAll();
    return true;
  }

  if (command.startsWith("run ")) {
    int id = parseIdFromCommand(command, "run ");

    if (id > 0) {
      if (!appManagerRunById(id)) {
        logWarn("Invalid app id: " + String(id));
      }
      return true;
    }
  }

  if (command == "status") {
    statusPrint();
    return true;
  }

  if (command == "profile") {
    String profile = storageGetString("profile", "operator");
    Serial.print("Current profile: ");
    Serial.println(profile);
    return true;
  }

  if (command == "heap") {
    Serial.print("Free heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
    return true;
  }

  if (command == "about") {
    uiShowSplash();
    return true;
  }

  if (command == "clear_storage") {
    storageClear();
    logWarn("Storage cleared by shell command.");
    return true;
  }

  if (command.startsWith("script ")) {
    String scriptName = command.substring(7);
    scriptName.trim();
    if (scriptName.length() > 0) {
      automationExecuteScript(scriptName.c_str());
      return true;
    }
  }

  if (command == "theme") {
    const ThemeConfig* theme = displayGetThemeConfig();
    Serial.print("Current theme: ");
    Serial.println(theme->name);
    return true;
  }

  if (command.startsWith("theme ")) {
    String themeName = command.substring(6);
    themeName.trim();
    themeName.toLowerCase();

    if (themeName == "dark") {
      displaySetTheme(THEME_DARK);
    } else if (themeName == "light") {
      displaySetTheme(THEME_LIGHT);
    } else if (themeName == "terminal") {
      displaySetTheme(THEME_TERMINAL);
    } else if (themeName == "matrix") {
      displaySetTheme(THEME_MATRIX);
    } else if (themeName == "minimal") {
      displaySetTheme(THEME_MINIMAL);
    } else {
      Serial.println("Available themes: dark, light, terminal, matrix, minimal");
      return true;
    }

    const ThemeConfig* newTheme = displayGetThemeConfig();
    Serial.print("Theme changed to: ");
    Serial.println(newTheme->name);
    return true;
  }

  if (command == "capture stats") {
    Serial.println("Available captures:");

    for (int i = 0; i < captureCategoryCount(); i++) {
      Serial.print("- ");
      Serial.println(captureCategoryAt(i));
    }

    return true;
  }

  if (command.startsWith("capture stats ")) {
    String category = command.substring(14);
    category.trim();

    if (!captureCategoryIsSupported(category)) {
      Serial.println("Unsupported capture category.");
      return true;
    }

    capturePrintStats(category);
    return true;
  }

  if (command.startsWith("capture export ")) {
    String args = command.substring(15);
    args.trim();
    int spaceIdx = args.indexOf(' ');
    if (spaceIdx > 0) {
      String category = args.substring(0, spaceIdx);
      String outputFile = args.substring(spaceIdx + 1);
      outputFile.trim();

      if (!captureCategoryIsSupported(category)) {
        Serial.println("Unsupported capture category.");
        return true;
      }

      if (!shellIsSafeFileName(outputFile)) {
        Serial.println("Invalid output filename.");
        return true;
      }
      
      String fullPath = storagePolicyGetCapturesPath() + "/" + outputFile;
      if (captureExportToCSV(category, fullPath)) {
        Serial.println("CSV export successful!");
      } else {
        Serial.println("CSV export failed.");
      }
      return true;
    } else {
      Serial.println("Usage: capture export <category> <filename>");
      Serial.println("Example: capture export subghz subghz_scan.csv");
      return true;
    }
  }

  if (command == "analyze subghz" || command == "analyze rf") {
    analyzeSubGHzCapture();
    return true;
  }

  if (command == "analyze") {
    Serial.println("Available analysis commands:");
    Serial.println("- analyze subghz  (analyze Sub-GHz capture data)");
    Serial.println("- analyze rf      (alias for analyze subghz)");
    Serial.println("- ai analyze      (send redacted context to AI gateway)");
    return true;
  }

  if (command == "spectrum" || command == "spectrum subghz") {
    visualizeSubGHzSpectrum();
    return true;
  }

  if (command == "subghz" || command == "subghz help") {
    Serial.println("SubGHz commands:");
    Serial.println("- run " + String(TOOL_SUBGHZ_SCANNER) + "            (start configured scan)");
    Serial.println("- analyze subghz    (summarize captures)");
    Serial.println("- spectrum subghz   (draw activity spectrum)");
    Serial.println("- subghz db         (build /signals/subghz_signals.csv)");
    return true;
  }

  if (command == "subghz db") {
    buildSubGHzSignalDatabase();
    return true;
  }

  if (command == "ai" || command == "ai help") {
    Serial.println("AI commands:");
    Serial.println("- ai status        (show gateway configuration)");
    Serial.println("- ai analyze       (run AI Analyzer app)");
    Serial.println("- ai report        (show last AI report)");
    Serial.println("- run " + String(TOOL_AI_ANALYZER) + "           (same app through AppManager)");
    Serial.println("Config: /ai/gateway.cfg");
    Serial.println("Reports: /reports/ai_last_report.txt");
    return true;
  }

  if (command == "ai analyze" || command == "analyze ai") {
    runAIAnalyzer();
    return true;
  }

  if (command == "ai status" || command == "status ai") {
    aiClientPrintStatus();
    return true;
  }

  if (command == "ai report" || command == "report ai") {
    runAIReportViewer();
    return true;
  }

  return false;
}
