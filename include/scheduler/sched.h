#pragma once

#include "common.h"
#include "scheduler/task.h"
#define INIT_TASK {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 1, 0, 0}

#define MAX_TASKS_NUMBER 16
#define TASK_NEW_STATE 1
#define TASK_RUNNING_STATE 2

typedef struct
{
    task_struct *tasks[MAX_TASKS_NUMBER];
    task_struct *current_task;
    uint64_t nr_tasks; /* number of tasks currently in the scheduler */
} scheduler_struct;

// TODO: investigate why using the extern key word results in weird behavior
// extern task_struct init_task;
// extern scheduler_struct *scheduler;

task_struct *get_init_task(void);
scheduler_struct *get_scheduler(void);
void init_scheduler(void);
void preempt_disable(void);
void preempt_enable(void);
void schedule_tail(void);
void ret_from_fork(void);
void reset_scheduler(void);
void schedule(void);
void scheduler_tick(void);
void yield(void); /* Voluntary scheduling */
task_struct *pick_next_task(void);
void context_switch(task_struct *next_task);
void cpu_switch_to(uint64_t prev_pointer, uint64_t next_pointer);
/**
 * used for creating a new task
 * @param function the function to run in the new task
 * @param arg the argument to pass to the function
 * @return 0 on success, 1 on failure
 */
uint32_t fork(uint64_t function, uint64_t priority, uint64_t arg);
