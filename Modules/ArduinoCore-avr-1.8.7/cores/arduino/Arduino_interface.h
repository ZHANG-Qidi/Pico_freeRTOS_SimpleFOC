#ifndef _ARDUINO_INTERFACE_H_
#define _ARDUINO_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS 17
#define PIN_SCK 18
#define PIN_MOSI 19

// I2C defines
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5

// UART defines
#define UART_ID uart0
#define BAUD_RATE 115200

// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define MOTOR_A (10)
#define MOTOR_B (11)
#define MOTOR_C (12)
#define MOTOR_EN (13)

void arduino_serial_init(void);
void arduino_spi_init(void);
void arduino_tim_init(void);
void arduino_hi2c_init(void);

#ifdef __cplusplus
}
#endif

#endif
