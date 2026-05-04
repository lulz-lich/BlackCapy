#ifndef AI_POLICY_H
#define AI_POLICY_H

#include <Arduino.h>

String aiPolicyRedactSensitiveText(const String& input);
String aiPolicyLimitText(const String& input, int maxChars);
String aiPolicyEscapeJSON(const String& input);

#endif
