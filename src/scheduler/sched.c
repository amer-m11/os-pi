#include "scheduler/sched.h"
#include "mm.h"
#include "scheduler/task.h"

task_struct init_task = INIT_TASK;

int64_t scheduler_space[1024];
scheduler_struct *scheduler = (scheduler_struct *)scheduler_space;

task_struct *get_init_task(void)
{
    return &init_task;
}

scheduler_struct *get_schedular(void)
{
    return scheduler;
}

void init_scheduler(void)
{
    memzero((uint64_t)scheduler->tasks, MAX_TASKS_NUMBER);
    scheduler->tasks[0] = &init_task;
    scheduler->current_task = &init_task;
    scheduler->nr_tasks = 1;
}

void preempt_disable(void)
{
    scheduler->current_task->disable_preemption = 1;
}

void preempt_enable(void)
{
    scheduler->current_task->disable_preemption = 0;
}

void schedule_tail(void)
{
    preempt_enable();
}

uint32_t fork(uint64_t function, uint64_t priority, uint64_t arg)
{
    preempt_disable(); // disable for current task

    task_struct *page_start = (task_struct *)allocate_page();
    if (!page_start)
    {
        return 1;
    }
    page_start->priority = priority;
    page_start->state = TASK_RUNNING_STATE;
    page_start->remaining_time = page_start->priority;
    page_start->disable_preemption = 1; // disable for new task

    page_start->cpu_context.x19 = function;
    page_start->cpu_context.x20 = arg;
    page_start->cpu_context.pc = (uint64_t)ret_from_fork;
    page_start->cpu_context.sp = (unsigned long)page_start + PAGE_SIZE - 16;
    uint64_t pid = scheduler->nr_tasks++;
    scheduler->tasks[pid] = page_start;
    preempt_enable();
    return 0;
}

void reset_scheduler(void)
{
    // Free all allocated task pages except init_task
    for (int i = 1; i < MAX_TASKS_NUMBER; i++)
    {
        if (scheduler->tasks[i] != 0 && scheduler->tasks[i] != &init_task)
        {
            free_page((uint64_t)scheduler->tasks[i]);
            scheduler->tasks[i] = 0;
        }
    }

    // Reset scheduler state
    scheduler->current_task = &init_task;
    scheduler->nr_tasks = 1;
}