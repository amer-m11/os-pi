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
#include "utils.h"
#include "utils/debug.h"
#include "utils/printf.h"

void putc(void *pointer, char char_to_send)
{
    (void)pointer; // Ignore unused parameter
    uart_send(char_to_send);
}

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

void test_memory_allocation(void)
{
    printf("\n=== Initial Memory State ===\n");
    print_memory_map();
    printf("current stack pointer: 0x%x\n", get_sp());

    printf("\n=== Allocating first page ===\n");
    uint64_t page1 = allocate_page();
    printf("Allocated first page at address: 0x%x\n", page1);
    print_memory_map();

    printf("\n=== Allocating second page ===\n");
    uint64_t page2 = allocate_page();
    printf("Allocated second page at address: 0x%x\n", page2);
    print_memory_map();

    printf("\n=== Allocating third page ===\n");
    uint64_t page3 = allocate_page();
    printf("Allocated third page at address: 0x%x\n", page3);
    print_memory_map();

    printf("\n=== Freeing second page ===\n");
    free_page(page2);
    printf("Freed second page at address: 0x%x\n", page2);
    print_memory_map();

    printf("\n=== Allocating new page (should reuse freed page) ===\n");
    uint64_t page4 = allocate_page();
    printf("Allocated new page at address: 0x%x (expected 0x%x)\n", page4, page2);
    print_memory_map();

    printf("\n=== Allocating fourth page ===\n");
    uint64_t page5 = allocate_page();
    printf("Allocated fourth page at address: 0x%x\n", page5);
    print_memory_map();

    printf("\n=== Final Memory State ===\n");
    printf("current stack pointer: 0x%x\n", get_sp());
    print_memory_map();
}

void print_task_info(task_struct *task, int index)
{
    if (!task)
    {
        printf("  tasks[%d]: NULL\n", index);
        return;
    }

    printf("  tasks[%d] @ 0x%x:\n", index, (uint64_t)task);
    printf("    state: %d\n", task->state);
    printf("    priority: %d\n", task->priority);
    printf("    remaining_time: %d\n", task->remaining_time);
    printf("    preempt: %d\n", task->disable_preemption);
    printf("    stack: 0x%x\n", task->cpu_context.sp);
    printf("    pc: 0x%x\n", task->cpu_context.pc);
}

void test_scheduler_setup(void)
{
    printf("\n=== Scheduler Initialization ===\n");

    // Print scheduler metadata
    printf("Scheduler @ 0x%x:\n", (uint64_t)SCHEDULER);
    printf("  current_task @ 0x%x\n", (uint64_t)SCHEDULER->current_task);
    printf("  nr_tasks: %d\n", SCHEDULER->nr_tasks);

    // Print all tasks in the array
    printf("\nTask List:\n");
    for (int i = 0; i < MAX_TASKS_NUMBER; i++)
    {
        print_task_info(SCHEDULER->tasks[i], i);
    }

    // Print current task details
    if (SCHEDULER->current_task)
    {
        printf("\nCurrent Task Details:\n");
        print_task_info(SCHEDULER->current_task, -1);
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

    // ######################## start of setup ########################

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
    test_memory_allocation();
    printf("\r\n\r\n");
    test_scheduler_setup();
    printf("\r\n\r\n");

    uart_echo();
}