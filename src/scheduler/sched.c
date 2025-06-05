#include "scheduler/sched.h"
#include "mm.h"
#include "scheduler/task.h"

// task_struct init_task = {
//     .cpu_context = {0}, .state = 0, .remaining_time = 0, .priority = 1, .disable_preemption = 0};
task_struct init_task = INIT_TASK;
task_struct *task[MAX_TASKS_NUMBER] = {
    &(init_task),
};

scheduler_struct sched = (scheduler_struct){
    .tasks = task,
    .current_task = &(init_task),
    .nr_tasks = 1,
};

scheduler_struct *SCHEDULER = &sched;

void preempt_disable(void)
{
    SCHEDULER->current_task->disable_preemption = 1;
}

void preempt_enable(void)
{
    SCHEDULER->current_task->disable_preemption = 0;
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
    uint64_t pid = SCHEDULER->nr_tasks++;
    SCHEDULER->tasks[pid] = page_start;
    preempt_enable();
    return 0;
}