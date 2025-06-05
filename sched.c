#include "scheduler/sched.h"
#include "common.h"
#include "interrupt/daif.h" // For enable_irq()/disable_irq()
#include "utils/printf.h"

// Task list initialization
static struct task_struct init_task = INIT_TASK;
struct task_struct *current = &init_task;
struct task_struct *task[MAX_TASKS_NUMBER] = {&init_task};
int nr_tasks = 1;

void _schedule(void)
{
    int next = 0, max_counter = -1;

    // Find task with highest counter
    for (int i = 0; i < NR_TASKS; i++)
    {
        if (task[i] && task[i]->state == TASK_RUNNING && task[i]->counter > max_counter)
        {
            max_counter = task[i]->counter;
            next = i;
        }
    }

    // If no tasks ready, decay counters and retry
    if (max_counter <= 0)
    {
        for (int i = 0; i < NR_TASKS; i++)
        {
            if (task[i])
            {
                task[i]->counter = (task[i]->counter >> 1) + task[i]->priority;
            }
        }
        return;
    }

    switch_to(task[next]);
}

void schedule(void)
{
    current->counter = 0;
    _schedule();
}

void switch_to(struct task_struct *next)
{
    if (current != next)
    {
        struct task_struct *prev = current;
        current = next;
        cpu_switch_to(prev, next);
    }
}

// Timer interrupt handler
void timer_tick(void)
{
    if (--current->counter <= 0 && current->preempt_count == 0)
    {
        current->counter = 0;
        enable_irq();
        _schedule();
        disable_irq();
    }
}