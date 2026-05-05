#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// ===============================
// BlackCapy Hardware Configuration
// ===============================

// Display
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define BLACKCAPY_DISPLAY_BACKEND_TFT 1
#define DISPLAY_TFT_CS_PIN 15
#define DISPLAY_TFT_DC_PIN 2
#define DISPLAY_TFT_RST_PIN 4
#define DISPLAY_TFT_BL_PIN -1
#define DISPLAY_TFT_ROTATION 1

// Navigation buttons
#define BLACKCAPY_INPUT_BACKEND_ADC 1
#define BTN_ADC_PIN 39
#define BTN_ADC_TOLERANCE 180
#define BTN_UP_ADC_VALUE 520
#define BTN_DOWN_ADC_VALUE 1120
#define BTN_LEFT_ADC_VALUE 1720
#define BTN_RIGHT_ADC_VALUE 2320
#define BTN_OK_ADC_VALUE 2920
#define BTN_BACK_ADC_VALUE 3520

// Status
#define STATUS_LED_PIN 25
#define BUZZER_PIN 26

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
#define PWM_TEST_PIN 27
#define PWM_TEST_CHANNEL 0
#define PWM_TEST_FREQUENCY 5000
#define PWM_TEST_RESOLUTION 8

#define ANALOG_TEST_PIN 34

// Native microSD
#define SD_CS_PIN SPI_CS_PIN

// Module detection
#define MODULE_ID_ADC_PIN 36

// Product expansion slot
// External capability modules are mutually exclusive and share these control lines.
#define EXPANSION_SPI_CS_PIN 13
#define EXPANSION_IRQ_PIN 32
#define EXPANSION_RST_PIN 33
#define EXPANSION_IO1_PIN 27
#define EXPANSION_IO2_PIN 35
#define EXPANSION_ANALOG_PIN 34

// Development features
#define BLACKCAPY_ENABLE_MODULE_MOCKS 0

// IR
#define IR_RX_PIN EXPANSION_IRQ_PIN
#define IR_TX_PIN EXPANSION_IO1_PIN

// RFID RC522
#define RFID_CS_PIN EXPANSION_SPI_CS_PIN
#define RFID_RST_PIN EXPANSION_RST_PIN

// NFC PN532
#define NFC_IRQ_PIN EXPANSION_IRQ_PIN
#define NFC_RST_PIN EXPANSION_RST_PIN

// GPS
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
#define GPS_BAUDRATE 9600

// LoRa SX127x
#define LORA_CS_PIN EXPANSION_SPI_CS_PIN
#define LORA_DIO0_PIN EXPANSION_IRQ_PIN
#define LORA_RST_PIN EXPANSION_RST_PIN
#define LORA_DIO1_PIN EXPANSION_IO2_PIN
#define LORA_FREQUENCY 915.0

// RF CC1101
#define RF_CS_PIN EXPANSION_SPI_CS_PIN
#define RF_GDO0_PIN EXPANSION_IRQ_PIN
#define RF_RST_PIN EXPANSION_RST_PIN
#define RF_GDO2_PIN EXPANSION_IO2_PIN
#define RF_FREQUENCY 433.92

// CAN MCP2515
#define CAN_CS_PIN EXPANSION_SPI_CS_PIN
#define CAN_SPEED CAN_500KBPS

#endif
