#include "common.h"
#include "scheduler/sched.h"
#include "utils.h"
#include "utils/printf.h"

void counting_function(char *array)
{
    printf("starting task counting over %s\n\r", array);
    for (int j = 0; j < 10; j++)
    {
        for (int i = 0; i < 5; i++)
        {
            printf("%c ", array[i]);
            delay(10000000);
        }
    }
    get_scheduler()->current_task->state = TASK_FINISHED_STATE;
    while (1)
    {
    }
}

void test_running_two_tasks_in_parallel()
{
    printf("\n\r=== TEST: Running two tasks in parallel ===\n\r");
    uint32_t res = fork((uint64_t)&counting_function, 1, (uint64_t)"12345");

    if (res != 0)
    {
        printf("Error creating task A\n\r");
        return;
    }

    res = fork((uint64_t)counting_function, 1, (uint64_t)"abcde");
    if (res != 0)
    {
        printf("Error creating task B\n\r");
        return;
    }

    // scheduler_struct *scheduler = get_scheduler();
    // for (int i = 0; i < scheduler->nr_tasks; i++)
    // {
    //     if (scheduler->tasks[i])
    //     {
    //         printf("\tTask %d: state=%d, rem_time=%d\n\r", i, scheduler->tasks[i]->state,
    //                scheduler->tasks[i]->remaining_time);
    //     }
    // }
}