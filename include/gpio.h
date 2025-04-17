#pragma once

#include "peripherals/gpio.h"

/**
 * Each FSEL register controls 10 GPIO pins (0:2 bit for first pin, 3:5 for
 * second pin, etc.) based on the value of these 3 bits, the GPIO pin can be set
 * to one of the functions defined here. see description of GPIO function select
 * register in Chapter 5 https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf
 */
typedef enum GpioFunc_
{
    input = 0,
    output = 1,
    alt0 = 4,
    alt1 = 5,
    alt2 = 6,
    alt3 = 7,
    alt4 = 3,
    alt5 = 2,
} GpioFunc;

/**
 * Set the function of a given GPIO pin. Each pin can be used as input, output
 * or one of the alternate functions (see docs).
 * @param func The function to set the pin to.
 */
void gpio_pin_set_func(uint8_t pinNumber, GpioFunc func);

/**
 * Disable the pull-up/down resistors for a given GPIO pin rendering it floating.
 */
void gpio_pin_enable(uint8_t pinNumber);