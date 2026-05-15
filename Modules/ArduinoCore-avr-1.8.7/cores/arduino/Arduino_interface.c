#include "Arduino_interface.h"

#include <stdio.h>

#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/timer.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

void arduino_serial_init(void) {
    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART

    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart
}

void arduino_spi_init(void) {
    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi
}

void arduino_tim_init(void) {}

void arduino_hi2c_init(void) {
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c
}

int __io_putchar(int ch) {
    uart_putc(UART_ID, ch);
    return ch;
}
