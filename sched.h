#ifndef _SCHED_H
#define _SCHED_H

#include "common.h"
#include "interrupt/daif.h"
#define THREAD_CPU_CONTEXT 0
#define THREAD_SIZE 4096
#define NR_TASKS 64
#define TASK_RUNNING 0

// Task array access
#define FIRST_TASK task[0]
#define LAST_TASK task[NR_TASKS - 1]

// Initial task definition
#define INIT_TASK {.cpu_context = {0}, .state = 0, .counter = 0, .priority = 1, .preempt_count = 0}

// Globals
extern struct task_struct *current;
extern struct task_struct *task[NR_TASKS];
extern int nr_tasks;

// Scheduler API
void schedule(void);
void switch_to(struct task_struct *next);
void timer_tick(void);
extern void cpu_switch_to(struct task_struct *prev, struct task_struct *next);

// IRQ-safe macros
#define preempt_disable() disable_irq()
#define preempt_enable() enable_irq()

#endif // _SCHED_H