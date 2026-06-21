#ifndef _pwm_src_h_
#define _pwm_src_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "pico/stdlib.h"
#include "pwm.pio.h"

void pio_pwm_set_level_basic(PIO pio, uint sm, uint32_t level);
void pio_pwm_init_basic(PIO *pio, uint *sm, uint *offset, uint pin, uint freq);

#ifdef __cplusplus
}
#endif

#endif