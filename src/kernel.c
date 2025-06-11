#include "common.h"
#include "io/mini_uart.h"
#include "regs/peripherals/irq.h"
#include "regs/peripherals/timer.h"
#include "scheduler/sched.h"
#include "scheduler/task.h"
#include "setup.h"
#include "shell/banner.h"
#include "shell/input_stream.h"
#include "shell/shell.h"
#include "shell/tictac.h"
#include "utils.h"
#include "utils/debug.h"
#include "utils/printf.h"

void uart_echo(void)
{
    printf("UART echo mode activated. Input will be echoed back:\r\n");
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

void launch_shell(void)
{
    print_block_banner();
    printf("> ");

    // Interactive shell loop - read from UART and feed to input stream
    while (1)
    {
        char received_char = uart_recv();

        // Handle carriage return/newline
        if (received_char == '\r')
        {
            uart_send('\r');
            uart_send('\n');
            write_char(get_input_stream(), '\n'); // Feed newline to input stream
        }
        else
        {
            uart_send(received_char);                      // Echo the character
            write_char(get_input_stream(), received_char); // Feed to input stream
        }

        // Process shell commands when we have data
        if (get_input_stream()->has_data)
        {
            char command[STREAM_BUFFER_SIZE];
            get_shell_command(get_shell(), command, STREAM_BUFFER_SIZE);
            int result = run_shell_command(get_shell(), command);

            if (result == 1) // tictac game mode
            {
                clear_input_stream(get_input_stream());
                printf("Starting Tic Tac Toe game...\r\n");
                tictac_run(get_input_stream());
            }
            else
            {
                clear_input_stream(get_input_stream());
            }
            printf("\r\n> "); // Show prompt
        }
    }
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

    setup_kernel();

    // test_running_two_tasks_in_parallel();

    launch_shell();
}