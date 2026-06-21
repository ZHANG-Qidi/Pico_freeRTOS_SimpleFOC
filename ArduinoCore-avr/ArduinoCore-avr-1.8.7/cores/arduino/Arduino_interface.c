#include "Arduino_interface.h"

#include <stdio.h>

#include "hardware/uart.h"

int __io_putchar(int ch) {
    uart_putc(UART_MASTER_NUM, ch);
    return ch;
}
