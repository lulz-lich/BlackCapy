#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// ===============================
// BlackCapy Hardware Configuration
// ===============================

// Display
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

// Navigation buttons
#define BTN_UP_PIN 32
#define BTN_DOWN_PIN 33
#define BTN_LEFT_PIN 25
#define BTN_RIGHT_PIN 26
#define BTN_OK_PIN 27
#define BTN_BACK_PIN 14

// Status
#define STATUS_LED_PIN 2
#define BUZZER_PIN 13

// I2C
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// UART expansion
#define UART_RX_PIN 16
#define UART_TX_PIN 17
#define UART_BAUDRATE 9600

// SPI
#define SPI_MOSI_PIN 23
#define SPI_MISO_PIN 19
#define SPI_SCK_PIN 18
#define SPI_CS_PIN 5

#endif