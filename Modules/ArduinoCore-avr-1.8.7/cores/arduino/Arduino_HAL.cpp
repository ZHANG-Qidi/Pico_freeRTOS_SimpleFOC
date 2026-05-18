#include "Arduino_HAL.h"

#include "Arduino.h"
#include "Arduino_interface.h"
#include "FreeRTOS.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "task.h"

// CPU / Global settings
volatile uint8_t SREG;

// USART registers (dummy AVR compatibility)
volatile uint8_t UBRRH;
volatile uint8_t UBRRL;
volatile uint8_t UCSRA;
volatile uint8_t UCSRB;
volatile uint8_t UCSRC;
volatile uint8_t UDR;

volatile uint8_t UBRR0H;
volatile uint8_t UBRR0L;
volatile uint8_t UCSR0A;
volatile uint8_t UCSR0B;
volatile uint8_t UCSR0C;
volatile uint8_t UDR0;

// SPI registers (dummy AVR compatibility)
volatile uint8_t SPCR;
volatile uint8_t SPSR;
volatile uint8_t SPDR;

void delayMicroseconds(unsigned int us) { sleep_us(us); }

// delay with FreeRTOS awareness
void delay(unsigned long ms) {
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        sleep_ms(ms);
    } else {
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
}

unsigned long micros(void) { return (unsigned long)time_us_64(); }

void digitalWrite(uint8_t pin, uint8_t val) { gpio_put(pin, val); }

void pinMode(uint8_t pin, uint8_t mode) {
    gpio_init(pin);
    switch (mode) {
        case OUTPUT:
            gpio_set_dir(pin, GPIO_OUT);
            break;
        case INPUT:
            gpio_set_dir(pin, GPIO_IN);
            gpio_disable_pulls(pin);
            break;
        case INPUT_PULLUP:
            gpio_set_dir(pin, GPIO_IN);
            gpio_pull_up(pin);
            break;
        case INPUT_PULLDOWN:
            gpio_set_dir(pin, GPIO_IN);
            gpio_pull_down(pin);
            break;
    }
}

void analogWrite(uint8_t pin, int value) {
    if (value < 0) value = 0;
    if (value > 255) value = 255;
    uint slice = pwm_gpio_to_slice_num(pin);
    uint channel = pwm_gpio_to_channel(pin);
    pwm_set_chan_level(slice, channel, value);
}

void analogWriteInit(uint8_t pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    uint channel = pwm_gpio_to_channel(pin);
    uint32_t sys_clk = clock_get_hz(clk_sys);
    float target_pwm_freq = 20000.0f;
    float wrap = 255.0f;
    float clkdiv = (float)sys_clk / (target_pwm_freq * (wrap + 1));
    pwm_set_clkdiv(slice, clkdiv);
    pwm_set_wrap(slice, wrap);
    pwm_set_enabled(slice, true);
}

// function setting the high pwm frequency to the supplied pins
// - BLDC motor - 3PWM setting
// - hardware speciffic
// in generic case dont do anything
void _configure3PWM(long pwm_frequency, const int pinA, const int pinB, const int pinC) {
    (void)(pwm_frequency);
    analogWriteInit(pinA);
    analogWriteInit(pinB);
    analogWriteInit(pinC);
    uint sliceA = pwm_gpio_to_slice_num(pinA);
    uint sliceB = pwm_gpio_to_slice_num(pinB);
    uint sliceC = pwm_gpio_to_slice_num(pinC);
    pwm_set_enabled(sliceA, false);
    pwm_set_enabled(sliceB, false);
    pwm_set_enabled(sliceC, false);
    uint32_t mask = (1u << sliceA) | (1u << sliceB) | (1u << sliceC);
    pwm_set_mask_enabled(mask);
}
