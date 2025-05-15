#include "common.h"
#include "io/mini_uart.h"
#include "utils/el.h"
#include "utils/printf.h"

void putc(void *p, char c)
{
    uart_send(c);
}

void kernel_main(void)
{
    uart_init();
    uart_send('\r');
    uart_send('\n');
    uart_send('\r');
    uart_send('\n');
    uart_send_string("Kernel is up and running!\r\n");

    uart_send_string("Initializing printf...");
    init_printf(0, putc);
    uart_send_string("done\r\n");

    printf("Kernel running on exception level: %d \r\n", get_current_el());

    uart_send_string("\r\n");
    uart_send_string("\r\n");
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