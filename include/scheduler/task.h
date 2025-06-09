#pragma once

#include "common.h"

struct cpu_context
{
    uint64_t x19;
    uint64_t x20;
    uint64_t x21;
    uint64_t x22;
    uint64_t x23;
    uint64_t x24;
    uint64_t x25;
    uint64_t x26;
    uint64_t x27;
    uint64_t x28;
    uint64_t fp;
    uint64_t sp;
    uint64_t pc; // lr
};

typedef struct
{
    struct cpu_context cpu_context;
    uint64_t state;
    int64_t remaining_time;
    uint64_t
        priority; /* currently, the priority is the time the tasks_array should be running for */
    uint64_t disable_preemption; /* if non-zero preemption is not allowed */
    uint64_t id;
} task_struct;