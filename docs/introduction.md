# BlackCapy Introduction

BlackCapy is a professional portable toolkit built around ESP32 hardware, native microSD storage, physical controls and expansion modules.

It is designed for field workflows that need hardware interaction, wireless analysis, automation, diagnostics and structured evidence capture.

The project is educational in the sense that the documentation should teach clearly, but the product direction is professional.

---

## What BlackCapy Is

BlackCapy is:

* A modular embedded firmware
* A hardware-facing toolkit
* A portable diagnostics and capture platform
* A base for future desktop, CLI, analyzer, flasher and sync tools

It should feel powerful, understandable and controllable.

---

## What BlackCapy Is Not

BlackCapy is not:

* A toy firmware
* A collection of unrelated tools
* A serial-only project
* A place for duplicate managers or parallel registries
* A firmware that hides operational evidence in generic logs

---

## Current Development Stage

The current firmware is a stable base that compiles under PlatformIO and establishes the core architecture:

* `AppManager` owns app registration and execution
* `ModuleManager` detects external modules
* `ModuleManifest` defines supported module types
* `Logger` records system behavior
* `CaptureWriter` records operational evidence
* `FileSystem` and `StoragePolicy` manage native microSD paths
* `UIController`, `ScreenManager` and `DisplayManager` prepare the physical UI flow

Serial is still available for development and diagnostics.

---

## Recommended Reading Order

1. `README.md`
2. `usage.md`
3. `docs/architecture.md`
4. `docs/modules.md`
5. `docs/tools.md`
6. `docs/hardware.md`
7. `docs/development_guide.md`
8. `docs/security_policy.md`
