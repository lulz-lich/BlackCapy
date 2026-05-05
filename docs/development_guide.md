# BlackCapy Development Guide

## Overview

This document defines how BlackCapy must be developed.

Every contribution must preserve:

- Performance
- Modularity
- Clean architecture
- Hardware control
- Professional usability

Never sacrifice architecture for speed of implementation.

---

# Development Flow

Before writing code:

Ask:

Can this be integrated into an existing module?

If yes:

Refactor.

If no:

Create a new module.

---

# Module Creation Rules

A new module is allowed only if:

- Responsibility is unique
- Existing modules cannot absorb it cleanly
- Performance will not be reduced
- It improves professional capability

Never create:

- ManagerV2
- HandlerV2
- RendererV2
- Temporary duplicate systems

Wrong:

logging_v2/
tool_manager/
display_renderer_v2/

Correct:

Refactor:

logging/
app_manager/
display/

---

# Folder Responsibilities

## firmware/

Contains:

- boot logic
- hardware startup
- main loop

Never place business logic here.

---

## core/

Contains:

Reusable system modules.

Examples:

- logging
- storage
- events
- input
- display
- screens

Core modules never depend on apps.

Apps depend on core.

Dependency direction:

apps -> core

Never:

core -> apps

---

## apps/

Contains user-facing tools.

Each app must have:

- one folder
- one header
- one implementation
- one entry function

Example:

wifi/

runWifiScanner();

---

# App Standard

Every app must expose one entry function and be registered once through `AppManager`:

```cpp
void runExampleTool();
```

Registration lives in `main/firmware/src/main.cpp` as a seven-field `AppEntry`:

```cpp
{
  TOOL_EXAMPLE_TOOL,
  "Example Tool",
  "hardware",
  "Short description",
  APP_PERMISSION_GPIO,
  APP_STATUS_STABLE,
  runExampleTool
}
```

Do not create a second tool list or a per-folder registry.

---

# Memory Rules

ESP32 memory is limited.

Avoid:

- dynamic allocation
- String abuse
- duplicated buffers

Prefer:

- static allocation
- const char*
- stack allocation

Wrong:

```cpp
String log = "Running...";
```

Better:

```cpp
const char* log = "Running...";
```

---

# Logging Rules

Every important action must log.

Examples:

- boot
- app launch
- module detection
- errors
- shutdown

Log levels:

- INFO
- WARN
- ERROR
- DEBUG

Use `Logger` for system behavior that must be retained. Apps may mirror short technical output to Serial while the serial console remains a debug interface, but Serial output must not replace `Logger`, `CaptureWriter` or the physical display.

Wrong:

```cpp
Serial.println("Boot");
```

Correct:

```cpp
logInfo("Boot");
```

---

# Input Rules

Buttons generate events.

Buttons never directly control apps.

Wrong:

button pressed -> launch app

Correct:

button -> UI Controller -> Screen -> AppManager

---

# Display Rules

Display rendering must be stateless.

Rendering never changes business logic.

Wrong:

display updates settings

Correct:

display only renders state

---

# Pull Request Checklist

Before merging:

[ ] No duplicate systems created  
[ ] No hardcoded pins  
[ ] No direct Serial prints  
[ ] AppManager remains source of truth  
[ ] Performance tested  
[ ] Logs added  
[ ] Documentation updated

---

BlackCapy is not a prototype.

Build like production.
