#include "mm.h"
#include "scheduler/sched.h"
#include "tests/tests.h"
#include "utils.h"
#include "utils/printf.h"

// TODO: clean up this file

#define RUN_TEST(fn)                                                                               \
    do                                                                                             \
    {                                                                                              \
        reset_scheduler();                                                                         \
        fn();                                                                                      \
    } while (0)

void dummy_function(void) {}

#define DUMMY_ADDR ((uint64_t)dummy_function)

// placeholder function pointer for fork()

void test_scheduler(void)
{
    RUN_TEST(test_init_task);
    RUN_TEST(test_local_scheduler_struct);
    RUN_TEST(test_global_scheduler_test);

    RUN_TEST(test_fork_success_basic);
    RUN_TEST(test_multiple_tasks_created_correctly);
}

void test_init_task()
{
    printf("\n\r\n\r");
    printf("=== INITIAL TASK STATE TEST ===\n\r");

    task_struct *init_task = get_init_task();

    printf("Init task state: %x (expected: 0) - %s\n\r", init_task->state,
           (init_task->state == 0) ? PASS_STR : FAIL_STR);

    printf("Init task remaining time: %x (expected: 0) - %s\n\r", init_task->remaining_time,
           (init_task->remaining_time == 0) ? PASS_STR : FAIL_STR);

    printf("Init task priority: %x (expected: 1) - %s\n\r", init_task->priority,
           (init_task->priority == 1) ? PASS_STR : FAIL_STR);

    printf("Init task preemption: %x (expected: 0) - %s\n\r", init_task->disable_preemption,
           (init_task->disable_preemption == 0) ? PASS_STR : FAIL_STR);

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
    printf("Number of tasks: %x (expected: 1) - %s\n\r", scheduler->nr_tasks,
           (scheduler->nr_tasks == 1) ? PASS_STR : FAIL_STR);
    printf("Current task is init_task: %s\n\r",
           (scheduler->current_task == get_init_task()) ? PASS_STR : FAIL_STR);
    printf("First task in array is init_task: %s\n\r",
           (scheduler->tasks[0] == get_init_task()) ? PASS_STR : FAIL_STR);

    printf("\n\r\n\r");
}

void test_global_scheduler_test()
{
    printf("\n\r\n\r");
    printf("=== GLOBAL scheduler INITIAL STATE TEST ===\n\r");

    scheduler_struct *scheduler = get_scheduler();
    printf("scheduler address: 0x%x\n\r", scheduler);

    printf("Number of tasks: %x (expected: 1) - %s\n\r", scheduler->nr_tasks,
           (scheduler->nr_tasks == 1) ? PASS_STR : FAIL_STR);

    printf("Current task is init_task: %s\n\r",
           (scheduler->current_task == get_init_task()) ? PASS_STR : FAIL_STR);

    printf("First task in array is init_task: %s\n\r",
           (scheduler->tasks[0] == get_init_task()) ? PASS_STR : FAIL_STR);

    printf("Second task in array: 0x%x (expected: 0x0) - %s\n\r", scheduler->tasks[1],
           (scheduler->tasks[1] == 0) ? PASS_STR : FAIL_STR);

    printf("\n\r\n\r");
}

void test_fork_success_basic(void)
{
    printf("\n\r\n\r");
    printf("=== TEST: task fields after creation ===\n\r");

    uint64_t current_count = get_scheduler()->nr_tasks;
    uint32_t result = fork((uint64_t)dummy_function, 5, 0xabc);

    printf("Fork result: %x (expected: 0) - %s\n\r", result, (result == 0) ? PASS_STR : FAIL_STR);

    printf("Total tasks: %d (expected: 2) - %s\n\r", get_scheduler()->nr_tasks,
           (get_scheduler()->nr_tasks == 2) ? PASS_STR : FAIL_STR);

    printf("New task PID: %x\n\r", current_count);

    task_struct *new_task = get_scheduler()->tasks[current_count];
    printf("New task != init_task: %s\n\r", (new_task != get_init_task()) ? PASS_STR : FAIL_STR);

    printf("New task priority: %x (expected: 5) - %s\n\r", new_task->priority,
           (new_task->priority == 5) ? PASS_STR : FAIL_STR);

    printf("New task state: %x (expected: 1) - %s\n\r", new_task->state,
           (new_task->state == 1) ? PASS_STR : FAIL_STR);

    printf("New task remaining time: %x (expected: 5) - %s\n\r", new_task->remaining_time,
           (new_task->remaining_time == 5) ? PASS_STR : FAIL_STR);

    printf("New task disable preemption %x (expected: 1) - %s\n\r", new_task->disable_preemption,
           (new_task->disable_preemption == 1) ? PASS_STR : FAIL_STR);

    printf("x19 (function): 0x%x (expected: 0x%x) - %s\n\r", new_task->cpu_context.x19, DUMMY_ADDR,
           (new_task->cpu_context.x19 == DUMMY_ADDR) ? PASS_STR : FAIL_STR);

    printf("x20 (arg): 0x%x (expected: 0xabc) - %s\n\r", new_task->cpu_context.x20,
           (new_task->cpu_context.x20 == 0xabc) ? PASS_STR : FAIL_STR);

    printf("PC: 0x%x (expected: 0x%x) - %s\n\r", new_task->cpu_context.pc, REF_FROM_FORM_ADR,
           (new_task->cpu_context.pc == REF_FROM_FORM_ADR) ? PASS_STR : FAIL_STR);

    printf("New task address: 0x%x (expected: starting of page)\n\r", new_task);
    printf("SP: 0x%x (expected: end of page)\n\r", new_task->cpu_context.sp);

    printf("\n\r\n\r");
}

void test_multiple_tasks_created_correctly()
{
    printf("\n\r\n\r");
    printf("=== TEST: multiple task creation ===\n\r");

    for (int i = 0; i < 3; i++)
    {
        uint32_t result = fork((uint64_t)dummy_function, 3 + i, 0x2000 + i);
        printf("Fork %d result: %x (expected: 0) - %s\n\r", i, result,
               (result == 0) ? PASS_STR : FAIL_STR);
    }

    uint64_t total_tasks = get_scheduler()->nr_tasks;
    printf("Total tasks: %d (expected: 4) - %s\n\r", total_tasks,
           (total_tasks == 4) ? PASS_STR : FAIL_STR);

    printf("Task 1 function (x19): 0x%x (expected: 0x%x) - %s\n\r",
           get_scheduler()->tasks[1]->cpu_context.x19, DUMMY_ADDR,
           (get_scheduler()->tasks[1]->cpu_context.x19 == DUMMY_ADDR) ? PASS_STR : FAIL_STR);

    printf("Task 2 function (x19): 0x%x (expected: 0x%x) - %s\n\r",
           get_scheduler()->tasks[2]->cpu_context.x19, DUMMY_ADDR,
           (get_scheduler()->tasks[2]->cpu_context.x19 == DUMMY_ADDR) ? PASS_STR : FAIL_STR);

    printf("Task 3 function (x19): 0x%x (expected: 0x%x) - %s\n\r",
           get_scheduler()->tasks[3]->cpu_context.x19, DUMMY_ADDR,
           (get_scheduler()->tasks[3]->cpu_context.x19 == DUMMY_ADDR) ? PASS_STR : FAIL_STR);

    for (int i = 1; i < total_tasks; i++)
    {
        task_struct *task = get_scheduler()->tasks[i];
        uint64_t expected_priority = 3 + (i - 1);
        uint64_t expected_arg = 0x2000 + (i - 1);

        printf("Task %d priority: %d (expected: %d) - %s\n\r", i, task->priority, expected_priority,
               (task->priority == expected_priority) ? PASS_STR : FAIL_STR);

        printf("Task %d arg(x20): 0x%x (expected: 0x%x) - %s\n\r", i, task->cpu_context.x20,
               expected_arg, (task->cpu_context.x20 == expected_arg) ? PASS_STR : FAIL_STR);

        printf("Task %d address: 0x%x\n\r", i, task);
    }

    printf("\n\r\n\r");
}
