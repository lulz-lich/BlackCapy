#include "ai_policy.h"

static String redactAfterKey(String text, const String& key) {
  int position = 0;

  while (position < text.length()) {
    int keyIndex = text.indexOf(key, position);
    if (keyIndex < 0) {
      break;
    }

    int valueStart = keyIndex + key.length();
    int valueEnd = text.indexOf('\n', valueStart);
    if (valueEnd < 0) {
      valueEnd = text.length();
    }

    text = text.substring(0, valueStart) + "[REDACTED]" + text.substring(valueEnd);
    position = valueStart + 10;
  }

  return text;
}

String aiPolicyRedactSensitiveText(const String& input) {
  String redacted = input;

  redacted.replace("Authorization: Bearer ", "Authorization: Bearer [REDACTED]");
  redacted = redactAfterKey(redacted, "api_key=");
  redacted = redactAfterKey(redacted, "token=");
  redacted = redactAfterKey(redacted, "password=");
  redacted = redactAfterKey(redacted, "ssid_password=");

  return redacted;
}

String aiPolicyLimitText(const String& input, int maxChars) {
  if (maxChars <= 0 || input.length() <= (unsigned)maxChars) {
    return input;
  }

  return input.substring(input.length() - maxChars);
}

String aiPolicyEscapeJSON(const String& input) {
  String escaped;
  escaped.reserve(input.length() + 16);

  for (int i = 0; i < input.length(); i++) {
    char c = input[i];

    switch (c) {
      case '"':
        escaped += "\\\"";
        break;
      case '\\':
        escaped += "\\\\";
        break;
      case '\n':
        escaped += "\\n";
        break;
      case '\r':
        escaped += "\\r";
        break;
      case '\t':
        escaped += "\\t";
        break;
      default:
        if ((uint8_t)c < 0x20) {
          escaped += " ";
        } else {
          escaped += c;
        }
        break;
    }
  }

  return escaped;
}
