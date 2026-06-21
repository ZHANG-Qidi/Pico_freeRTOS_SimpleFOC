#ifndef _svpwm_src_h_
#define _svpwm_src_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "pico/stdlib.h"
#include "svpwm.pio.h"

void pio_pwm_init(PIO *pio, uint *sm, uint *offset, uint pin, uint freq);
void pio_pwm_set_level(PIO pio, uint sm, uint32_t level);

#ifdef __cplusplus
}
#endif

#endif