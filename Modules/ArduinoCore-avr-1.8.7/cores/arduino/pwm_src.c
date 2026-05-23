#include "pwm_src.h"

#include "hardware/clocks.h"

// Write `period` to the input shift register
void pio_pwm_set_period_basic(PIO pio, uint sm, uint32_t period) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, period);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
    pio_sm_set_enabled(pio, sm, true);
}

// Write `level` to TX FIFO. State machine will copy this into X.
void pio_pwm_set_level_basic(PIO pio, uint sm, uint32_t level) { pio_sm_put_blocking(pio, sm, level); }

void pio_pwm_init_basic(PIO *pio, uint *sm, uint *offset, uint pin, uint freq) {
    // Find a free pio and state machine and add the program
    bool rc = pio_claim_free_sm_and_add_program_for_gpio_range(&pwm_program, pio, sm, offset, pin, 1, true);
    hard_assert(rc);
    pwm_program_init(*pio, *sm, *offset, pin);
    uint period = ((clock_get_hz(clk_sys) / (float)freq) - 10.f) / 2.f;
    pio_pwm_set_period_basic(*pio, *sm, period);
}
