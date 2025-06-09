#include "scheduler/sched.h"
#include "interrupt/daif.h"
#include "mm.h"
#include "scheduler/task.h"
#include "utils/debug.h"
#include "utils/printf.h"

task_struct init_task = INIT_TASK;

int64_t scheduler_space[1024];
scheduler_struct *scheduler = (scheduler_struct *)scheduler_space;

task_struct *get_init_task(void)
{
    return &init_task;
}

scheduler_struct *get_scheduler(void)
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
    page_start->state = TASK_NEW_STATE;
    page_start->remaining_time = page_start->priority;
    page_start->disable_preemption = 1; // disable for new task

    page_start->cpu_context.x19 = function;
    page_start->cpu_context.x20 = arg;
    page_start->cpu_context.pc = REF_FROM_FORM_ADR;
    page_start->cpu_context.sp = (unsigned long)page_start + PAGE_SIZE - 16;
    uint64_t pid = scheduler->nr_tasks++;
    scheduler->tasks[pid] = page_start;
    page_start->id = pid;
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

void scheduler_tick(void)
{

    task_struct *current = scheduler->current_task;
    current->remaining_time--;

    LOG("[scheduler_tick] current task (%d), remaining time: %d, disable preemption? %d\n\r",
        current->id, current->remaining_time, current->disable_preemption);

    if (current->disable_preemption)
    {
        return;
    }

    LOG("[scheduler_tick] preemption enabled for task %d\n\r", current->id);
    LOG("[scheduler_tick] preemption enabled for task %d\n\r", current->id);

    if (current->remaining_time > 0)
    {
        return;
    }

    LOG("[scheduler_tick] Task %d time expired, scheduling next task\n\r", current->id);

    current->remaining_time = 0;
    enable_irq();
    schedule();
    disable_irq();
}

// Revised pick_next_task with priority awareness
task_struct *pick_next_task(void)
{
    task_struct *next = 0;
    uint64_t highest_priority = 0;

    LOG("[pick_next_task] Picking next task from %d tasks\n\r", scheduler->nr_tasks);

    // First pass: Find highest priority ready task
    while (1)
    {
        for (int i = 0; i < scheduler->nr_tasks; i++)
        {
            task_struct *task = scheduler->tasks[i];
            if (!task)
            {
                continue;
            }

            LOG("[pick_next_task] Considering task %d: priority %d, time %d\n\r", i, task->priority,
                task->remaining_time);

            if (task->remaining_time > 0 && task->priority > highest_priority)
            {
                highest_priority = task->priority;
                next = task;
            }
        }

        if (next)
        {
            LOG("[pick_next_task] Selected task %d (priority %d)\n\r", next->id, next->priority);
            return next;
        }

        // If none found with time, recharge and pick highest priority
        LOG("[pick_next_task] No tasks with time, recharging...\n\r");
        for (int i = 0; i < scheduler->nr_tasks; i++)
        {
            task_struct *task = scheduler->tasks[i];
            if (task)
            {
                task->remaining_time = task->priority;
                LOG("[pick_next_task] Recharged task %d to %d\n\r", i, task->remaining_time);
            }
        }
    }
}

void schedule(void)
{
    LOG("[schedule] Scheduling from task %d\n\r", scheduler->current_task->id);
    preempt_disable();
    task_struct *next = pick_next_task();
    context_switch(next);
    LOG("[schedule] Switched to task %d\n\r", next->id);
    preempt_enable();
}

void context_switch(task_struct *next_task)
{
    if (scheduler->current_task == next_task)
    {
        return;
    }

    uint64_t prev = (uint64_t)scheduler->current_task;

    LOG("[context_switch] SWITCH: %d -> %d\n\r", scheduler->current_task->id, next_task->id);
    scheduler->current_task = next_task;
    cpu_switch_to(prev, (uint64_t)next_task);
}
