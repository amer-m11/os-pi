.globl ret_from_fork
ret_from_fork:
	bl	schedule_tail
	mov	x0, x20
	blr	x19 		//should never return
    mov x16, #17
    bl alarm


.globl cpu_switch_to
cpu_switch_to:
    mov    x10, #0                // Offset to cpu_context in task_struct
    add    x8, x0, x10            // Pointer to prev task's cpu_context
    mov    x9, sp
    stp    x19, x20, [x8], #16    // Store callee-saved registers
    stp    x21, x22, [x8], #16
    stp    x23, x24, [x8], #16
    stp    x25, x26, [x8], #16
    stp    x27, x28, [x8], #16
    stp    x29, x9, [x8], #16     // Store FP and SP
    str    x30, [x8]              // Store LR (return address)

    
    add    x8, x1, x10            // Pointer to next task's cpu_context
    ldp    x19, x20, [x8], #16    // Restore callee-saved registers
    ldp    x21, x22, [x8], #16
    ldp    x23, x24, [x8], #16
    ldp    x25, x26, [x8], #16
    ldp    x27, x28, [x8], #16
    ldp    x29, x9, [x8], #16     // Restore FP and SP
    mov    x16, x8
    ldr    x30, [x8]              // Restore LR
    mov    sp, x9
    ret