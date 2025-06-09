#include "setup.h"
#include "common.h"
#include "drivers/timer.h"
#include "el.h"
#include "interrupt/daif.h"
#include "interrupt/handler.h"
#include "interrupt/vectors.h"
#include "io/mini_uart.h"
#include "mm.h"
#include "regs/peripherals/irq.h"
#include "regs/peripherals/timer.h"
#include "scheduler/sched.h"
#include "scheduler/task.h"
#include "shell/input_stream.h"
#include "shell/shell.h"
#include "tests/tests.h"
#include "utils.h"
#include "utils/debug.h"
#include "utils/printf.h"

void putc(void *pointer, char char_to_send)
{
    (void)pointer; // Ignore unused parameter
    uart_send(char_to_send);
}

void setup_printf()
{
    uart_send_string("Initializing printf...");
    init_printf(0, putc);
    uart_send_string("Done\r\n\r\n");
}

void configure_els()
{
    printf("Kernel running on exception level: %d (expected: 3)\r\n", get_current_el());
    printf("Configuring exception levels...");
    configure_el1();
    configure_el2();
    configure_el3();
    printf("Done\r\n");
}

void configure_cpu_interrupt_configuration()
{
    printf("Initializing exception vectors and enabling IRQ...");
    init_exception_vectors_el1();
    enable_irq();
    printf("Done\r\n");
}

void configure_board_interrupt_configuration()
{
    printf("Enabling interrupt controller...");
    enable_interrupt_controller();
    printf("Done\r\n");
}

void setup_shell()
{
    printf("Setting up shell...");
    // test_input_stream_struct();
    // test_shell();
    shell_init(get_shell(), get_input_stream(), "OS-Pi Shell");
    printf("Done\r\n");
}

void setup_kernel(void)
{
    setup_printf();
    printf("======= Setting up kernel =======\r\n");

    configure_els();
    test_sys_configuration();

    printf("Switching to EL1...");
    switch_to_el1_from_el3();
    printf("Done\r\n");
    printf("Kernel running on exception level: %d (expected: 1)\r\n", get_current_el());

    configure_cpu_interrupt_configuration();
    test_cpu_interrupt_configuration();

    configure_board_interrupt_configuration();
    test_board_interrupt_configuration();
    timer1_init();

    test_memory_allocation();
    init_scheduler();
    test_scheduler();
    reset_scheduler();

    setup_shell();

    printf("======= Kernel setup complete =======");
}