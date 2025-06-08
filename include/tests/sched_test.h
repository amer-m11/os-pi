#pragma once

void test_scheduler(void);

// Individual test cases
void test_init_task(void);
void test_local_scheduler_struct(void);
void test_global_scheduler_test(void);

void test_fork_success_basic(void);
void test_multiple_tasks_created_correctly(void);
void test_task_stack_does_not_overlap(void);
void test_schedule_from_timer(void);
void test_voluntary_yield(void);
void test_remaining_time_decrement(void);
void test_full_scheduling_cycle(void);
void test_preemption_control(void);
void test_mixed_yield_scenarios(void);
