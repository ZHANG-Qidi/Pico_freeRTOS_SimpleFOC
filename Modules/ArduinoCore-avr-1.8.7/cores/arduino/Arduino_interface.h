#ifndef _ARDUINO_INTERFACE_H_
#define _ARDUINO_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

// I2C
#define I2C_MASTER_NUM i2c0         /*!< I2C port number for master dev */
#define I2C_MASTER_SCL_IO (5)       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO (4)       /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_FREQ_HZ (400000) /*!< I2C master clock frequency */

// SPI
#define SPI_MASTER_NUM spi0
#define SPI_MASTER_MOSI_IO (19)
#define SPI_MASTER_MISO_IO (16)
#define SPI_MASTER_SCLK_IO (18)
#define SPI_MASTER_CS_IO (17)

// UART
#define UART_MASTER_NUM uart0
#define UART_MASTER_TX_IO (0)
#define UART_MASTER_RX_IO (1)

// PWM
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
