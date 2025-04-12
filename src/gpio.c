#include "gpio.h"
#include "utils.h"

void gpio_pin_set_func(uint8_t pinNumber, GpioFunc func) {
    uint8_t bit_start_index_in_func_rel_reg = (pinNumber * 3) % 30;
    uint8_t func_sel_index = pinNumber / 10; 

    uint32_t selector = REGS_GPIO->func_select[func_sel_index];
    selector &= ~(7 << bit_start_index_in_func_rel_reg); // clear the 3 bits for this pin
    selector |= (func << bit_start_index_in_func_rel_reg); // set the 3 bits with the given function

    REGS_GPIO->func_select[func_sel_index] = selector;
}

void gpio_pin_enable(uint8_t pinNumber) {
    REGS_GPIO->pull_up_down_enable = 0;
    delay(150);
    REGS_GPIO->pull_up_down_clock[pinNumber / 32] = 1 << (pinNumber % 32);
    delay(150);
    REGS_GPIO->pull_up_down_enable = 0;
    REGS_GPIO->pull_up_down_clock[pinNumber / 32] = 0;
}