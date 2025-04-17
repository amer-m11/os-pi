#include "common.h"
#include "mini_uart.h"

void kernel_main(void)
{
    uart_init();
    uart_send('\r');
    uart_send('\n');
    uart_send('\r');
    uart_send('\n');
    uart_send_string("Kernel is up and running!\r\n");
    uart_send_string("\r\n");
    uart_send_string("\r\n");
    uart_send_string("All given input will be mirrored:\r\n");

    while (1)
    {
        uart_send(uart_recv());
    }
}