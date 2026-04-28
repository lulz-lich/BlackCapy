# BlackCapy

BlackCapy is a professional modular embedded toolkit designed for hardware interaction, wireless analysis, automation and system diagnostics.

This project targets ESP32-based devices and is built with a modular architecture to support expansion modules, automation workflows and future advanced capabilities.

---

## Overview

BlackCapy is not an educational toy.

It is a portable technical platform focused on:

* Wireless environment analysis
* Hardware interfacing (GPIO, I2C, SPI, UART)
* Automation execution
* Structured logging
* Modular expansion

The system is designed to evolve into a fully featured portable toolkit.

---

## Current Features

### Core System

* Modular firmware architecture
* Serial-based command interface
* Tool registry system
* Structured logging with timestamps
* Menu system with command parsing

### Tools Implemented

* System Monitor
  Displays chip info, memory usage, uptime and system data

* WiFi Scanner
  Scans nearby networks with RSSI, channel, encryption and BSSID

* GPIO Console
  Basic GPIO control and testing (status LED)

* Module Manager
  Scans I2C bus and prepares expansion module interface

* Automation Engine
  Base structure for rule-based execution

* Reboot Tool
  Software reset via command

---

## Project Structure

main/
├── firmware/
├── core/
├── apps/

hardware/
├── schematics/
├── pcb/
├── modules/

scripts/
tests/
docs/
assets/

---

## Requirements

* ESP32 development board
* PlatformIO (recommended) or Arduino IDE
* USB cable

---

## Build & Upload

cd main/firmware
platformio run
platformio run --target upload

---

## Serial Monitor

platformio device monitor

Baud rate: 115200

---

## Usage

After boot:

blackcapy>

Commands:

* Type a number → execute tool
* help → show menu
* clear → clear screen

Example:

1 → System Monitor
2 → WiFi Scanner
3 → GPIO Console

---

## Architecture

Firmware Layer
Core Services
Applications (Tools)
Expansion Modules

### Core Components

* Logger
* Menu System
* Automation Engine
* Module Manager

---

## Expansion Vision

* BLE Scanner
* RF Analyzer
* IR Transceiver
* NFC/RFID
* SD card logging
* Display + physical controls
* Scriptable automation
* AI-assisted analysis

---

## Design Principles

* Modular first
* Performance over aesthetics
* Extensibility over shortcuts
* Clear separation of concerns

---

## Status

Phase: Core Firmware (Stable Base)

---

## Author

Henrique

---
