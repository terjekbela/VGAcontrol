#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

#define vgacolor_640x480_wrap_target 2
#define vgacolor_640x480_wrap 8

static const uint16_t vgacolor_640x480_program_instructions[] = {
    0x80a0, //  0: pull   block                      
    0xa047, //  1: mov    y, osr                     
            //     .wrap_target
    0xe000, //  2: set    pins, 0                    
    0xa022, //  3: mov    x, y                       
    0x23c1, //  4: wait   1 irq, 1               [3] 
    0x80a0, //  5: pull   block                      
    0x6404, //  6: out    pins, 4                [4] 
    0x6204, //  7: out    pins, 4                [2] 
    0x0045, //  8: jmp    x--, 5                     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program vgacolor_640x480_program = {
    .instructions = vgacolor_640x480_program_instructions,
    .length = 9,
    .origin = -1,
};

static inline pio_sm_config vgacolor_640x480_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + vgacolor_640x480_wrap_target, offset + vgacolor_640x480_wrap);
    return c;
}

static inline void vgacolor_640x480_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = vgacolor_640x480_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin, 4);
    sm_config_set_out_pins(&c, pin, 4);
    pio_gpio_init(pio, pin);
    pio_gpio_init(pio, pin+1);
    pio_gpio_init(pio, pin+2);
    pio_gpio_init(pio, pin+3);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 4, true);
    pio_sm_init(pio, sm, offset, &c);
}
#endif