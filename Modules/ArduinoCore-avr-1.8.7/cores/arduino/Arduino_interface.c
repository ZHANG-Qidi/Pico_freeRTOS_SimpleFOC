#include "Arduino_interface.h"

#include <stdio.h>

#include "hardware/uart.h"

void arduino_serial_init(void) {}

void arduino_spi_init(void) {}

void arduino_tim_init(void) {}

void arduino_hi2c_init(void) {}

int __io_putchar(int ch) {
    uart_putc(UART_MASTER_NUM, ch);
    return ch;
}
