#include "tests/sched_test.h"
#include "mm.h"
#include "scheduler/sched.h"
#include "utils/printf.h"

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
    // printf("\n\r=== dummy_function address: 0x%x ===\n\r\n\r", DUMMY_ADDR);
    RUN_TEST(test_init_task);
    RUN_TEST(test_local_scheduler_struct);
    RUN_TEST(test_global_scheduler_test);
    RUN_TEST(test_fork_success_basic);
    RUN_TEST(test_fork_context_fields);
    RUN_TEST(test_copy_process_allocation_success);
    RUN_TEST(test_multiple_tasks_created_correctly);
    RUN_TEST(test_new_tasks_added_to_task_array);
    RUN_TEST(test_task_stack_does_not_overlap);
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

void test_fork_success_basic(void)
{
    printf("\n\r=== TEST: fork basic success ===\n\r");
    uint64_t current_count = get_schedular()->nr_tasks;
    uint32_t result = fork((uint64_t)dummy_function, 5, 0xabc);
    printf("Fork result: %x (expected: 0)\n\r", result);
    printf("New task PID: %x\n\r", current_count);
    task_struct *new_task = get_schedular()->tasks[current_count];
    printf("New task address: 0x%x\n\r", new_task);
    printf("New task state: %x (expected: 1)\n\r", new_task->state);
    printf("New task priority: %x (expected: 5)\n\r", new_task->priority);
    printf("New task remaining time: %x (expected: 5)\n\r", new_task->remaining_time);
    printf("New task PC (should be ret_from_fork): 0x%x\n\r", new_task->cpu_context.pc);
}

void test_fork_context_fields(void)
{
    printf("\n\r=== TEST: fork cpu context setup ===\n\r");
    uint64_t current_count = get_schedular()->nr_tasks;
    fork((uint64_t)dummy_function, 2, 0x22222222);
    task_struct *new_task = get_schedular()->tasks[current_count];

    printf("x19 (function): 0x%x (expected: 0x%x)\n\r", new_task->cpu_context.x19, DUMMY_ADDR);
    printf("x20 (arg): 0x%x (expected: 0x22222222)\n\r", new_task->cpu_context.x20);
    printf("SP: 0x%x\n\r", new_task->cpu_context.sp);
    printf("PC: 0x%x (expected: ret_from_fork addr)\n\r", new_task->cpu_context.pc);
}

void test_copy_process_allocation_success()
{
    printf("=== TEST: copy process allocation ===\n\r");

    uint64_t function = (uint64_t)dummy_function;
    uint64_t priority = 2;
    uint64_t arg = 0xdeadbeef;

    uint32_t result = fork(function, priority, arg);
    printf("fork() result: %d (expected: 0)\n\r", result);
    printf("Total tasks: %d (expected: 2)\n\r", get_schedular()->nr_tasks);

    task_struct *new_task = get_schedular()->tasks[1];
    printf("New task address: 0x%x\n\r", new_task);
    printf("New task != init_task: %s\n\r", (new_task != get_init_task()) ? "PASS" : "FAIL");

    printf("\n\r");
}

void test_multiple_tasks_created_correctly()
{
    printf("=== TEST: multiple task creation ===\n\r");

    for (int i = 0; i < 3; i++)
    {
        fork((uint64_t)dummy_function, 3 + i, 0x2000 + i);
    }

    uint64_t total_tasks = get_schedular()->nr_tasks;
    printf("Total tasks: %d (expected: 4)\n\r", total_tasks); // init + 3 new

    for (int i = 1; i < total_tasks; i++)
    {
        task_struct *task = get_schedular()->tasks[i];
        printf("Task %d address: 0x%x | priority: %d | arg(x20): 0x%x\n\r", i, task, task->priority,
               task->cpu_context.x20);
    }

    printf("\n\r");
}

void test_new_tasks_added_to_task_array()
{
    printf("=== TEST: task array update ===\n\r");

    fork((uint64_t)dummy_function, 4, 0xbbbb);
    fork((uint64_t)dummy_function, 5, 0xdddd);

    printf("Task 1 function (x19): 0x%x (expected: 0x%x)\n\r",
           get_schedular()->tasks[1]->cpu_context.x19, DUMMY_ADDR);
    printf("Task 2 function (x19): 0x%x (expected: 0x%x)\n\r",
           get_schedular()->tasks[2]->cpu_context.x19, DUMMY_ADDR);

    printf("nr_tasks = %d (expected: 3)\n\r", get_schedular()->nr_tasks);
    printf("\n\r");
}

void test_task_stack_does_not_overlap()
{
    printf("=== TEST: stack allocation separation ===\n\r");

    fork((uint64_t)dummy_function, 1, 0x1);
    fork((uint64_t)dummy_function, 1, 0x2);

    task_struct *task1 = get_schedular()->tasks[1];
    task_struct *task2 = get_schedular()->tasks[2];

    printf("Task 1 SP: 0x%x\n\r", task1->cpu_context.sp);
    printf("Task 2 SP: 0x%x\n\r", task2->cpu_context.sp);

    int overlap = (task1 == task2) || (task1->cpu_context.sp == task2->cpu_context.sp);
    printf("Stacks overlap: %s (expected: NO)\n\r", overlap ? "YES - FAIL" : "NO - PASS");

    printf("\n\r");
}
