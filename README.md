# BlackCapy

⠀⠀⢀⣀⠤⠿⢤⢖⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⡔⢩⠂⠀⠒⠗⠈⠀⠉⠢⠄⣀⠠⠤⠄⠒⢖⡒⢒⠂⠤⢄⠀⠀⠀⠀
⠇⠤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠈⠀⠈⠈⡨⢀⠡⡪⠢⡀⠀
⠈⠒⠀⠤⠤⣄⡆⡂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠢⠀⢕⠱⠀
⠀⠀⠀⠀⠀⠈⢳⣐⡐⠐⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠁⠇
⠀⠀⠀⠀⠀⠀⠀⠑⢤⢁⠀⠆⠀⠀⠀⠀⠀⢀⢰⠀⠀⠀⡀⢄⡜⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠘⡦⠄⡷⠢⠤⠤⠤⠤⢬⢈⡇⢠⣈⣰⠎⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣃⢸⡇⠀⠀⠀⠀⠀⠈⢪⢀⣺⡅⢈⠆⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠶⡿⠤⠚⠁⠀⠀⠀⢀⣠⡤⢺⣥⠟⢡⠃⠀⠀⠀⠀⠀⠀⠀⠀

BlackCapy is a modular embedded systems project inspired by the concept of the Flipper Zero. The goal of this project is to develop a portable hardware platform capable of interacting with multiple electronic communication protocols while also serving as an educational tool for studying embedded systems, wireless communication, and security testing.

The project focuses on combining hardware design, firmware development, and interface design into a single device that can be expanded and customized. BlackCapy is designed not only as a functional tool but also as a learning platform for experimentation with electronics, signals, and communication protocols.

## Project Goals

The main goal of BlackCapy is to create a flexible and modular device that allows users to explore how electronic systems communicate. The platform aims to help students and developers understand topics such as RF communication, RFID technologies, infrared protocols, and embedded programming.

Another important objective is to provide a system that can be extended through hardware modules and firmware plugins, encouraging experimentation and collaborative development.

## Hardware Architecture

The hardware architecture is based on a central microcontroller responsible for coordinating all communication modules and managing the user interface.

The system includes several integrated modules:

* Sub-GHz RF transceiver for wireless signal interaction
* RFID / NFC reader for studying card and tag protocols
* Infrared transmitter and receiver for remote control signals
* OLED display for the graphical interface
* Navigation buttons and buzzer for user interaction
* Flash storage for saving captured signals and plugins
* Rechargeable Li-Po battery with charging circuit
* Expansion port for additional modules

This modular hardware structure allows the platform to evolve as new modules and features are developed.

## Firmware

The firmware is responsible for controlling the hardware and providing the user interface. It is designed with a modular architecture that allows new functionalities to be added through plugins.

The firmware manages:

* hardware drivers and communication interfaces
* signal capture and reproduction
* data storage and device configuration
* menu navigation and user interface
* plugin loading and execution

This architecture allows the system to grow without requiring large modifications to the core code.

## Interface and Visual Design

BlackCapy uses a lightweight graphical interface designed for small displays. The visual style is based on pixel art and simple animations, giving the device a unique personality while maintaining low computational requirements.

The interface may include:

* pixel art characters and icons
* animated status indicators
* ASCII style boot screens
* customizable visual themes

This approach combines technical functionality with a distinctive aesthetic identity.

## Educational Security Tools

One of the core ideas of the project is to provide tools that help users study how electronic devices communicate. The device can be used to observe and analyze signals from various systems.

Examples of educational tools include:

* RF signal scanning and analysis
* infrared signal capture and replay
* RFID protocol exploration
* communication protocol visualization
* basic signal analysis utilities

These features are intended for research and educational purposes related to embedded systems and communication technologies.

## Expandability

BlackCapy is designed to support expansion both in hardware and software.

Hardware expansion allows external modules to be connected through GPIO or communication interfaces such as SPI, I2C, or UART.

Software expansion is possible through a plugin architecture that allows developers to create new tools and functionalities without modifying the entire firmware.

## Repository Contents

This repository includes:

* project documentation
* architecture diagrams
* hardware design concepts
* firmware structure ideas
* interface and visual design concepts

## Project Status

BlackCapy is currently in the conceptual and design stage. The repository is intended to document the development process and serve as a collaborative platform for future improvements.

## License

This project is open for experimentation, research, and educational purposes.
