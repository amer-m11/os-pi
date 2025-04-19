#include "mini_uart.h"
#include "gpio.h"
#include "peripherals/auxiliaries.h"
#include "utils.h"

#define TXD 14
#define RXD 15

void uart_init()
{

    gpio_pin_set_func(TXD, alt5);
    gpio_pin_set_func(RXD, alt5);

    gpio_pin_enable(TXD);
    gpio_pin_enable(RXD);

    REGS_AUX->enables = 1; // set first bit to 1
    REGS_AUX->mu_cntl = 0;
    REGS_AUX->mu_ier = 0;
    REGS_AUX->mu_lcr = 3; // 8 bit mode
    REGS_AUX->mu_mcr = 0;
    REGS_AUX->mu_baud = 541; // = 115200 @ 500 MHz
    REGS_AUX->mu_cntl = 3;   // enables transmitter and receiver
}

void uart_send(char letter)
{
    while ((REGS_AUX->mu_lsr & 0x20) == 0)
    {
        // wait until fifth bit is set (ready to send)
    };
    REGS_AUX->mu_io = letter;
}

char uart_recv()
{
    while ((REGS_AUX->mu_lsr & 0x01) == 0)
    {
        // wait until first bit is set (data available)
    };
    return REGS_AUX->mu_io & 0xFF; // return the received character (one byte)
}

void uart_send_string(char *str)
{
    while (*str)
    {
        uart_send(*str++);
    }
}