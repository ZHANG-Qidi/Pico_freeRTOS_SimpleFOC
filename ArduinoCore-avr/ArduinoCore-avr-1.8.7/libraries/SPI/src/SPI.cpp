/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPI.h"

#include "pico/stdlib.h"

SPIClass SPI;

void SPIClass::begin() {
    spi_init(SPI_MASTER_NUM, 10 * 1000 * 1000);
    spi_set_format(SPI_MASTER_NUM, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_set_function(SPI_MASTER_MISO_IO, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MASTER_CS_IO, GPIO_FUNC_SIO);
    gpio_set_function(SPI_MASTER_SCLK_IO, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MASTER_MOSI_IO, GPIO_FUNC_SPI);
}

void SPIClass::end() {
    spi_deinit(SPI_MASTER_NUM);
    gpio_set_function(SPI_MASTER_MISO_IO, GPIO_FUNC_SIO);
    gpio_set_function(SPI_MASTER_CS_IO, GPIO_FUNC_SIO);
    gpio_set_function(SPI_MASTER_SCLK_IO, GPIO_FUNC_SIO);
    gpio_set_function(SPI_MASTER_MOSI_IO, GPIO_FUNC_SIO);
    gpio_set_dir(SPI_MASTER_MISO_IO, GPIO_IN);
    gpio_set_dir(SPI_MASTER_CS_IO, GPIO_IN);
    gpio_set_dir(SPI_MASTER_SCLK_IO, GPIO_IN);
    gpio_set_dir(SPI_MASTER_MOSI_IO, GPIO_IN);
    gpio_disable_pulls(SPI_MASTER_MISO_IO);
    gpio_disable_pulls(SPI_MASTER_CS_IO);
    gpio_disable_pulls(SPI_MASTER_SCLK_IO);
    gpio_disable_pulls(SPI_MASTER_MOSI_IO);
}

void SPIClass::usingInterrupt(uint8_t interruptNumber) {}

void SPIClass::notUsingInterrupt(uint8_t interruptNumber) {}
