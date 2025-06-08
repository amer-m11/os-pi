#include "tests/sched_test.h"
#include "mm.h"
#include "scheduler/sched.h"
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
    // printf("\n\r=== dummy_function address: 0x%x ===\n\r\n\r", DUMMY_ADDR);
    RUN_TEST(test_init_task);
    RUN_TEST(test_local_scheduler_struct);
    RUN_TEST(test_global_scheduler_test);

    // Task creation tests
    RUN_TEST(test_fork_success_basic);
    RUN_TEST(test_multiple_tasks_created_correctly);
    RUN_TEST(test_task_stack_does_not_overlap);

    // // Scheduling behavior tests
    RUN_TEST(test_schedule_from_timer);
    RUN_TEST(test_voluntary_yield);
    RUN_TEST(test_remaining_time_decrement);
    RUN_TEST(test_preemption_control);
    RUN_TEST(test_full_scheduling_cycle);
    RUN_TEST(test_mixed_yield_scenarios);

    printf("\n\rAll scheduler tests completed!\n\r");
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

    printf("scheduler address: 0x%x", get_scheduler());
    printf("Number of tasks: %x (expected: 1)\n\r", get_scheduler()->nr_tasks);
    printf("Current task is init_task: %s\n\r",
           (get_scheduler()->current_task == get_init_task()) ? "PASS" : "FAIL");
    printf("First task in array is init_task: %s\n\r",
           (get_scheduler()->tasks[0] == get_init_task()) ? "PASS" : "FAIL");
    printf("Second task in array: 0x%x (expected: \\0)", get_scheduler()->tasks[1]);
    printf("\n\r\n\r");
}

void test_fork_success_basic(void)
{
    printf("\n\r=== TEST: task fields after creation ===\n\r");

    uint64_t current_count = get_scheduler()->nr_tasks;
    uint32_t result = fork((uint64_t)dummy_function, 5, 0xabc);
    printf("Fork result: %x (expected: 0)\n\r", result);
    printf("Total tasks: %d (expected: 2)\n\r", get_scheduler()->nr_tasks);
    printf("New task PID: %x\n\r", current_count);
    task_struct *new_task = get_scheduler()->tasks[current_count];
    printf("New task != init_task: %s\n\r", (new_task != get_init_task()) ? "PASS" : "FAIL");
    printf("New task priority: %x (expected: 5)\n\r", new_task->priority);
    printf("New task state: %x (expected: 1)\n\r", new_task->state);
    printf("New task remaining time: %x (expected: 5)\n\r", new_task->remaining_time);
    printf("New task disable preemption %x (expected: 1)\n\r", new_task->disable_preemption);

    printf("x19 (function): 0x%x (expected: 0x%x)\n\r", new_task->cpu_context.x19, DUMMY_ADDR);
    printf("x20 (arg): 0x%x (expected: 0xabc)\n\r", new_task->cpu_context.x20);
    printf("PC: 0x%x (expected: 0x%x)\n\r", new_task->cpu_context.pc, ret_from_fork);
    printf("New task address: 0x%x (expected: starting of page)\n\r", new_task);
    printf("SP: 0x%x (expected: end of page)\n\r", new_task->cpu_context.sp);
}

void test_multiple_tasks_created_correctly()
{
    printf("=== TEST: multiple task creation ===\n\r");

    for (int i = 0; i < 3; i++)
    {
        fork((uint64_t)dummy_function, 3 + i, 0x2000 + i);
    }

    uint64_t total_tasks = get_scheduler()->nr_tasks;
    printf("Total tasks: %d (expected: 4)\n\r", total_tasks); // init + 3 new

    printf("Task 1 function (x19): 0x%x (expected: 0x%x)\n\r",
           get_scheduler()->tasks[1]->cpu_context.x19, DUMMY_ADDR);
    printf("Task 2 function (x19): 0x%x (expected: 0x%x)\n\r",
           get_scheduler()->tasks[2]->cpu_context.x19, DUMMY_ADDR);

    for (int i = 1; i < total_tasks; i++)
    {
        task_struct *task = get_scheduler()->tasks[i];
        printf("Task %d address: 0x%x | priority: %d | arg(x20): 0x%x\n\r", i, task, task->priority,
               task->cpu_context.x20);
    }

    printf("\n\r");
}

void test_task_stack_does_not_overlap()
{
    printf("=== TEST: stack allocation separation ===\n\r");

    fork((uint64_t)dummy_function, 1, 0x1);
    fork((uint64_t)dummy_function, 1, 0x2);

    task_struct *task1 = get_scheduler()->tasks[1];
    task_struct *task2 = get_scheduler()->tasks[2];

    printf("Task 1 SP: 0x%x\n\r", task1->cpu_context.sp);
    printf("Task 2 SP: 0x%x\n\r", task2->cpu_context.sp);

    int overlap = (task1 == task2) || (task1->cpu_context.sp == task2->cpu_context.sp);
    printf("Stacks overlap: %s (expected: NO)\n\r", overlap ? "YES - FAIL" : "NO - PASS");

    printf("\n\r");
}

// ------------------------------------------------------------

void test_schedule_from_timer(void)
{
    printf("\n\r=== TEST: Timer-driven yield ===\n\r");
    reset_scheduler();

    // Create a test task
    fork(DUMMY_ADDR, 3, 0);
    scheduler_struct *sched = get_scheduler();
    sched->current_task = sched->tasks[1]; // Set new task as current

    // Simulate timer interrupt
    printf("Before timer tick - remaining_time: %d\n\r", sched->current_task->remaining_time);
    scheduler_tick(); // This should internally call yield() when time expires

    printf("After timer tick - remaining_time: %d\n\r", sched->current_task->remaining_time);
    printf("Verify time was decremented: %s\n\r",
           (sched->current_task->remaining_time == 2) ? "PASS" : "FAIL");
}

void test_voluntary_yield(void)
{
    printf("\n\r=== TEST: Voluntary yield ===\n\r");
    reset_scheduler();

    // Create two test tasks
    fork(DUMMY_ADDR, 3, 0x1111);
    fork(DUMMY_ADDR, 2, 0x2222);
    scheduler_struct *sched = get_scheduler();

    // Start with task1
    sched->current_task = sched->tasks[1];
    printf("Starting with Task1 (remaining_time: %d)\n\r", sched->current_task->remaining_time);

    // Task voluntarily yields
    printf("Task1 calling yield() voluntarily\n\r");
    yield();

    printf("Current task after yield: %s\n\r",
           (sched->current_task == sched->tasks[2])   ? "Task2 (PASS)"
           : (sched->current_task == sched->tasks[0]) ? "Init (PASS if no other tasks)"
                                                      : "FAIL");
}

void test_mixed_yield_scenarios(void)
{
    printf("\n\r=== TEST: Mixed yield scenarios ===\n\r");
    reset_scheduler();

    // Create three test tasks with different priorities
    fork(DUMMY_ADDR, 4, 0xaaaa); // High priority
    fork(DUMMY_ADDR, 2, 0xbbbb); // Medium priority
    fork(DUMMY_ADDR, 1, 0xcccc); // Low priority

    scheduler_struct *sched = get_scheduler();
    sched->current_task = sched->tasks[1]; // Start with high priority task

    printf("\n\rScenario 1: Timer tick without expiration\n\r");
    printf("Before: remaining_time = %d\n\r", sched->current_task->remaining_time);
    scheduler_tick();
    printf("After: remaining_time = %d (should decrement but not yield)\n\r",
           sched->current_task->remaining_time);

    printf("\n\rScenario 2: Voluntary yield\n\r");
    printf("Current task voluntarily yields\n\r");
    yield();
    printf("Now running: %s (should switch to next ready task)\n\r",
           (sched->current_task == sched->tasks[2])   ? "Task2"
           : (sched->current_task == sched->tasks[3]) ? "Task3"
                                                      : "Init");

    printf("\n\rScenario 3: Timer expiration\n\r");
    // Run until time expires
    while (sched->current_task->remaining_time > 0)
    {
        scheduler_tick();
    }
    printf("Time expired, should have yielded to next task\n\r");
    printf("Now running: %s\n\r",
           (sched->current_task != sched->tasks[1]) ? "Another task (PASS)" : "Same task (FAIL)");
}

void test_remaining_time_decrement()
{
    fork(DUMMY_ADDR, 3, 0);
    scheduler_struct *sched = get_scheduler();
    sched->current_task = sched->tasks[1];

    printf("Initial remaining_time: %d\n\r", sched->current_task->remaining_time);
    for (int i = 0; i < 3; i++)
    {
        schedule();
        printf("After tick %d: %d\n\r", i + 1, sched->current_task->remaining_time);
    }
}

void test_preemption_control()
{
    fork(DUMMY_ADDR, 3, 0);
    scheduler_struct *sched = get_scheduler();
    sched->current_task = sched->tasks[1];

    printf("Testing preemption disable...\n\r");
    preempt_disable();
    printf("disable_preemption: %d (expected: 1)\n\r", sched->current_task->disable_preemption);

    uint64_t initial_time = sched->current_task->remaining_time;
    scheduler_tick();
    printf("With preemption disabled, time should not change: %d (expected: %d)\n\r",
           sched->current_task->remaining_time, initial_time);

    preempt_enable();
    printf("disable_preemption: %d (expected: 0)\n\r", sched->current_task->disable_preemption);
    scheduler_tick();
    printf("With preemption enabled, time should decrement: %d (expected: %d)\n\r",
           sched->current_task->remaining_time, initial_time - 1);
}

void test_full_scheduling_cycle()
{
    // Create test tasks
    fork(DUMMY_ADDR, 3, 0x1111); // Task1 - priority 3
    fork(DUMMY_ADDR, 2, 0x2222); // Task2 - priority 2

    scheduler_struct *sched = get_scheduler();
    task_struct *init = sched->tasks[0];
    task_struct *task1 = sched->tasks[1];
    task_struct *task2 = sched->tasks[2];

    // Start with task1
    sched->current_task = task1;
    printf("\n\rStarting with Task1 (priority %d)\n\r", task1->priority);

    // Run through complete scheduling cycles
    for (int cycle = 0; cycle < 2; cycle++)
    {
        printf("\n\r--- Cycle %d ---\n\r", cycle + 1);

        // Task1 should run for 3 ticks
        for (int i = 0; i < 3; i++)
        {
            scheduler_tick();
            printf("Tick %d: %s (time: %d)\n\r", i + 1,
                   (sched->current_task == task1) ? "Task1" : "WRONG",
                   sched->current_task->remaining_time);
        }

        // Task2 should run for 2 ticks
        for (int i = 0; i < 2; i++)
        {
            scheduler_tick();
            printf("Tick %d: %s (time: %d)\n\r", i + 1,
                   (sched->current_task == task2) ? "Task2" : "WRONG",
                   sched->current_task->remaining_time);
        }

        // Should return to init task
        scheduler_tick();
        printf("Final tick: %s\n\r", (sched->current_task == init) ? "Init" : "Other task");
    }
}