#pragma once

#include "common.h"
#include "scheduler/task.h"
#define INIT_TASK {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 1, 0}

#define MAX_TASKS_NUMBER 16
#define TASK_RUNNING_STATE 1

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
scheduler_struct *get_schedular(void);
void init_scheduler(void);
void preempt_disable(void);
void preempt_enable(void);
void schedule_tail(void);
void ret_from_fork(void);
void reset_scheduler(void);
/**
 * used for creating a new task
 * @param function the function to run in the new task
 * @param arg the argument to pass to the function
 * @return 0 on success, 1 on failure
 */
uint32_t fork(uint64_t function, uint64_t priority, uint64_t arg);
