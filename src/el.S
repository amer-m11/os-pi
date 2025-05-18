#include "regs/sys.h"

.globl get_current_el
get_current_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2 // first two bits are reserved
    ret


.globl configure_el2
configure_el2:
    ldr    x0, =HCR_VALUE
    msr    hcr_el2, x0
    ret


.globl configure_el1
configure_el1:
    ldr     x0, =SCTLR_VALUE_MMU_DISABLED
    msr     sctlr_el1, x0  
    ret


.globl switch_to_el1
switch_to_el1:
    ldr     x0, =SPSR_VALUE
    msr     spsr_el2, x0

    adr     x0, el1_entry
    msr     elr_el2, x0

    // save return address and stack pointer
    mov     x1, lr
    mov     x2, sp

    eret

el1_entry:
    // after moving to EL1, we restore lr and sp of EL2
    // they use different registers but with no ways
    // to access them without being in the same EL
    mov    lr, x1
    mov    sp, x2
    ret
