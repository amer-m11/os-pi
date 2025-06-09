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
#include "tests/shell_test.h"
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

void counting_function(char *array)
{
    printf("Process started with array (%s) in task %d\n\r", array,
           get_scheduler()->current_task->id);
    while (1)
    {
        for (int i = 0; i < 5; i++)
        {
            printf("%c ", array[i]);
            delay(3000000);
        }
    }
}

void test_running_two_tasks_in_parallel()
{
    printf("\n\r=== TEST: Running two tasks in parallel ===\n\r");

    printf("address of process function: 0x%x\n\r", (uint64_t)counting_function);
    uint32_t res = fork((uint64_t)&counting_function, 2, (uint64_t)"12345");

    if (res != 0)
    {
        printf("Error creating task A\n\r");
        return;
    }

    printf("Scheduler state:\n\r");
    scheduler_struct *scheduler = get_scheduler();
    printf("Current task: %d\n\r", scheduler->current_task->id);
    printf("Total tasks: %d (expected: 2)\n\r", scheduler->nr_tasks);

    task_struct *new_task = get_scheduler()->tasks[scheduler->nr_tasks - 1];
    printf("New task priority: %x (expected: 2)\n\r", new_task->priority);
    printf("New task state: %x (expected: 1)\n\r", new_task->state);
    printf("New task remaining time: %x (expected: 2)\n\r", new_task->remaining_time);
    printf("New task disable preemption %x (expected: 1)\n\r", new_task->disable_preemption);

    printf("x19 (function): 0x%x (expected: 0x%x)\n\r", new_task->cpu_context.x19,
           counting_function);
    printf("x20 (arg): %s (expected: 12345)\n\r", new_task->cpu_context.x20);
    printf("PC: 0x%x (expected: 0x%x)\n\r", new_task->cpu_context.pc, ret_from_fork);
    printf("New task address: 0x%x (expected: starting of page)\n\r", new_task);
    printf("SP: 0x%x (expected: end of page)\n\r", new_task->cpu_context.sp);

    res = fork((uint64_t)counting_function, 1, (uint64_t)"abcde");
    if (res != 0)
    {
        printf("Error creating task B\n\r");
        return;
    }
    printf("Current task: %d\n\r", scheduler->current_task->id);
    printf("Total tasks: %d\n\r", scheduler->nr_tasks);

    for (int i = 0; i < scheduler->nr_tasks; i++)
    {
        if (scheduler->tasks[i])
        {
            printf("\tTask %d: state=%d, rem_time=%d\n\r", i, scheduler->tasks[i]->state,
                   scheduler->tasks[i]->remaining_time);
        }
    }

    while (1)
    {
        scheduler->current_task->remaining_time = 0;
        schedule();
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

    // ----------------------- printf -----------------------
    uart_send_string("Initializing printf...");
    init_printf(0, putc);
    uart_send_string("done\r\n\r\n\r\n");

    // ----------------------- exception levels configuration -----------------------
    printf("Kernel running on exception level: %d <- confirm = 3\r\n", get_current_el());
    printf("System Control Register (SCTLR_EL1): 0x%x\r\n", get_sctlr_el1());
    printf("Hypervisor Configuration Register (HCR_EL2): 0x%x\r\n", get_hcr_el2());
    printf("Secure Configuration Register (SCR_EL3): 0x%x\r\n", get_scr_el3());
    printf("Saved Program Status Register (SPSR_EL1): 0x%x\r\n", get_spsr_el1());
    printf("Configuring exception levels...\r\n");
    configure_el1();
    configure_el2();
    configure_el3();
    printf("System Control Register (SCTLR_EL1): 0x%x <- confirm = 0x30d00800 or 0x30d00801\r\n",
           get_sctlr_el1());
    printf("Hypervisor Configuration Register (HCR_EL2): 0x%x <- confirm = 0x80000000\r\n",
           get_hcr_el2());
    printf("Secure Configuration Register (SCR_EL3): 0x%x <- confirm = 0x431\r\n", get_scr_el3());
    printf("Switching to EL1...\r\n");
    switch_to_el1_from_el3();
    printf("Kernel running on exception level: %d <- confirm = 1\r\n", get_current_el()); // EL1
    printf("Saved Program Status Register (SPSR_EL1): 0x%x <- TODO: confirm that 0x10 is the "
           "correct value\r\n",
           get_spsr_el1());
    uart_send_string("\r\n\r\n");

    // ----------------------- x16 -----------------------
    // set x16 to zero for later use as a debugging register
    // NOTE: x16 is usually used to hold the system call number
    printf("x16: 0x%x\r\n", get_x16());
    set_x16(0);
    printf("x16: 0x%x <- confirm = 0x0\r\n", get_x16());
    uart_send_string("\r\n\r\n");

    // ----------------------- interrupts cpu specific -----------------------

    printf("vector base register: 0x%x\r\n", get_vector_base_register());
    printf("DAIF register: 0x%x\r\n", get_daif_register());
    printf("Initializing exception vectors and enabling IRQ...\r\n");
    init_exception_vectors_el1();
    enable_irq();
    printf("vector base register: 0x%x <- confirm = 0x%x\r\n", get_vector_base_register(),
           get_vectors_adr());
    printf("DAIF register: 0x%x <- confirm = 0x140\r\n", get_daif_register());
    printf("\r\n\r\n");

    // ----------------------- interrupts pi board specific -----------------------
    printf("irq0_pending_0 addr: 0x%x <- confirm = 0xfe00b200\r\n", &REGS_IRQ->irq0_pending_0);
    printf("irq0_enable_0 addr: 0x%x <- confirm = 0xfe00b200\r\n", &REGS_IRQ->irq0_enable_0);
    printf("AUX_IRQ : 0x%x <- confirm = 0x20000000\r\n", AUX_IRQ);
    printf("enabling interrupt controller...\r\n");
    enable_interrupt_controller();
    //     printf("irq0_enable_0: 0x%x <- confirm = 0x20000002\r\n", REGS_IRQ->irq0_enable_0);
    printf("irq0_enable_0: 0x%x <- confirm = 0x2\r\n", REGS_IRQ->irq0_enable_0);
    timer1_init(); // TODO: test and adjust interval time for timer interrupt

    printf("\r\n\r\n");

    // ######################## end of setup ########################
    // test_input_stream_struct();
    printf("Running shell tests...\r\n");
    // test_shell();
    // test_shell_comprehensive();
    // test_running_two_tasks_in_parallel();

    // Initialize and run the actual shell instead of uart_echo
    struct InputStream *input_stream = get_input_stream();
    struct Shell *shell = get_shell();
    shell_init(shell, input_stream, "OS-Pi Shell");

    printf("Shell initialized. Starting interactive mode...\r\n");
    // Print the banner
    print_block_banner(); // Choose which banner you want
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
            write_char(input_stream, '\n'); // Feed newline to input stream
        }
        else
        {
            uart_send(received_char);                // Echo the character
            write_char(input_stream, received_char); // Feed to input stream
        }

        // Process shell commands when we have data
        if (input_stream->has_data)
        {
            char command[STREAM_BUFFER_SIZE];
            get_shell_command(shell, command, STREAM_BUFFER_SIZE);
            int result = run_shell_command(shell, command);

            if (result == 1) // tictac game mode
            {
                clear_input_stream(input_stream);
                printf("Starting Tic Tac Toe game...\r\n");
                tictac_run(input_stream);
            }
            else
            {
                clear_input_stream(input_stream);
            }
            printf("\r\n> "); // Show prompt
        }
    }
}