#pragma once

#include "peripherals/gpio.h"

// see description of GPIO function select register
// in Chapter 5 https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf

// Each FSEL register controls 10 GPIO pins (0:2 bit for first pin, 3:5 for second pin, etc.)
// based on the value of these 3 bits, the GPIO pin can be set to one of the functions defined here.
// Each pin can be used as input, output or one of the alternate functions (see docs).
typedef enum _GpioFunc {
    GFInput = 0,
    GFOutput = 1,
    GFAlt0 = 4,
    GFAlt1 = 5,
    GFAlt2 = 6,
    GFAlt3 = 7,
    GFAlt4 = 3,
    GFAlt5 = 2,
} GpioFunc;

void gpio_pin_set_func(uint8_t pinNumber, GpioFunc func);

void gpio_pin_enable(uint8_t pinNumber);