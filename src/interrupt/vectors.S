#include "interrupt/vectors.h"

.include "./interrupt/switch.S"

    .macro  handle_invalid_entry type
    mov x0, #\type
    mrs x1, esr_el1 // Exception Syndrome Register
    mrs x2, elr_el1
    bl  show_invalid_entry_message
    b   err_hang
    .endm

    .macro  ventry  label
    .align  7
    b   \label
    .endm

.globl err_hang
err_hang: b err_hang

handle_el1_irq:
	kernel_entry 
	bl	handle_irq
	kernel_exit 

sync_invalid_el1t:
    handle_invalid_entry SYNC_INVALID_EL1t

irq_invalid_el1t:
    handle_invalid_entry IRQ_INVALID_EL1t

fiq_invalid_el1t:
    handle_invalid_entry FIQ_INVALID_EL1t

error_invalid_el1t:
    handle_invalid_entry ERROR_INVALID_EL1t

sync_invalid_el1h:
    handle_invalid_entry SYNC_INVALID_EL1h

fiq_invalid_el1h:
    handle_invalid_entry FIQ_INVALID_EL1h

error_invalid_el1h:
    handle_invalid_entry ERROR_INVALID_EL1h

sync_invalid_el0_64:
    handle_invalid_entry SYNC_INVALID_EL0_64

irq_invalid_el0_64:
    handle_invalid_entry IRQ_INVALID_EL0_64

fiq_invalid_el0_64:
    handle_invalid_entry FIQ_INVALID_EL0_64

error_invalid_el0_64:
    handle_invalid_entry ERROR_INVALID_EL0_64

sync_invalid_el0_32:
    handle_invalid_entry SYNC_INVALID_EL0_32

irq_invalid_el0_32:
    handle_invalid_entry IRQ_INVALID_EL0_32

fiq_invalid_el0_32:
    handle_invalid_entry FIQ_INVALID_EL0_32

error_invalid_el0_32:
    handle_invalid_entry ERROR_INVALID_EL0_32


/**
 * Exception vectors.
 */
.align	11
.globl vectors
vectors:
    ventry  sync_invalid_el1t           // Synchronous EL1t
    ventry  irq_invalid_el1t            // IRQ EL1t
    ventry  fiq_invalid_el1t            // FIQ EL1t
    ventry  error_invalid_el1t          // Error EL1t

    ventry  sync_invalid_el1h           // Synchronous EL1h
    ventry  handle_el1_irq              // IRQ EL1h
    ventry  fiq_invalid_el1h            // FIQ EL1h
    ventry  error_invalid_el1h          // Error EL1h

    ventry  sync_invalid_el0_64         // Synchronous 64-bit EL0
    ventry  irq_invalid_el0_64          // IRQ 64-bit EL0
    ventry  fiq_invalid_el0_64          // FIQ 64-bit EL0
    ventry  error_invalid_el0_64        // Error 64-bit EL0

    ventry  sync_invalid_el0_32         // Synchronous 32-bit EL0
    ventry  irq_invalid_el0_32          // IRQ 32-bit EL0
    ventry  fiq_invalid_el0_32          // FIQ 32-bit EL0
    ventry  error_invalid_el0_32        // Error 32-bit EL0


.globl init_exception_vectors_el1
init_exception_vectors_el1:
    // load VBAR_EL1 with vector table address
    adr	x0, vectors
    msr	vbar_el1, x0 // Vector Base Address Register
    ret

.globl get_vectors_adr
get_vectors_adr:
    adr x0, vectors
    ret

.globl get_vector_base_register
get_vector_base_register:
    mrs x0, vbar_el1
    ret