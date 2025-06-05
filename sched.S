#include "scheduler/sched.h"

.globl cpu_switch_to
cpu_switch_to:
    // Save previous task's context
    mov     x10, #THREAD_CPU_CONTEXT
    add     x8, x0, x10            // x8 = &prev->cpu_context
    
    // Save all callee-saved registers (matches their switch.S)
    stp     x19, x20, [x8], #16
    stp     x21, x22, [x8], #16
    stp     x23, x24, [x8], #16
    stp     x25, x26, [x8], #16
    stp     x27, x28, [x8], #16
    stp     x29, x30, [x8], #16    // Saves FP (x29) and LR (x30)
    mrs     x9, sp_el0             // Save user stack pointer
    str     x9, [x8]

    // Restore next task's context
    add     x8, x1, x10            // x8 = &next->cpu_context
    ldp     x19, x20, [x8], #16
    ldp     x21, x22, [x8], #16
    ldp     x23, x24, [x8], #16
    ldp     x25, x26, [x8], #16
    ldp     x27, x28, [x8], #16
    ldp     x29, x30, [x8], #16    // Restores FP and LR
    ldr     x9, [x8]
    msr     sp_el0, x9             // Restore user stack pointer
    
    ret