#include "common.h"
#include "mini_uart.h"

void kernel_main(void)
{
	uart_init();
	uart_send_string("I'mWorking\r\n");
	uart_send_string("Hello,world!\r\n");
	uart_send_string("Hello, world!\r\n");

	while (1) {
		uart_send(uart_recv());
	}
}