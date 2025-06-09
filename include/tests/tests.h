#pragma once

#define FAIL_STR "FAIL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define PASS_STR "PASS"

// configuration tests
void test_sys_configuration(void);
void test_cpu_interrupt_configuration(void);
void test_board_interrupt_configuration(void);

// memory management tests
void test_memory_allocation(void);

void test_scheduler(void);

// Individual test cases
void test_init_task(void);
void test_local_scheduler_struct(void);
void test_global_scheduler_test(void);

void test_fork_success_basic(void);
void test_multiple_tasks_created_correctly(void);
