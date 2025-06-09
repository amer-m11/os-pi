#include "regs/sys.h"

.globl get_current_el
get_current_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2 // first two bits are reserved
    ret

.globl configure_el3
configure_el3:
    ldr x0, =SCR_VALUE
    msr scr_el3, x0
    ret

.globl get_scr_el3
get_scr_el3:
    mrs x0, scr_el3
    ret

.globl configure_el2
configure_el2:
    ldr    x0, =HCR_VALUE
    msr    hcr_el2, x0
    ret

.globl get_hcr_el2
get_hcr_el2:
    mrs x0, hcr_el2
    ret

.globl configure_el1
configure_el1:
    ldr     x0, =SCTLR_VALUE_MMU_DISABLED
    msr     sctlr_el1, x0  
    ret

.globl get_sctlr_el1
get_sctlr_el1:
    mrs x0, sctlr_el1
    ret

.globl switch_to_el1_from_el2
switch_to_el1_from_el2:
    ldr     x0, =SPSR_VALUE
    msr     spsr_el2, x0

    adr     x0, el_entry
    msr     elr_el2, x0

    // save return address and stack pointer
    mov     x1, lr
    mov     x2, sp

    eret

.globl switch_to_el1_from_el3
switch_to_el1_from_el3:
    ldr     x0, =SPSR_VALUE
    msr     spsr_el3, x0

    adr     x0, el_entry
    msr     elr_el3, x0

    // save return address and stack pointer
    mov     x1, lr
    mov     x2, sp

    eret


el_entry:
    // after moving to EL1, we restore the lr and sp registers
    // ELs use different registers(?) but with no ways
    // to access them without being in the same EL
    mov    lr, x1
    mov    sp, x2
    ret

.globl get_spsr_el1
get_spsr_el1:
    mrs x0, spsr_el1
    ret

.globl get_elr_el1
get_elr_el1:
    mrs x0, elr_el1
    ret