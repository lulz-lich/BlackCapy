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

// SPI
#define SPI_MOSI_PIN 23
#define SPI_MISO_PIN 19
#define SPI_SCK_PIN 18
#define SPI_CS_PIN 5

// UART
#define UART_RX_PIN 16
#define UART_TX_PIN 17
#define UART_BAUDRATE 9600

// General hardware tool defaults
#define PWM_TEST_PIN 18
#define PWM_TEST_CHANNEL 0
#define PWM_TEST_FREQUENCY 5000
#define PWM_TEST_RESOLUTION 8

#define ANALOG_TEST_PIN 34

// Native microSD
#define SD_CS_PIN SPI_CS_PIN

// Module detection
#define MODULE_ID_ADC_PIN 36

// IR
#define IR_RX_PIN 34
#define IR_TX_PIN 4

// RFID RC522
#define RFID_CS_PIN 15
#define RFID_RST_PIN 27

// NFC PN532
#define NFC_IRQ_PIN 32
#define NFC_RST_PIN 33

// GPS
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
#define GPS_BAUDRATE 9600

// LoRa SX127x
#define LORA_CS_PIN 5
#define LORA_DIO0_PIN 26
#define LORA_RST_PIN 14
#define LORA_DIO1_PIN 35
#define LORA_FREQUENCY 915.0

// RF CC1101
#define RF_CS_PIN 13
#define RF_GDO0_PIN 25
#define RF_RST_PIN 12
#define RF_GDO2_PIN 39
#define RF_FREQUENCY 433.92

// CAN MCP2515
#define CAN_CS_PIN 2
#define CAN_SPEED CAN_500KBPS

#endif
