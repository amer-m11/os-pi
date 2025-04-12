#include "gpio.h"
#include "utils.h"
#include "peripherals/aux.h"
#include "mini_uart.h"

#define TXD 14
#define RXD 15

void uart_init() {

    gpio_pin_set_func(TXD, GFAlt5);
    gpio_pin_set_func(RXD, GFAlt5);
    
    gpio_pin_enable(TXD);
    gpio_pin_enable(RXD);

    REGS_AUX->enables = 1; // set first bit to 1
    REGS_AUX->mu_cntl = 0;
    REGS_AUX->mu_ier = 0;
    REGS_AUX->mu_lcr = 3; // 8 bit mode
    REGS_AUX->mu_mcr = 0;
    REGS_AUX->mu_baud = 541; // = 115200 @ 500 MHz
    REGS_AUX->mu_cntl = 3; // enables transmitter and receiver

    uart_send('\r');
    uart_send('\n');
    uart_send('\n');
}

void uart_send(char c) {
    while (REGS_AUX->mu_lsr & 0x20 == 0); // wait until fifth bit is set (ready to send)
    REGS_AUX->mu_io = c;
}

char uart_recv() {
    while (REGS_AUX->mu_lsr & 0x01 == 0); // wait until first bit is set (data available)
    return REGS_AUX->mu_io & 0xFF; // return the received character (one byte)
}

void uart_send_string(char *str) {
    while (*str) {
        if (*str == '\n') {
            uart_send('\r'); // send carriage return before line feed
        }
        uart_send(*str++);
    }
}