#include "common.h"
#include "io/mini_uart.h"
#include "scheduler/sched.h"
#include "scheduler/task.h"
#include "setup.h"
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

    test_running_two_tasks_in_parallel();

    uart_echo();
}