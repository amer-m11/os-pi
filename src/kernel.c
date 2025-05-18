#include "common.h"
#include "el.h"
#include "io/mini_uart.h"
#include "utils/debug.h"
#include "utils/printf.h"

void putc(void *pointer, char char_to_send)
{
    (void)pointer; // Ignore unused parameter
    uart_send(char_to_send);
}

void kernel_main(void)
{
    // ----------------------- first alive signal -----------------------
    uart_init();
    uart_send('\r');
    uart_send('\n');
    uart_send('\r');
    uart_send('\n');
    uart_send_string("Kernel is up and running!\r\n");
    uart_send_string("\r\n\r\n");

    // ----------------------- printf -----------------------
    uart_send_string("Initializing printf...");
    init_printf(0, putc);
    uart_send_string("done\r\n\r\n\r\n");

    // ----------------------- exception levels configuration -----------------------
    configure_el2();
    configure_el1();
    printf("Kernel running on exception level: %d \r\n", get_current_el()); // EL2
    printf("Switching to EL1...\r\n");
    switch_to_el1();
    printf("Kernel running on exception level: %d \r\n", get_current_el()); // EL1
    uart_send_string("\r\n\r\n");

    // ----------------------- x16 -----------------------
    // set x16 to zero for later use as a debugging register
    // NOTE: x16 is usually used to hold the system call number
    printf("x16: %x\r\n", get_x16());
    set_x16(0);
    printf("x16: %x <- confirm that the value is zero\r\n", get_x16());
    uart_send_string("\r\n\r\n");

    printf("All given input will be mirrored:\r\n");

    while (1)
    {
        char received_char = uart_recv();
        if (received_char == '\r')
        {
            uart_send('\r');
            uart_send('\n');
            continue;
        }
        uart_send(received_char);
    }
}