#include "tests/sched_test.h"
#include "mm.h"
#include "scheduler/sched.h"
#include "utils/printf.h"

void test_scheduler(void)
{
    test_init_task();
    test_local_scheduler_struct();
    test_global_scheduler_test();
}

void test_init_task()
{
    printf("\n\r\n\r");

    printf("=== INITIAL TASK STATE TEST ===\n\r");
    printf("Init task state: %x (expected: 0)\n\r", get_init_task()->state);
    printf("Init task remaining time: %x (expected: 0)\n\r", get_init_task()->remaining_time);
    printf("Init task priority: %x (expected: 1)\n\r", get_init_task()->priority);
    printf("Init task preemption: %x (expected: 0)\n\r", get_init_task()->disable_preemption);

    printf("\n\r\n\r");
}

void test_local_scheduler_struct()
{
    uint64_t array[1024];
    memzero((uint64_t)array, 1024);

    scheduler_struct *scheduler = (scheduler_struct *)array;
    scheduler->tasks[0] = get_init_task();
    scheduler->current_task = get_init_task();
    scheduler->nr_tasks = 1;

    printf("\n\r\n\r");

    printf("=== LOCAL scheduler INITIAL STATE TEST ===\n\r");

    printf("scheduler address: 0x%x\n\r", scheduler);
    printf("Number of tasks: %x (expected: 1)\n\r", scheduler->nr_tasks);
    printf("Current task is init_task: %s\n\r",
           (scheduler->current_task == get_init_task()) ? "PASS" : "FAIL");
    printf("First task in array is init_task: %s\n\r",
           (scheduler->tasks[0] == get_init_task()) ? "PASS" : "FAIL");

    printf("\n\r\n\r");
}

void test_global_scheduler_test()
{
    printf("\n\r\n\r");
    printf("=== GLOBAL scheduler INITIAL STATE TEST ===\n\r");

    printf("scheduler address: 0x%x", get_schedular());
    printf("Number of tasks: %x (expected: 1)\n\r", get_schedular()->nr_tasks);
    printf("Current task is init_task: %s\n\r",
           (get_schedular()->current_task == get_init_task()) ? "PASS" : "FAIL");
    printf("First task in array is init_task: %s\n\r",
           (get_schedular()->tasks[0] == get_init_task()) ? "PASS" : "FAIL");
    printf("Second task in array: 0x%x (expected: \\0)", get_schedular()->tasks[1]);
    printf("\n\r\n\r");
}
